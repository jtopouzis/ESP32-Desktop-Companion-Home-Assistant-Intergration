#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

void buzzerInit(uint8_t pin);
void buzzerPlayRTTTL(const char* melody);
void buzzerStop();
void buzzerUpdate();
bool buzzerIsPlaying();

#endif