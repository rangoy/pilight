/*
	Copyright (C) 2013 CurlyMo

	This file is part of pilight.

    pilight is free software: you can redistribute it and/or modify it under the 
	terms of the GNU General Public License as published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later 
	version.

    pilight is distributed in the hope that it will be useful, but WITHOUT ANY 
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR 
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pilight. If not, see	<http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pilight.h"
#include "common.h"
#include "log.h"
#include "protocol.h"
#include "hardware.h"
#include "binary.h"
#include "gc.h"
#include "alecto.h"

void alectoParseCode(void) {
	int i = 0, x = 0;
	int temperature;
	int id;

	for(i=1;i<alecto->rawlen-1;i+=2) {
		alecto->binary[x++] = alecto->code[i];
	}

	id = binToDecRev(alecto->binary, 0, 11);
	temperature = binToDecRev(alecto->binary, 16, 27);

	alecto->message = json_mkobject();
	json_append_member(alecto->message, "id", json_mknumber(id));
	json_append_member(alecto->message, "temperature", json_mknumber(temperature));
}

void alectoInit(void) {
	
	protocol_register(&alecto);
	protocol_set_id(alecto, "alecto");
	protocol_device_add(alecto, "alecto", "Alecto weather stations");
	protocol_plslen_add(alecto, 270);
	alecto->devtype = WEATHER;
	alecto->hwtype = RX433;
	alecto->pulse = 14;
	alecto->rawlen = 74;
	alecto->lsb = 3;

	options_add(&alecto->options, 't', "temperature", has_value, config_value, "^[0-9]{1,3}$");
	options_add(&alecto->options, 'i', "id", has_value, config_id, "[0-9]");

	protocol_setting_add_number(alecto, "decimals", 1);
	protocol_setting_add_number(alecto, "humidity", 0);
	protocol_setting_add_number(alecto, "temperature", 1);
	protocol_setting_add_number(alecto, "battery", 0);
	
	alecto->parseCode=&alectoParseCode;
}
