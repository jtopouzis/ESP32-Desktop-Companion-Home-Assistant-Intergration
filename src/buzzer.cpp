#include "buzzer.h"
#include <anyrtttl.h>

static bool isPlaying = false;
static uint8_t buzzerPinGlobal = 0;

void buzzerInit(uint8_t pin) {
    buzzerPinGlobal = pin;
}

void buzzerPlayRTTTL(const char* melody) {
    // Only start if not already playing
    if (!isPlaying) {
        anyrtttl::nonblocking::begin(buzzerPinGlobal, melody);
        isPlaying = true;
    }
}

void buzzerStop() {
    anyrtttl::nonblocking::stop();
    isPlaying = false;
}

void buzzerUpdate() {
    if (isPlaying) {
        anyrtttl::nonblocking::play();  // advance notes
        if (!anyrtttl::nonblocking::isPlaying()) {
            isPlaying = false;
        }
    }
}

bool buzzerIsPlaying() {
    return isPlaying;
}