#ifndef CRANE_H
#define CRANE_H

#include <Arduino.h>
#include <Servo.h>

class Crane {
public:
    // Length of the edge between the shoulder and elbow
    const float length1;

    // Length of the edge between the elbow and wrist
    const float length2;

    float targetAngleShoulder;
    float targetAngleElbow;

    int moveDuration;

    Servo servoShoulder;
    Servo servoElbow;
    Servo servoRotation;

    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_),
        targetAngle1(30),
        targetAngle2(30) {}
    boolean moveTo(float, float, int = 1);
    boolean rotate(float);
    void attachServos(int, int);
    void update();
};

#endif
