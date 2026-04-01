#include "ha_interface.h"
#include "mqtt_manager.h"
#include <FluxGarage_RoboEyes.h>
#include <Adafruit_SH110X.h>
#include "ha_config.h"

static RoboEyes<Adafruit_SH1106G>* roboEyes = nullptr;

// ---------------- DEVICE JSON ----------------
String deviceJson() {
    return String("{") +
           "\"identifiers\":[\"" DEVICE_ID "\"]," +
           "\"name\":\"" DEVICE_NAME "\"," +
           "\"model\":\"" DEVICE_MODEL "\"," +
           "\"manufacturer\":\"" DEVICE_MANUFACTURER "\"" +
           "}";
}

// ---------------- TOUCH DISCOVERY ----------------
void publishTouchDiscovery() {
    // ---------------- SINGLE TAP ----------------
    {
        String topic = "homeassistant/device_automation/mochi_touch_single/config";

        String payload =
        "{"
            "\"automation_type\":\"trigger\","
            "\"type\":\"action\","
            "\"subtype\":\"single\","
            // "\"payload\":\"single\","
            "\"topic\":\"" TOUCH_TOPIC "\","
            "\"device\":{\"identifiers\":[\"" DEVICE_ID "\"],"
                        "\"name\":\"" DEVICE_NAME "\","
                        "\"model\":\"" DEVICE_MODEL "\","
                        "\"manufacturer\":\"" DEVICE_MANUFACTURER "\"}"
        "}";

        Serial.println("MQTT Publish JSON (single tap):");
        Serial.println(payload);
        mqttPublish(topic.c_str(), payload.c_str(), true);
    }

    // ---------------- DOUBLE TAP ----------------
    {
        String topic = "homeassistant/device_automation/mochi_touch_double/config";

        String payload =
        "{"
            "\"automation_type\":\"trigger\","
            "\"type\":\"action\","
            "\"subtype\":\"double\","
            "\"payload\":\"double\","
            "\"topic\":\"" TOUCH_TOPIC "\","
            "\"device\":{\"identifiers\":[\"" DEVICE_ID "\"],"
                        "\"name\":\"" DEVICE_NAME "\","
                        "\"model\":\"" DEVICE_MODEL "\","
                        "\"manufacturer\":\"" DEVICE_MANUFACTURER "\"}"
        "}";

        Serial.println("MQTT Publish JSON (double tap):");
        Serial.println(payload);
        mqttPublish(topic.c_str(), payload.c_str(), true);
    }
}
// ---------------- HA INIT ----------------
template <typename T>
void haInit(RoboEyes<T>* eyes) {
    roboEyes = eyes;
    publishTouchDiscovery();

    // publish availability online
    mqttPublish(STATUS_TOPIC, "online", true); // retain = true
}

// Explicit instantiation
template void haInit(RoboEyes<Adafruit_SH1106G>* eyes);

// ---------------- SEND TOUCH EVENT ----------------
void haPublishTouch(const char* event) {
    mqttPublish(TOUCH_TOPIC, event, false);
}

// ---------------- HANDLE COMMANDS ----------------
void haHandleMessage(char* topic, String payload) {
    // parse animations/sounds later
}