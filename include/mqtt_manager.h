#pragma once
#include <Arduino.h>
#include "ha_config.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <FluxGarage_RoboEyes.h>

template <typename T>
void mqttSetup(RoboEyes<T>* eyes);

void mqttLoop();
bool mqttPublish(const char* topic, const char* payload, bool retained = false);
bool mqttSubscribe(const char* topic);
bool mqttConnect();