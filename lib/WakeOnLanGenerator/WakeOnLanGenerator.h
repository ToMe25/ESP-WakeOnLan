/*
 * WakeOnLanGenerator.h
 *
 * Created on: 24.11.2021
 *
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */

#ifndef LIB_WAKEONLANGENERATOR_H_
#define LIB_WAKEONLANGENERATOR_H_

#include <regex>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncUDP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#endif

static const std::regex MAC_REGEX(
		"[a-fA-F0-9]{2}:[a-fA-F0-9]{2}:[a-fA-F0-9]{2}:[a-fA-F0-9]{2}:[a-fA-F0-9]{2}:[a-fA-F0-9]{2}");

class WakeOnLanGenerator {
public:

	/*
	 * Generates a Wake on Lan packet for the given target mac address.
	 */
	static AsyncUDPMessage generateWoLPacket(std::string targetMac) {
		return generateWoLPacket(targetMac.c_str());
	}

	/*
	 * Generates a Wake on Lan packet for the given target mac address.
	 */
	static AsyncUDPMessage generateWoLPacket(const char *targetMac);

	/*
	 * Parses the given mac address from a string to an array of 6 bytes.
	 * Returns NULL if given an invalid mac address.
	 */
	static const uint8_t* parseMacAddr(const std::string mac) {
		return parseMacAddr(mac.c_str());
	}

	/*
	 * Parses the given mac address from a string to an array of 6 bytes.
	 * Returns NULL if given an invalid mac address.
	 */
	static const uint8_t* parseMacAddr(const char *mac);

	/*
	 * Checks whether the given string is a valid mac address.
	 */
	static const bool isValidMac(const std::string mac) {
		return isValidMac(mac.c_str());
	}

	/*
	 * Checks whether the given string is a valid mac address.
	 */
	static const bool isValidMac(const char *mac) {
		return std::regex_match(mac, MAC_REGEX);
	}
};

#endif /* LIB_WAKEONLANGENERATOR_H_ */
