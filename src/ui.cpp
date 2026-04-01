#include "ui.h"

static Adafruit_SH1106G* display = nullptr;

void uiInit(Adafruit_SH1106G* disp) {
    display = disp;
}

void uiDrawWifiIcon(bool connected) {

    if (!display) return;

    int x = 0;
    int y = 0;

    if (connected) {
        // WiFi arcs
        display->drawCircle(x + 6, y + 6, 5, SH110X_WHITE);
        display->drawCircle(x + 6, y + 6, 3, SH110X_WHITE);
        display->fillCircle(x + 6, y + 6, 1, SH110X_WHITE);
    } else {
        // Disconnected icon
        display->drawCircle(x + 6, y + 6, 5, SH110X_WHITE);
        display->drawLine(x + 2, y + 2, x + 10, y + 10, SH110X_WHITE);
        display->drawLine(x + 10, y + 2, x + 2, y + 10, SH110X_WHITE);
    }
}