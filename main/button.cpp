#include "button.h"

void Button::setPinMode() {
    pinMode(pin, INPUT_PULLUP);
}

boolean Button::isDown() {
    // Returns true if the button is pressed down; false otherwise
    return !digitalRead(pin);
}

boolean Button::isStateChanged() {
    // Returns true if the button's state is on and different
    // from the state the last time isStateChanged() was called;
    // false otherwise
    boolean down = isDown();
    boolean changed = down && !prevState;
    prevState = down;
    return changed;
}
