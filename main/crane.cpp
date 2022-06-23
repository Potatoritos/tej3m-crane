#include "crane.h"

// Boilerplate code for overloading operators +=, +, -=, -, /=, /
Position& Position::operator+=(const Position& other) {
    x += other.x;
    y += other.y;
    rotation += other.rotation;
    return *this;
}

const Position Position::operator+(const Position& other) const {
    return Position(*this) += other;
}

Position& Position::operator-=(const Position& other) {
    x -= other.x;
    y -= other.y;
    rotation -= other.rotation;
    return *this;
}

const Position Position::operator-(const Position& other) const {
    return Position(*this) -= other;
}

Position& Position::operator/=(int other) {
    x /= other;
    y /= other;
    rotation /= other;
    return *this;
}

const Position Position::operator/(int other) const {
    return Position(*this) /= other;
}


void Crane::move(Position newPos, int durationTicks /* = 1 */) {
    moveRemainingSteps = durationTicks;

    // Set dPos such that pos + dPos * durationTicks = newPos
    dPos = (newPos - pos) / durationTicks;
}

void Crane::setPositionAlongPlane(float x, float y) {
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

    // Update position
    pos.x = x;
    pos.y = y;
}

void Crane::setRotation(float degrees) {
    // Restrict rotation to the interval [0, 180]
    if (0 <= degrees && degrees <= 180) {
        targetRotation = degrees;
        pos.rotation = degrees;
    }
}

void Crane::attachServos(int pinRotation, int pinShoulder, int pinElbow) {
    // Attach each servo
    servoRotation.attach(pinRotation);
    servoShoulder.attach(pinShoulder);
    servoElbow.attach(pinElbow);
}

void Crane::update() {
    // Only write to servos if there are still remaining steps
    if (moveRemainingSteps) {
        moveRemainingSteps--;

        // Add dPos to pos
        setPositionAlongPlane(pos.x + dPos.x, pos.y + dPos.y);
        setRotation(pos.rotation + dPos.rotation);

        // Write to the servos
        servoShoulder.write(targetAngleShoulder);
        servoElbow.write(targetAngleElbow);
        servoRotation.write(targetRotation);
    }
}

void Crane::updateUntilMoveDone() {
    // Keep calling update() until there are no move steps left
    while (moveRemainingSteps) {
        const long startMillis = millis();
        update();

        // Delay 50ms
        delay(max(0, 50 - min(50, millis() - startMillis)));
    }
}

const Position& Crane::position() const {
    return pos;
}
