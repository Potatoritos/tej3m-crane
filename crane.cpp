#include <bits/extc++.h>


const int scaleFactor = 6;

constexpr long scale(long n) {
    return n << scaleFactor;
}
constexpr long unscale(long n) {
    return n >> scaleFactor;
}

class Crane {
public:
    const float length1;
    const float length2;

    float targetAngle1;
    float targetAngle2;

    int moveDuration;

    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_),
        targetAngle1(0),
        targetAngle2(0)
    {}
    bool moveTo(float, float, int);
    void update();
};

bool Crane::moveTo(float x, float y, int t = 1) {
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

    const float xPOI = (-b - std::sqrt(discriminant)) / (2 * a);
    const float yPOI = std::sqrt(l1 - xPOI*xPOI);

    // The angle servo1 should be at such that servo2 is at the point of intersection
    targetAngle1 = std::acos(xPOI / length1);

    // The angle servo2 should be at such that the end of the crane is at (x, y)
    targetAngle2 = std::atan((y - yPOI) / (x - xPOI)) - targetAngle1;

    moveDuration = t;

    return true;
}

Crane crane(17, 17);

const int joystickX = 1, joystickY = 2;

int main() {
    debug(crane.targetAngle1, crane.targetAngle2);

    crane.moveTo(20, 10);
    debug(crane.targetAngle1, crane.targetAngle2);
}
