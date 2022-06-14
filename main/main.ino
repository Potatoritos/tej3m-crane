#include "crane.h"

//const int scaleFactor = 6;
//constexpr int operator"" _s(int n) {
    //return n << scaleFactor;
//}
//constexpr long operator"" _s(long n) {
    //return n << scaleFactor
//}

Crane crane(17, 17);

const int pinJoystickX = A1, pinJoystickY = A2;
const int pinServo1 = 3, pinServo2 = 4;

void setup() {
//    crane.attachServos(pinServo1, pinServo2);
//    crane.moveTo(20, 10);
    Serial.begin(9600);
    long start = millis();
    for (int i = 0; i < 10000; i++) {
      crane.moveTo(20, 10);
    }
    Serial.println(millis() - start);
}

void loop() {
//    float x = analogRead(pinJoystickX)/1023.0+1;
//    float y = analogRead(pinJoystickY)/1023.0+1;
//    crane.moveTo(x, y);
//    crane.update();
//    Serial.println(crane.targetAngle1);
    delay(20);
}
