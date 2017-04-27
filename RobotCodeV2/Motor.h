#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <Servo.h>
#include <PID_v1.h>
#include <DFW.h>

class Motor {
  public:
    Motor(int pinIn, boolean reverse = false);
    void setup();
    void setPower(int);

  private:
    long lastSetTime;
    boolean isReverse;
    int pin;
    Servo motor;
};
#endif
