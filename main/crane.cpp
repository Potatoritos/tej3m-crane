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

    // If there are no real solutions, the move is impossible
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
    
    moveDuration = t;
    return true;
}

boolean Crane::rotate(float angle) {
    servoRotation.write((int)angle);
}

void Crane::attachServos(int pin1, int pin2) {
    // Set the pin of each servo
    servoShoulder.attach(pin1);
    servoElbow.attach(pin2);
}

void Crane::update() {
    // Write to the servos
    servoShoulder.write((int)targetAngle1);
    servoElbow.write((int)targetAngle2);
}
