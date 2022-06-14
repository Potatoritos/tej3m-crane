#include <Servo.h>


//const int scaleFactor = 6;
//constexpr int operator"" _s(int n) {
    //return n << scaleFactor;
//}
//constexpr long operator"" _s(long n) {
    //return n << scaleFactor
//}

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
    boolean moveTo(float, float, int);
    void attachServos(int, int);
    void update();
};

boolean Crane::moveTo(float x, float y, int t = 1) {
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


Crane crane(17, 17);

const int pinJoystickX = A1, pinJoystickY = A2;
const int pinServo1 = 3, pinServo2 = 4;

void setup() {
//    crane.attachServos(pinServo1, pinServo2);
//    crane.moveTo(20, 10);
    Serial.begin(9600);
    long start = millis();
    for (int i = 0; i < 10000; i++) {
      crane.moveTo(20, 10);
    }
    Serial.println(millis() - start);
}

void loop() {
//    float x = analogRead(pinJoystickX)/1023.0+1;
//    float y = analogRead(pinJoystickY)/1023.0+1;
//    crane.moveTo(x, y);
//    crane.update();
//    Serial.println(crane.targetAngle1);
    delay(20);
}
