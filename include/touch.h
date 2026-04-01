#ifndef TOUCH_H
#define TOUCH_H

enum TouchEvent {
    TOUCH_NONE,
    TOUCH_SINGLE_TAP,
    TOUCH_DOUBLE_TAP,
    TOUCH_LONG_PRESS
};

TouchEvent touchUpdate();

#endif