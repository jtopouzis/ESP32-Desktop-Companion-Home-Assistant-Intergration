#pragma once
#include <Arduino.h>
#include "ha_config.h"

template <typename T>
class RoboEyes;

template <typename T>
void haInit(RoboEyes<T>* eyes);

void haPublishTouch(const char* event);
void haHandleMessage(char* topic, String payload);