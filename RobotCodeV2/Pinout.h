#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

//-----Drive Train Constants---------------------------
#define DRIVELEFT 9
const boolean isDriveLeftReversed = true;
#define ENCODERLEFT1 2
#define ENCODERLEFT2 22

#define DRIVERIGHT 6
const boolean isDriveRightReversed = false;
#define ENCODERRIGHT1 3
#define ENCODERRIGHT2 23
//-----Lift Constants----------------------------------
#define LIFTLEFT 8
const boolean isLiftLeftReversed = true;
#define LIFTRIGHT 5
const boolean isLiftRightReversed = false;
#define POT 11
//-----Intake Constants--------------------------------
#define INTAKELIFT 7
const boolean isIntakeLiftReversed = false;
#define INTAKELEFT 4
const boolean isIntakeLeftReversed = false;
#define INTAKERIGHT 10     //not used
const boolean isIntakeRighteReversed = false;
//-----------------------------------------------------
#endif
