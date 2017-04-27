#include "Motor.h"

Motor::Motor(int pinIn, boolean reverse):motor() {
  pin = pinIn;
  isReverse = reverse;
}

void Motor::setup() {
  motor.attach(pin, 1000, 2000);
  setPower(0);
}

void Motor::setPower(int power) {
  if (abs(power) < 20) {        //used to prevent stall at low set speed
    power = 0;
  }

  if ((millis() - lastSetTime) > 20) {
    if (isReverse == true) {
      power*=-1;
    }
    motor.write(constrain(map(power, -100, 100, 0, 180), 0, 180));
    lastSetTime = millis();
  }
}

