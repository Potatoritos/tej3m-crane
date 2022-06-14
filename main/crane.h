#ifndef CRANE_H
#define CRANE_H
#include <Arduino.h>
#include <Servo.h>

class Crane {
public:
    const float length1;
    const float length2;

    float targetAngle1;
    float targetAngle2;

    int moveDuration;

    Servo servo1;
    Servo servo2;

    Crane(float length1_, float length2_) :
        length1(length1_),
        length2(length2_),
        targetAngle1(30),
        targetAngle2(30)
    {}
    boolean moveTo(float, float, int = 1);
    void attachServos(int, int);
    void update();
};

#endif
