#include "Lift.h"
#include "Pinout.h"

Lift::Lift():
      liftLeft(LIFTLEFT, isLiftLeftReversed),
      liftRight(LIFTRIGHT, isLiftRightReversed)
  {
    liftPID = new PID(&liftInput, &liftOutput, &liftSetpoint, P_lift, I_lift, D_lift, DIRECT);
    liftPID -> SetMode(AUTOMATIC); //PID disabled by default
    liftPID -> SetSampleTime(PID_UPDATE_TIME);
    liftPID -> SetOutputLimits(-100,100);
  }
void Lift::liftSetup(){
  liftLeft.motorSetup();
  liftRight.motorSetup();
  calibrate();
}

void Lift::setLift(int setpoint){
  liftSetpoint = setpoint;
  liftInput = readPot();
  liftPID -> Compute();
  liftLeft.setPower(liftOutput);
  liftRight.setPower(liftOutput);
}

void Lift::up(){
  setLift(liftUp);
}

void Lift::down(){
  //no speed down
}

void Lift::hold(){
  setLift(readPot());
}

void Lift::stopAll(){
  liftLeft.setPower(0);
  liftRight.setPower(0);
}

int readPot(){
  return analogRead(POT);
}

void Lift::calibrate(){
  liftDown = readPot();
  liftUp = liftDown + liftOffset;
}
