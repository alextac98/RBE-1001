#include "Intake.h"
#include "Pinout.h"
//-----Intake Setup----------------------------------
Intake::Intake():
        intakeMotor(INTAKE, isIntakeReversed),
        intakeLift(INTAKELIFT, isIntakeLiftReversed)
{
  isOn = false;
}

void Intake::intakeSetup(){
  intakeMotor.motorSetup();
  intakeLift.motorSetup();
}
//-----Intake Lift-----------------------------------
void Intake::liftUp(){
  lift(downSpeed);
}

void Intake::liftDown(){
  lift(upSpeed);
}

void Intake::liftOff(){
  lift(0);
}

void Intake::lift(int power){
  intakeLift.setPower(power);
}
//-----Intake Functions------------------------------
void Intake::intakeIn(){
  intake(inSpeed);
  isOn = true;
}

void Intake::intakeOut(){
  intake(outSpeed);
  isOn = true;
}

void Intake::intakeOff(){
  intake(0);
  isOn = false;
}

void Intake::intake(int power) {
  intakeMotor.setPower(power);
}


