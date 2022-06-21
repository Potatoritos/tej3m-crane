#include "button.h"
#include "crane.h"

#include <EEPROM.h>

Crane crane(13.7, 15);

const int pinJoystickX = A1, pinJoystickY = A0;
const int pinJoystickButton = 2;
const int pinSaveButton = 13;
const int pinServoRotation = 9, pinServoShoulder = 10, pinServoElbow = 11;

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
        Serial.print("Save button pressed: ")
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
            crane.move(crane.pos() + Position({0, 0, dx*4}));
        }
    } else if (abs(dx) > 0.1 || abs(dy) > 0.1) {
        // Move if rotationMode is false and both dx and dy are above a certain threshold
        crane.move(crane.pos() + Position({dx, dy, 0}));
    }

    // Write to the crane's servos
    crane.update();

    // Save position to EEPROM once every ~3 seconds
    writePosCounter++;
    if (!(writePosCounter &= 63)) {
        EEPROM.put(0, crane.pos());
    }

    // Delay such that loop() loops every 50ms
    delay(max(0, 50 - min(50, millis() - startMillis)));
}
