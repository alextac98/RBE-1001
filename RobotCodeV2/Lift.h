#ifndef LIFT_H
#define LIFT_H
#include <PID_v1.h>

#include "Motor.h"

class Lift{
  public:
    Lift();
    void liftSetup();
    
    void setLift(int);
    void up();
    void down();
    void hold();
    void stopAll();
    void calibrate();
    int readPot();
  private:
    Motor liftLeft;
    Motor liftRight;
    
    static const int UP_POWER = 100;
    static const int DOWN_POWER = 0;
    
    PID* liftPID;
    double liftSetpoint;
    double liftInput;
    double liftOutput;
    const double P_lift = 2.1;
    const double I_lift = 0;
    const double D_lift = 0;
    
    const int PID_UPDATE_HZ = 200; //in hz
    const int PID_UPDATE_TIME = 1000/PID_UPDATE_HZ; //in seconds

    int liftDown;
    int liftUp;
    const int liftOffset = 400;

    
};

#endif

