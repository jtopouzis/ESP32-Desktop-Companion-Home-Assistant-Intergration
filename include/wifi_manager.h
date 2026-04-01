// wifi_manager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SH110X.h>
#include "config.h"

extern bool messageActive;  // ← declare global

// Assign optional display for status messages
void setDisplay(Adafruit_SH1106G* disp);

// Initialize WiFi (nonblocking)
void wifiInit();

// Call in every loop
void wifiLoop();

// Returns current WiFi status
bool isWiFiConnected();

// Show a message immediately (nonblocking)
void showMessage(const String& line1, const String& line2 = "", unsigned long duration_ms = 3000);

void startWifiManager();

bool isWiFiReady();  // tells if wifiTask finished initialization
#endif