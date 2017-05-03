#ifndef INTAKE_H
#define INTAKE_H

#include "Motor.h"

class Intake{
  public:
//-----Setup Functions------------------------------------
    Intake();
    void intakeSetup();
//-----Intake Lift----------------------------------------    
    void liftUp();
    void liftDown();
    void liftOff();
    void lift(int);
//-----Intake Functions-----------------------------------
    void intakeIn();
    void intakeOut();
    void intakeOff();
    void intake(int); //-100 to 100
  private:
    Motor intakeMotor;
    Motor intakeLift;

    boolean isOn;
    
    const int inSpeed = 100;
    const int outSpeed = -100;

    const int upSpeed = 50; //to prevent chain snap
    const int downSpeed = -30; //to prevent chain snap
};


#endif
