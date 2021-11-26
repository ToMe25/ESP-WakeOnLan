/*
 * test.cpp
 *
 * Created on: 24.11.2021
 *
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */

#include "WakeOnLanGenerator.h"
#include <sstream>

AsyncUDPMessage WakeOnLanGenerator::generateWoLPacket(const char *targetMac) {
	AsyncUDPMessage message;
	for (uint8_t i = 0; i < 6; i++) {
		message.write(0xFF);
	}

	const uint8_t *target = parseMacAddr(targetMac);
	for (uint8_t i = 0; i < 16; i++) {
		message.write(target, 6);
	}
	delete[] target;

	return message;
}

const uint8_t* WakeOnLanGenerator::parseMacAddr(const char *mac) {
	if (!isValidMac(mac)) {
		return NULL;
	}

	uint8_t *parsed = new uint8_t[6];
	std::stringstream converter;
	uint temp;

	for (uint8_t i = 0; i < 6; i++) {
		converter << mac[i * 3] << mac[i * 3 + 1];
		converter >> std::hex >> temp;
		parsed[i] = temp;
		converter.str("");
		converter.clear();
	}

	return parsed;
}
