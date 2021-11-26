/*
 * main.cpp
 *
 * Created on: 23.11.2021
 *
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */

#include <main.h>
#include <NetworkHandler.h>

void setup() {
	Serial.begin(115200);

	NetworkHandler::setup();
}

void loop() {
	NetworkHandler::loop();
}
