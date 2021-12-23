/*
 * NetworkHandler.cpp
 *
 * Created on: 23.11.2021
 *
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */

#include <NetworkHandler.h>
#include <WakeOnLanGenerator.h>
#include <regex>
#include <sstream>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

IPAddress NetworkHandler::localhost = IPADDR_NONE;
IPv6Address NetworkHandler::localhost_ipv6;
AsyncWebServer NetworkHandler::webServer(WEBSERVER_PORT);
std::string NetworkHandler::indexHtml = INDEX_HTML;
std::vector<std::string> NetworkHandler::deviceMacs;
IPAddress NetworkHandler::targetBroadcast = DEFAULT_TARGET_BROADCAST;
AsyncUDP NetworkHandler::udp;

void NetworkHandler::setup() {
	setupWiFi();
	setupWebServer();
	setupOTA();
}

void NetworkHandler::setupWiFi() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect(1);
	WiFi.onEvent(onWiFiEvent);

	if (!WiFi.config(localhost, GATEWAY, SUBNET)) {
		Serial.println("Configuring WiFi failed!");
		return;
	}

	WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void NetworkHandler::loop() {
	ArduinoOTA.handle();
}

void NetworkHandler::onWiFiEvent(WiFiEvent_t event) {
	switch (event) {
	case SYSTEM_EVENT_STA_START:
		WiFi.setHostname(HOSTNAME);
		break;
	case SYSTEM_EVENT_STA_CONNECTED:
		Serial.println("WiFi connected.");
		WiFi.enableIpV6();
		break;
	case SYSTEM_EVENT_GOT_IP6:
		Serial.print("STA IPv6: ");
		Serial.println(localhost_ipv6 = WiFi.localIPv6());
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		Serial.print("STA IP: ");
		Serial.println(localhost = WiFi.localIP());

		if (DEFAULT_TARGET_BROADCAST == IPAddress((uint) 0)) {
			targetBroadcast = WiFi.broadcastIP();
		}
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		WiFi.reconnect();
		break;
	default:
		break;
	}
}

void NetworkHandler::setupWebServer() {
	std::string deviceMacs = DEVICE_MACS;
	deviceMacs.erase(std::remove(deviceMacs.begin(), deviceMacs.end(), ' '),
			deviceMacs.end());
	deviceMacs.erase(std::remove(deviceMacs.begin(), deviceMacs.end(), '\r'),
			deviceMacs.end());

	std::istringstream deviceStream(deviceMacs);
	std::string device;
	while (std::getline(deviceStream, device, '\n')) {
		if (device.length() > 0) {
			NetworkHandler::deviceMacs.push_back(device);
		}
	}

	webServer.on("/", HTTP_GET, onIndexGet);
	webServer.on("/", HTTP_POST, onIndexPost);
	webServer.on("/index.html", HTTP_GET, onIndexGet);
	webServer.on("/index.html", HTTP_POST, onIndexPost);

	webServer.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/css", MAIN_CSS);
	});

	webServer.onNotFound([](AsyncWebServerRequest *request) {
		request->send(404, "text/html", NOT_FOUND_HTML);
	});

	webServer.begin();
}

void NetworkHandler::setupOTA() {
	ArduinoOTA.setHostname(HOSTNAME);
	ArduinoOTA.setPassword(OTA_PASS);

	ArduinoOTA.onStart([]() {
		Serial.println("Start updating sketch.");
	});

	ArduinoOTA.onProgress([](uint progress, uint total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onEnd([]() {
		Serial.println("\nUpdate Done.");
	});

	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("OTA Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed.");
		} else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed.");
		} else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed.");
		} else if (error == OTA_RECEIVE_ERROR) {
			Serial.println("Receive Failed.");
		} else if (error == OTA_END_ERROR) {
			Serial.println("End Failed.");
		}
	});

	ArduinoOTA.begin();
	MDNS.addService("http", "tcp", 80);
}

void NetworkHandler::onIndexGet(AsyncWebServerRequest *request) {
	std::string response = indexHtml;

	std::string devices;
	for (std::string device : deviceMacs) {
		devices += "<option value=\"";
		devices += device;
		devices += "\">\n";
	}
	response = std::regex_replace(response, std::regex("\\$devices"),
			devices.c_str());

	std::string targets = "<option value=\"";
	targets += targetBroadcast.toString().c_str();
	targets += "\">\n";
	IPAddress broadcast(255, 255, 255, 255);
	if (targetBroadcast != broadcast) {
		targets += "<option value=\"";
		targets += broadcast.toString().c_str();
		targets += "\">\n";
	}
	response = std::regex_replace(response, std::regex("\\$targets"),
			targets.c_str());

	response = std::regex_replace(response, std::regex("\\$device"), "");
	response = std::regex_replace(response, std::regex("\\$target"),
			targetBroadcast.toString().c_str());

	request->send(200, "text/html", response.c_str());
}

void NetworkHandler::onIndexPost(AsyncWebServerRequest *request) {
	if (request->hasParam("device", true) && request->hasParam("target", true)) {
		String device = request->getParam("device", true)->value();
		String target = request->getParam("target", true)->value();
		std::string response = indexHtml;

		std::string devices;
		for (std::string device : deviceMacs) {
			devices += "<option value=\"";
			devices += device;
			devices += "\">\n";
		}
		response = std::regex_replace(response, std::regex("\\$devices"),
				devices.c_str());

		std::string targets = "<option value=\"";
		targets += targetBroadcast.toString().c_str();
		targets += "\">\n";
		IPAddress broadcast(255, 255, 255, 255);
		if (targetBroadcast != broadcast) {
			targets += "<option value=\"";
			targets += broadcast.toString().c_str();
			targets += "\">\n";
		}
		response = std::regex_replace(response, std::regex("\\$targets"),
				targets.c_str());

		response = std::regex_replace(response, std::regex("\\$target"),
				target.c_str());
		response = std::regex_replace(response, std::regex("\\$device"),
				device.c_str());

		IPAddress originalBroadcastTarget = targetBroadcast;

		if (!WakeOnLanGenerator::isValidMac(device.c_str())) {
			std::string message = "error\">Received invalid device mac address \"";
			message += device.c_str();
			message += "\".";

			response = std::regex_replace(response, std::regex("\\$message_type\" hidden>\\$message"), message.c_str());
			request->send(400, "text/html", response.c_str());
		} else if (!targetBroadcast.fromString(target)) {
			targetBroadcast = originalBroadcastTarget;
			std::string message = "error\">Received invalid target IP address \"";
			message += target.c_str();
			message += "\".";

			response = std::regex_replace(response, std::regex("\\$message_type\" hidden>\\$message"), message.c_str());
			request->send(400, "text/html", response.c_str());
		} else {
			Serial.print("Waking up device ");
			Serial.print(device);
			Serial.println('.');

			AsyncUDPMessage wakePacket = WakeOnLanGenerator::generateWoLPacket(device.c_str());
			udp.sendTo(wakePacket, targetBroadcast, TARGET_PORT);

			std::string message = "success\">Woke up device ";
			message += device.c_str();
			message += '.';

			response = std::regex_replace(response, std::regex("\\$message_type\" hidden>\\$message"), message.c_str());
			request->send(200, "text/html", response.c_str());
		}
	} else {
		onIndexGet(request);
	}
}
