/*
 * NetworkHandler.h
 *
 * Created on: 23.11.2021
 *
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */

#ifndef SRC_NETWORKHANDLER_H_
#define SRC_NETWORKHANDLER_H_

#include <string>
#include <ESPAsyncWebServer.h>
#ifdef ESP32
#include <AsyncUDP.h>
#elif defined(ESP8266)
#include <ESPAsyncUDP.h>
#endif

// Includes the content of the file "wifissid.txt" in the project root.
// Make sure this file doesn't end with an empty line.
extern const char WIFI_SSID[] asm("_binary_wifissid_txt_start");
// Includes the content of the file "wifipass.txt" in the project root.
// Make sure this file doesn't end with an empty line.
extern const char WIFI_PASS[] asm("_binary_wifipass_txt_start");
// Includes the content of the file "otapass.txt" in the project root.
// Make sure this file doesn't end with an empty line.
extern const char OTA_PASS[] asm("_binary_otapass_txt_start");

// Includes the content of the file "devices.txt" in the project root.
// Each line in that file should contain one mac address.
extern const char DEVICE_MACS[] asm("_binary_devices_txt_start");

extern const char INDEX_HTML[] asm("_binary_src_html_index_html_start");
extern const char INDEX_JS[] asm("_binary_src_html_index_js_start");
extern const char MAIN_CSS[] asm("_binary_src_html_main_css_start");
extern const char NOT_FOUND_HTML[] asm("_binary_src_html_not_found_html_start");

// WiFi constants
static const char HOSTNAME[] = "esp-wol";
static const IPAddress GATEWAY = IPADDR_ANY;
static const IPAddress SUBNET = IPADDR_ANY;

// Web Server constants
static const uint16_t WEBSERVER_PORT = 80;
static const IPAddress DEFAULT_TARGET_BROADCAST = (uint) 0;// set to 0 to use the local network broadcast ip instead.

// Wake on Lan constants
static const uint16_t TARGET_PORT = 9;

class NetworkHandler {
public:
	static void setup();

	static void setupWiFi();

	static void loop();
private:

	// WiFi variables
	static IPAddress localhost;

	// Web Server variables
	static AsyncWebServer webServer;
	static std::vector<std::string> deviceMacs;
	static IPAddress targetBroadcast;

	// Wake on Lan variables
	static AsyncUDP udp;

	static void onWiFiEvent(WiFiEvent_t event);

	/**
	 * Set up the Web Server.
	 */
	static void setupWebServer();

	static void setupOTA();

	static void onIndexGet(AsyncWebServerRequest *request);
	static void onIndexPost(AsyncWebServerRequest *request);

	/**
	 * Replaces some variables in indexHtml to prepare it for being sent to the client.
	 *
	 * @param device	The currently selected device MAC address.
	 * @param target	The currently selected target broadcast ip address.
	 * @return	The prepared index.html string.
	 */
	static std::string prepareIndexResponse(const String device, const String target);
};

#endif /* SRC_NETWORKHANDLER_H_ */
