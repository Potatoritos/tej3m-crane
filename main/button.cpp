#include "button.h"

void Button::setPinMode() {
    pinMode(pin, INPUT_PULLUP);
}

boolean Button::isDown() {
    return !digitalRead(pin);
}

boolean Button::isStateChanged() {
    const boolean down = isDown();
    const boolean changed = down && !prevState;
    prevState = down;
    return changed;
}
