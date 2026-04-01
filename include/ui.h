#ifndef UI_H
#define UI_H

#include <Adafruit_SH110X.h>

void uiInit(Adafruit_SH1106G* disp);
void uiDrawWifiIcon(bool connected);

#endif