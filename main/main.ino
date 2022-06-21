/*

TEJ3M Final Project - Crane
by Elliott Cheng and Brendan Tam

A robot arm that is controlled by a joystick and a button.

The joystick button toggles controls between two modes: rotation mode and move mode.
In move mode, the joystick controls the x-y coordinates of the tip of the arm.
In rotation mode, the joystick controls the rotation of the arm.

Pressing the button once saves the arm's current position; pressing it again moves
the arm back to the saved position.

*/


#include "button.h"
#include "crane.h"

#include <EEPROM.h>

const int pinJoystickX = A1, pinJoystickY = A0;
const int pinJoystickButton = 2;
const int pinSaveButton = 13;
const int pinServoRotation = 9, pinServoShoulder = 10, pinServoElbow = 11;

Crane crane(13.7, 15);

Button buttonJoystick(pinJoystickButton);
Button buttonSave(pinSaveButton);

Position savedPosition;

boolean rotationMode = false;
boolean saveMode = true;

int writePosCounter = 0;

void setup() {
    Serial.begin(9600);

    // Set up crane and button pins
    crane.attachServos(pinServoRotation, pinServoShoulder, pinServoElbow);
    buttonJoystick.setPinMode();
    buttonSave.setPinMode();

    // Get initial position from EEPROM
    Position initialPosition;
    EEPROM.get(0, initialPosition);

    Serial.println("Read from EEPROM");
    Serial.print("x: "); Serial.println(initialPosition.x);
    Serial.print("y: "); Serial.println(initialPosition.y);
    Serial.print("r: "); Serial.println(initialPosition.rotation);

    // Set the crane's position to initialPosition
    crane.move(initialPosition);
    crane.update();

    // Slowly move to the starting position
    crane.move(Position({20, 10, 30}), 40);
    crane.updateUntilMoveDone();
}

void loop() {
    const long startMillis = millis();

    // Toggle rotationMode when the joystick button is pressed
    if (buttonJoystick.isStateChanged()) {
        rotationMode = !rotationMode;
    }

    // Save position / move to saved position when the save button is pressed
    if (buttonSave.isStateChanged()) {
        Serial.print("Save button pressed: ");
        Serial.println(saveMode);
        if (saveMode) {
            savedPosition = Position(crane.position());
        } else {
            crane.move(savedPosition, 20);
            crane.updateUntilMoveDone();
        }
        saveMode = !saveMode;
    }

    // Get x and y input from joystick
    float dx = analogRead(pinJoystickX)/2000.0 + 0.256;
    float dy = analogRead(pinJoystickY)/2000.0 - 0.256;

    if (rotationMode) {
        // Rotate if rotationMode is true and if dx is above a certain threshold
        if (abs(dx) > 0.1) {
            crane.move(crane.position() + Position({0, 0, dx*4}));
        }
    } else if (abs(dx) > 0.1 || abs(dy) > 0.1) {
        // Move if rotationMode is false and both dx and dy are above a certain threshold
        crane.move(crane.position() + Position({dx, dy, 0}));
    }

    // Write to the crane's servos
    crane.update();

    // Save position to EEPROM once every ~3 seconds
    writePosCounter++;
    if (!(writePosCounter &= 63)) {
        Serial.println("Writing to EEPROM");
        Serial.print("x: "); Serial.println(crane.position().x);
        Serial.print("y: "); Serial.println(crane.position().y);
        Serial.print("r: "); Serial.println(crane.position().rotation);
        EEPROM.put(0, crane.position());
    }

    // Delay such that loop() loops every 50ms
    delay(max(0, 50 - min(50, millis() - startMillis)));
}
