#include "wifi_manager.h"
#include <esp_wifi.h>
#include "config.h"

static Adafruit_SH1106G* debugDisplay = nullptr;

bool messageActive = false;

static volatile bool wifiConnected = false;
static volatile bool wifiReady = false;
static bool lastConnectedState = false;

static unsigned long messageStart = 0;
static unsigned long messageDuration = 0;

static unsigned long lastReconnectAttempt = 0;
static unsigned long lastDotUpdate = 0;
static int dotCount = 0;

static bool showingConnectingAnimation = false;

// ===== INTERNAL FUNCTION =====
static void drawText(const String& line1, const String& line2 = "") {

    if (!debugDisplay) return;

    debugDisplay->clearDisplay();
    debugDisplay->setCursor(0, 0);
    debugDisplay->setTextSize(1);
    debugDisplay->setTextColor(SH110X_WHITE);
    debugDisplay->println(line1);
    if (line2.length() > 0)
        debugDisplay->println(line2);

    debugDisplay->display();
}

// ===== TIMED MESSAGE =====
static void startMessage(const String& l1, const String& l2, unsigned long duration) {

#if DISPLAY_ENABLED
    messageActive = true;
    messageStart = millis();
    messageDuration = duration;
    showingConnectingAnimation = false;

    drawText(l1, l2);
#else
    messageActive = false;   // force it OFF
#endif
}

void setDisplay(Adafruit_SH1106G* disp) {
    debugDisplay = disp;
}

void wifiInit() {

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.enableSTA(true);
    esp_wifi_set_country_code("GR", true);
    WiFi.setTxPower(WIFI_POWER_8_5dBm);

    wifiConnected = (WiFi.status() == WL_CONNECTED);
    lastConnectedState = wifiConnected;

    lastReconnectAttempt = millis();
    lastDotUpdate = millis();
    dotCount = 0;

    WiFi.setSleep(false);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    startMessage("Connecting to WiFi", String(WIFI_SSID), 3000);
}

void wifiLoop() {

    #if !DISPLAY_ENABLED
        messageActive = false;      // eyes never blocked
        showingConnectingAnimation = false;  // prevent flicker animation
    #endif

    unsigned long now = millis();
    bool connectedNow = (WiFi.status() == WL_CONNECTED);

    // ===== MESSAGE TIMEOUT =====
    if (messageActive && (now - messageStart > messageDuration)) {

        messageActive = false;
        messageDuration = 0;

        #if DISPLAY_ENABLED
            if (debugDisplay)
                debugDisplay->clearDisplay();

            // After first message expires and still disconnected → start animation
            if (!connectedNow) {
                showingConnectingAnimation = true;
            }
        #endif
    }

    // ===== RECONNECT ATTEMPT =====
    if (!connectedNow && now - lastReconnectAttempt > RECONNECT_TIMER) {

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        lastReconnectAttempt = now;

        startMessage("WiFi lost!", "Reconnecting...", 3000);
    }

    // ===== CONNECTING DOT ANIMATION =====
    #if DISPLAY_ENABLED
        if (!connectedNow && showingConnectingAnimation && !messageActive) {

            if (now - lastDotUpdate > 500) {

                dotCount = (dotCount + 1) % 4;

                if (debugDisplay) {
                    debugDisplay->clearDisplay();
                    debugDisplay->setCursor(0, 0);
                    debugDisplay->setTextSize(1);
                    debugDisplay->setTextColor(SH110X_WHITE);

                    debugDisplay->print("Connecting to WiFi");

                    for (int i = 0; i < dotCount; i++)
                        debugDisplay->print(".");

                    debugDisplay->display();
                }

                lastDotUpdate = now;
            }
        }
    #endif

    // ===== JUST CONNECTED EVENT =====
    if (connectedNow && !lastConnectedState) {

        showingConnectingAnimation = false;

        startMessage(
            "WiFi Connected!",
            "IP: " + WiFi.localIP().toString(),
            3000
        );
    }

    lastConnectedState = connectedNow;
    wifiConnected = connectedNow;
}

bool isWiFiConnected() {
    return wifiConnected;
}

TaskHandle_t wifiTaskHandle = NULL;

void wifiTask(void *parameter)
{
    wifiInit();
    wifiReady = true;      // signal that WiFi has initialized

    while(true) {
        wifiLoop();
        if(WiFi.isConnected()) vTaskDelay(50 / portTICK_PERIOD_MS);
        else vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

bool isWiFiReady() {
    return wifiReady;
}

void startWifiManager()
{
    xTaskCreatePinnedToCore(
        wifiTask,           // function
        "wifiTask",         // task name
        4096,               // stack size
        NULL,
        0,                  // priority
        &wifiTaskHandle,
        0                   // CORE 0 (WiFi core)
    );
}