#include "crane.h"

boolean Crane::moveTo(float x, float y, int t /*= 1*/) {
    // Moves the crane to the coordinates (x, y)
    // Returns true if moving is possible; false otherwise

    // Let C1 be the circle with length length1 centered at the origin
    // Let C2 be the circle with length length2 centered at (x, y)
    // Finding the point of intersection of C1 and C2:
    const float l1 = length1*length1, l2 = length2*length2;
    const float v = l2 - l1 - x*x - y*y;

    const float a = 1 + (x*x)/(y*y);
    const float b = (x*v)/(y*y);
    const float c = (v*v)/(4*y*y) - l1;

    const float discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return false;
    }

    const float xPOI = (-b - sqrt(discriminant)) / (2 * a);
    const float yPOI = sqrt(l1 - xPOI*xPOI);

    // The angle servo1 should be at such that servo2 is at the point of intersection
    targetAngle1 = acos(xPOI / length1);

    // The angle servo2 should be at such that the end of the crane is at (x, y)
    targetAngle2 = atan2(y - yPOI, x - xPOI) - targetAngle1;

    moveDuration = t;
    return true;
}

void Crane::attachServos(int pin1, int pin2) {
//    servo1.attach(pin1);
//    servo2.attach(pin2);
}

void Crane::update() {
//    servo1.write(targetAngle1);
//    servo2.write(targetAngle2);
}
