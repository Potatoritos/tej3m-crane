#include "crane.h"

boolean Crane::moveTo(float x, float y, int t /*= 1*/) {
    // Moves the crane to the coordinates (x, y)
    // Returns true if moving is possible; false otherwise

    // Restrict movement to 1st quadrant
    if (x <= 0 || y <= 0) {
        return false;
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

    // If there is no point of intersection
    if (discriminant < 0) {
        return false;
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
    
    moveRemainingSteps = 8 * t;
    moveDX = x - posX_;
    moveDY = y - posY_;

    
    posX_ = x;
    posY_ = y;
    return true;
}

boolean Crane::rotate(float angle) {
    if (0 <= angle && angle <= 180) {
        targetRotation = angle;
        rotation_ = angle;
        return true;
    }
    return false;
}

void Crane::attachServos(int pinRotation, int pinShoulder, int pinElbow) {
    // Set the pin of each servo
    servoRotation.attach(pinRotation);
    servoShoulder.attach(pinShoulder);
    servoElbow.attach(pinElbow);
}



void Crane::update() {
    // Write to the servos
    servoShoulder.write(targetAngleShoulder);
    servoElbow.write(targetAngleElbow);
    servoRotation.write(targetRotation);
}

void Crane::updateUntilMoveDone() {
    while (moveRemainingSteps) {
        update();
    }
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
