#ifndef CRANE_H
#define CRANE_H

#include <Arduino.h>
#include <Servo.h>

class Crane {
public:
    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_),
        targetAngle1(30),
        targetAngle2(30),
        targetRotation(0)
    {}

    boolean moveTo(float, float, int = 1);
    boolean rotate(float);
    void attachServos(int, int, int);
    void update();

private:
    // Length of the edge between the shoulder and elbow
    const float length1;

    // Length of the edge between the elbow and wrist
    const float length2;

    float targetAngleShoulder;
    float targetAngleElbow;
    float targetRotation;

    int moveDuration;

    Servo servoShoulder;
    Servo servoElbow;
    Servo servoRotation;
};

#endif
