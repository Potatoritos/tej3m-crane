#ifndef CRANE_H
#define CRANE_H

#include <Arduino.h>
#include <Servo.h>

struct Position {
    float x, y, rotation;

    Position& operator+=(const Position& other);
    const Position operator+(const Position& other) const;
    Position& operator-=(const Position& other);
    const Position operator-(const Position& other) const;
    Position& operator/=(const Position& other);
    const Position operator/(const Position& other) const;
};


class Crane {
public:
    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_)
    {}

    //void move(float x, float y, float rotation, int durationTicks = 1);
    void move(Position newPos, int durationTicks = 1);

    // Sets the position of the crane to (x, y)
    // Returns without setting position if the position is invalid
    void setPositionAlongPlane(float x, float y);
    
    // Sets the rotation of the crane
    // Does not set rotation if the angle is not valid
    void setRotation(float degrees);


    void attachServos(int, int, int);
    void update();
    void updateUntilMoveDone();

    Position position();

private:
    // Length of the edge between the shoulder and elbow
    const float length1;

    // Length of the edge between the elbow and wrist
    const float length2;

    Position pos;
    Position dPos;

    int moveRemainingSteps;

    float targetAngleShoulder;
    float targetAngleElbow;
    float targetRotation;

    Servo servoShoulder;
    Servo servoElbow;
    Servo servoRotation;
};

#endif
