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
const int pinServoRotation = 9, pinServoShoulder = 10, pinServoElbow = 11;

Button buttonJoystick(pinJoystickButton);

void setup() {
    Serial.begin(9600);
//    Serial.println("START");
    crane.attachServos(pinServoRotation, pinServoShoulder, pinServoElbow);

    buttonJoystick.setPinMode();
}

float posX = 5, posY = 5, rotation = 0;

boolean rotationMode = false;

boolean joystickButtonPressed = false;

void loop() {
    long startMillis = millis();
    float dx = -analogRead(pinJoystickX)/2000.0 + 0.256;
    float dy = analogRead(pinJoystickY)/2000.0 - 0.256;
    Serial.println("======");
    Serial.println(dx);
    Serial.println(dy);

    if (buttonJoystick.isStateChanged()) {
        rotationMode = !rotationMode;
    }

    if (rotationMode) {
        if (abs(dx) > 0.1 && crane.rotate(rotation + dx)) {
            rotation += dx;
            crane.update();
        }
    } else {
        if ((abs(dx) > 0.1 || abs(dy) > 0.1) && crane.moveTo(posX+dx, posY+dy)) {
            posX += dx;
            posY += dy;
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
