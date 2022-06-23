#ifndef CRANE_H
#define CRANE_H

#include <Arduino.h>
#include <Servo.h>

struct Position {
    float x, y, rotation;

    // Overload basic arithmetic operations
    Position& operator+=(const Position& other);
    const Position operator+(const Position& other) const;
    Position& operator-=(const Position& other);
    const Position operator-(const Position& other) const;
    Position& operator/=(int other);
    const Position operator/(int other) const;
};


class Crane {
public:
    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_)
    {}

    // Starts moving the crane to a position
    void move(Position newPos, int durationTicks = 1);

    // Sets the position of the crane to (x, y)
    // Returns without setting position if the position is invalid
    void setPositionAlongPlane(float x, float y);
    
    // Sets the rotation of the crane
    // Does not set rotation if the angle is not valid
    void setRotation(float degrees);

    // Attach all servos to their respective pins
    void attachServos(int, int, int);

    // Writes to the crane's servos
    void update();

    // Repeatedly call update() until a move is complete
    void updateUntilMoveDone();

    // Getter method for the crane's position
    const Position& position() const;

private:
    // Length of the edge between the shoulder and elbow
    const float length1;

    // Length of the edge between the elbow and wrist
    const float length2;

    // The crane's current position
    Position pos;
    
    // The value to add to pos every update so that the crane reaches
    // its target destination after an amount of ticks
    Position dPos;

    // The amount of ticks remaining for the current move
    int moveRemainingSteps;

    // The values to write to the servos
    float targetAngleShoulder;
    float targetAngleElbow;
    float targetRotation;

    Servo servoShoulder;
    Servo servoElbow;
    Servo servoRotation;
};

#endif
