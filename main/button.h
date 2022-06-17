#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
    Button(int pin_): pin(pin_) {}

    void setPinMode();
    boolean isDown();
    boolean isStateChanged();

private:
    const int pin;
    boolean prevState;
};

#endif
