#include "button.h"
#include "crane.h"

Crane crane(13.7, 15);

const int pinJoystickX = A1, pinJoystickY = A0;
const int pinJoystickButton = 2;
const int pinSaveButton = 13;
const int pinServoRotation = 9, pinServoShoulder = 10, pinServoElbow = 11;

Button buttonJoystick(pinJoystickButton);
Button buttonSave(pinSaveButton);

void setup() {
    Serial.begin(9600);
//    Serial.println("START");
    crane.attachServos(pinServoRotation, pinServoShoulder, pinServoElbow);

    crane.move(20, 10, 0);
    crane.update();

    buttonJoystick.setPinMode();
    buttonSave.setPinMode();
}

boolean rotationMode = false;

boolean joystickButtonPressed = false;

float savedPosX = 20, savedPosY = 10, savedRotation;
boolean saveMode = true;

void loop() {
    const long startMillis = millis();
    float dx = -analogRead(pinJoystickX)/2000.0 + 0.256;
    float dy = analogRead(pinJoystickY)/2000.0 - 0.256;

//    Serial.println("=======");
//    Serial.println(dx);
//    Serial.println(dy);
//    Serial.println(crane.posX());
//    Serial.println(crane.posY());
//    Serial.println("========");
//    Serial.println(dx);

    if (buttonJoystick.isStateChanged()) {
        rotationMode = !rotationMode;
    }

    if (buttonSave.isStateChanged()) {
        Serial.println(saveMode);
        if (saveMode) {
           savedPosX = crane.posX();
           savedPosY = crane.posY();
           savedRotation = crane.rotation();
        } else {
            crane.move(savedPosX, savedPosY, savedRotation, 20);
            crane.updateUntilMoveDone();
        }
        saveMode = !saveMode;
    }

    if (rotationMode) {
        if (abs(dx) > 0.1) {
            crane.move(crane.posX(), crane.posY(), crane.rotation() - dx*4);
        }
    } else {
        if (abs(dx) > 0.1 || abs(dy) > 0.1) {
            crane.move(crane.posX() - dx, crane.posY() + dy, crane.rotation());
        }
    }

    crane.update();

    // Delay such that loop() loops every 50ms
    delay(max(0L, 50L - min(50, millis() - startMillis)));
}
