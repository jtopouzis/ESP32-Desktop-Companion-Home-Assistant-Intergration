#pragma once
#ifndef CONFIG_H
#define CONFIG_H

// =====================
// WIFI
// =====================

#define WIFI_SSID "**********"
#define WIFI_PASSWORD "**********************"

#define RECONNECT_TIMER 30000 // Try to reconnect on WiFi disconnect every XX seconds

// =====================
// MQTT SERVER AND PORT (HOME ASSISTANT)
// =====================
#define MQTT_SERVER "192.168.1.17"
#define MQTT_PORT 1883
#define MQTT_USER "motchi"
#define MQTT_PASSWORD "motchiesp"

// =====================
// OLED SETTINGS
// =====================
#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DISPLAY_ENABLED 0 // 1 to enable, 0 to disable messages eg wifi connected etc

// =====================
// I2C PINS
// =====================
#define SDA_PIN 8
#define SCL_PIN 9

// =====================
// TOUCH SENSOR
// =====================
#define TOUCH_PIN 7
#define LONG_PRESS_TIME 700      // ms
#define DOUBLE_TAP_TIME 300      // ms

// =====================
// BUZZER SENSOR
// =====================
#define BUZZER_PIN 5

// =====================
// ROBOEYES DEFAULTS
// =====================
#define ROBO_FPS 60

#define AUTOBLINK ON
#define AUTOBLINK_INTERVAL 3
#define AUTOBLINK_VARIATION 2

#define IDLE ON
#define IDLE_INTERVAL 7
#define IDLE_VARIATION 5

//   ### Define Eye Shapes, all values in pixels
#define E_WIDTH_L 30 // _(byte leftEye)_
#define E_WIDTH_R 30 // _(byte rightEye)_
#define E_HEIGHT_L 32 // _(byte leftEye)_
#define E_HEIGHT_R 32 // _(byte rightEye)_
#define E_BORDERRADIUS_L 5 // _(byte leftEye)_
#define E_BORDERRADIUS_R 5 // _(byte rightEye)_
#define E_SPACEBETWEEN 15 // _(int space) -> can also be negative_
#define E_CYCLOPS OFF // _(bool ON/OFF) -> if turned ON, robot has only on eye_

#endif