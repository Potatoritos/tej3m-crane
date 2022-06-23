/*

TEJ3M Final Project - Crane
by Elliott Cheng and Brendan Tam

A robot arm that is controlled by two joysticks.

The left joystick controls the arm's rotation, and
the right joystick controls the arm's x-y coordinates.

Pressing each joystick button once independently saves
the arm's current position; pressing it again moves
the arm back to the saved position.

*/


#include "button.h"
#include "crane.h"

#include <EEPROM.h>

const int pinJoystickX = A1, pinJoystickY = A0, pinJoystickRotation = A2;
const int pinJoystickButton1 = 2, pinJoystickButton2 = 13;
const int pinServoRotation = 9, pinServoShoulder = 10, pinServoElbow = 11;

Crane crane(13.7, 14.5);

Button saveButtons[] = {Button(pinJoystickButton1), Button(pinJoystickButton2)};
Position savedPositions[2];
boolean saveModes[] = {true, true};

int writePosCounter = 0;

void setup() {
    Serial.begin(9600);

    // Set up crane and button pins
    crane.attachServos(pinServoRotation, pinServoShoulder, pinServoElbow);

    for (Button &button : saveButtons) {
        button.setPinMode();
    }

    // Get initial position from EEPROM
    Position initialPosition;
    EEPROM.get(0, initialPosition);

    // Set initial position to default value if
    // value from EEPROM is NaN
    if (initialPosition.x != initialPosition.x) {
        initialPosition.x = 20;
    }
    if (initialPosition.y != initialPosition.y) {
        initialPosition.y = 10;
    }

    // Set the crane's position to initialPosition
    crane.move(initialPosition);
    crane.update();

    // Slowly move to the starting position
    crane.move(Position({20, 10, 30}), 40);
    crane.updateUntilMoveDone();
}

void loop() {
    const long startMillis = millis();

    // Save position / move to saved position when the save button is pressed
    for (int i = 0; i <= 1; i++) {
        if (saveButtons[i].isStateChanged()) {
            if (saveModes[i]) {
                savedPositions[i] = Position(crane.position());
            } else {
                crane.move(savedPositions[i], 15);
                crane.updateUntilMoveDone();
            }
            saveModes[i] = !saveModes[i];
        }
    }

    // Get input from joysticks
    float dx = analogRead(pinJoystickX)/2000.0 - 0.256;
    float dy = analogRead(pinJoystickY)/2000.0 - 0.256;

    float dRotation = analogRead(pinJoystickRotation)/2000.0 - 0.256;

    // Move if |dx|, |dy|, or |dRotation| is above a certain threshold
    if (abs(dx) > 0.1 || abs(dy) > 0.1 || abs(dRotation) > 0.1) {
        crane.move(crane.position() + Position({-dx, -dy, 5*dRotation}));
    }

    // Write to the crane's servos
    crane.update();

    // Save position to EEPROM once every ~3 seconds
    writePosCounter++;
    if (!(writePosCounter &= 63)) {
        EEPROM.put(0, crane.position());
    }

    // Delay such that loop() loops every 50ms
    delay(max(0, 50 - min(50, millis() - startMillis)));
}
