#include <Arduino.h>
#include "config.h"
#include "touch.h"

static bool lastState = LOW;
static unsigned long pressStart = 0;
static unsigned long lastRelease = 0;

static bool waitingSecondTap = false;
static bool longPressTriggered = false;

TouchEvent touchUpdate() {

    bool currentState = digitalRead(TOUCH_PIN);
    unsigned long now = millis();

    // =========================
    // PRESS START
    // =========================
    if (currentState == HIGH && lastState == LOW) {
        pressStart = now;
        longPressTriggered = false;
    }

    // =========================
    // LONG PRESS (while holding)
    // =========================
    if (currentState == HIGH && !longPressTriggered) {
        if (now - pressStart >= LONG_PRESS_TIME) {
            longPressTriggered = true;
            waitingSecondTap = false;
            lastState = currentState;
            return TOUCH_LONG_PRESS;
        }
    }

    // =========================
    // RELEASE
    // =========================
    if (currentState == LOW && lastState == HIGH) {

        // If long press already triggered, ignore release
        if (longPressTriggered) {
            lastState = currentState;
            return TOUCH_NONE;
        }

        // Short tap handling
        if (waitingSecondTap && (now - lastRelease) < DOUBLE_TAP_TIME) {
            waitingSecondTap = false;
            lastState = currentState;
            return TOUCH_DOUBLE_TAP;
        } else {
            waitingSecondTap = true;
            lastRelease = now;
        }
    }

    // =========================
    // SINGLE TAP timeout
    // =========================
    if (waitingSecondTap && (now - lastRelease) >= DOUBLE_TAP_TIME) {
        waitingSecondTap = false;
        lastState = currentState;
        return TOUCH_SINGLE_TAP;
    }

    lastState = currentState;
    return TOUCH_NONE;
}