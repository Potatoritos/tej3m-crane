#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
    Button(int pin_): pin(pin_) {}

    // Sets up the button's pin
    void setPinMode();

    // Returns true if the button is pressed down; false otherwise
    boolean isDown();

    // Returns true if the button's state is on and different
    // from the state the last time isStateChanged() was called;
    // false otherwise
    boolean isStateChanged();

private:
    const int pin;
    boolean prevState;
};

#endif
