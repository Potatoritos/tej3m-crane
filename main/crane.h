#ifndef CRANE_H
#define CRANE_H

#include <Arduino.h>
#include <Servo.h>

class Crane {
public:
    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_),
        targetAngleShoulder(0),
        targetAngleElbow(0),
        targetRotation(0),
        posX_(20),
        posY_(10),
        rotation_(0),
        moveRemainingSteps(1)
    {}

    boolean moveTo(float, float, int = 0);
    boolean rotate(float);
    void attachServos(int, int, int);
    void update();
    void updateUntilMoveDone();

    float posX();
    float posY();
    float rotation();

private:
    // Length of the edge between the shoulder and elbow
    const float length1;

    // Length of the edge between the elbow and wrist
    const float length2;

    float posX_;
    float posY_;
    float rotation_;

    float moveDX;
    float moveDY;
    int moveRemainingSteps;

    float targetAngleShoulder;
    float targetAngleElbow;
    float targetRotation;

    Servo servoShoulder;
    Servo servoElbow;
    Servo servoRotation;
};

#endif
