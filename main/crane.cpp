#include "crane.h"

void Crane::move(float x, float y, float rotation, int durationTicks /* = 1 */) {
    moveRemainingSteps = durationTicks;
    moveDX = (x - posX_) / durationTicks;
    moveDY = (y - posY_) / durationTicks;
    moveDRotation = (rotation - rotation_) / durationTicks;
}

void Crane::setPositionAlongPlane(float x, float y) {
    // Sets the position of the crane to (x, y) on the plane
    // Returns without moving the crane if the move is not valid

    // Restrict movement to the 1st quadrant
    if (x <= 0 || y <= 0) {
        return;
    }

    // Let C1 be the circle with length length1 centered at the origin
    // Let C2 be the circle with length length2 centered at (x, y)
    // Finding the point of intersection of C1 and C2:
    const float l1 = length1*length1, l2 = length2*length2;
    const float v = l2 - l1 - x*x - y*y;

    const float a = 1 + (x*x) / (y*y);
    const float b = (x*v) / (y*y);
    const float c = (v*v) / (4*y*y) - l1;

    const float discriminant = b*b - 4*a*c;

    // Return if there is no point of intersection
    if (discriminant < 0) {
        return;
    }
    
    const float xPOI = (-b - sqrt(discriminant)) / (2 * a);
    const float yPOI = sqrt(l1 - xPOI * xPOI);
    
    // The angle servo1 should be at such that servo2 reaches (xPOI, yPOI)
    targetAngleShoulder = acos(xPOI / length1) * RAD_TO_DEG;
    
    // The angle servo2 should be at such that the end of the crane reaches (x, y)
    targetAngleElbow = atan2(y - yPOI, x - xPOI) * RAD_TO_DEG;

    // The actual angles to write to the servos
    targetAngleElbow = 180 - (targetAngleShoulder - targetAngleElbow) - 12;
    targetAngleShoulder = targetAngleShoulder - 20;
    
    posX_ = x;
    posY_ = y;
}

void Crane::setRotation(float degrees) {
    // Sets the rotation of the crane to degrees
    // Does not set rotation if degrees is not valid
    if (0 <= degrees && degrees <= 180) {
        targetRotation = degrees;
        rotation_ = degrees;
    }
}

void Crane::attachServos(int pinRotation, int pinShoulder, int pinElbow) {
    // Attach each servo
    servoRotation.attach(pinRotation);
    servoShoulder.attach(pinShoulder);
    servoElbow.attach(pinElbow);
}

void Crane::update() {
    // Write to the servos
    if (moveRemainingSteps) {
        moveRemainingSteps--;

        setPositionAlongPlane(posX_ + moveDX, posY_ + moveDY);
        setRotation(rotation_ + moveDRotation);

        servoShoulder.write(targetAngleShoulder);
        servoElbow.write(targetAngleElbow);
        servoRotation.write(targetRotation);
    }
}

void Crane::updateUntilMoveDone() {
    while (moveRemainingSteps) {
        Serial.println(moveRemainingSteps);
        const long startMillis = millis();
        update();
        // Delay such that loop() loops every 50ms
        Serial.println(max(0, 50 - (millis() - startMillis)));
        delay(max(0, 50 - (millis() - startMillis)));
    }
        Serial.println(moveRemainingSteps);
}

float Crane::posX() {
    return posX_;
}

float Crane::posY() {
    return posY_;
}

float Crane::rotation() {
    return rotation_;
}
