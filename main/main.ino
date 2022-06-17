#include "button.h"
#include "crane.h"

//const int scaleFactor = 6;
//constexpr int operator"" _s(int n) {
    //return n << scaleFactor;
//}
//constexpr long operator"" _s(long n) {
    //return n << scaleFactor
//}

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

    crane.moveTo(20, 10);
    crane.update();

    buttonJoystick.setPinMode();
    buttonSave.setPinMode();
}

boolean rotationMode = false;

boolean joystickButtonPressed = false;

float savedPosX = 20, savedPosY = 10, savedRotation;
boolean saveMode = true;

void loop() {
    long startMillis = millis();
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
            crane.moveTo(savedPosX, savedPosY);
            crane.rotate(savedRotation);
            crane.update();
        }
        saveMode = !saveMode;
    }

    if (rotationMode) {
        if (abs(dx) > 0.1 && crane.rotate(crane.rotation() - dx*4)) {
            crane.update();
        }
    } else {
        if ((abs(dx) > 0.1 || abs(dy) > 0.1) && crane.moveTo(crane.posX()-dx, crane.posY()+dy)) {
            crane.update();
        }  
    }
    // Delay such that loop() loops every 50ms
    delay(max(0, 50 - (millis() - startMillis)));

//    for (int i = 20; i < 80; i++) {
//        crane.moveTo(i/4.0, 5);
//        crane.update();
//        delay(25);
//    }
//
//    for (int i = 20; i < 80; i++) {
//        crane.moveTo(20, i/4.0);
//        crane.update();
//        delay(25);
//    }
//    
//    for (int i = 80; i > 20; i--) {
//        crane.moveTo(i/4.0, 20);
//        crane.update();
//        delay(25);
//    }
//
//    for (int i = 80; i > 20; i--) {
//        crane.moveTo(5, i/4.0);
//        crane.update();
//        delay(25);
//    }
}
