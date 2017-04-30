#ifndef LIFT_H
#define LIFT_H

#include "Motor.h"

class FourBar{
  public:
    Fourbar();
    void fourbarSetup();
    void up();
    void down();
    void stopAll();
  private:
    static const int UP_POWER = 100;
    static const int DOWN_POWER = 0;
    Motor leftLift();
    Motor rightLift();
};

#endif

