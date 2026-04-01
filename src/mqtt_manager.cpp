#include "mqtt_manager.h"
#include "ha_interface.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <FluxGarage_RoboEyes.h>
#include <Adafruit_SH110X.h>
#include "ha_config.h"
#include "config.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastReconnectAttempt = 0;
static RoboEyes<Adafruit_SH1106G>* roboEyes = nullptr;

// ---------------- MQTT CALLBACK ----------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++)
        msg += (char)payload[i];
    haHandleMessage(topic, msg);
}

// ---------------- MQTT CONNECT ----------------
bool mqttConnect() {
    Serial.println("Attempting MQTT connection...");

    if (mqttClient.connect("MochiRobot", MQTT_USER, MQTT_PASSWORD, STATUS_TOPIC, 0, true, "offline")) {
        // Publish ONLINE status to HA with retain = true
        mqttPublish(STATUS_TOPIC, "online", true);  // <--- THIS IS KEY

        // Subscribe to commands/topics
        mqttClient.subscribe("mochi/#");

        Serial.println("MQTT connected");
        return true;
    }

    Serial.print("MQTT failed, rc=");
    Serial.println(mqttClient.state());
    return false;
}

// ---------------- MQTT SETUP ----------------
template <typename T>
void mqttSetup(RoboEyes<T>* eyes) {
    roboEyes = eyes;
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    lastReconnectAttempt = 0;
}

// Explicit instantiation
template void mqttSetup(RoboEyes<Adafruit_SH1106G>* eyes);

// ---------------- MQTT LOOP ----------------
void mqttLoop() {
    if (!mqttClient.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = now;
            Serial.println("Attempting MQTT reconnect...");
            if (mqttConnect())
                lastReconnectAttempt = 0;
        }
    } else {
        mqttClient.loop();
    }
}

bool mqttPublish(const char* topic, const char* payload, bool retained) {
    if (!mqttClient.connected()) return false;
    return mqttClient.publish(topic, payload, retained);
}

bool mqttSubscribe(const char* topic) {
    if (!mqttClient.connected()) return false;
    return mqttClient.subscribe(topic);
}