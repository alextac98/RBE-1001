#include "Drivetrain.h"
#include "Pinout.h"

Drivetrain::Drivetrain():
  leftDrive(LEFTDRIVE, isLeftDriveReversed),
  rightDrive(RIGHTDRIVE, isRightDriveReversed),
  leftEncoder(ENCODERLEFT1, ENCODERLEFT2),
  rightEncoder(ENCODERRIGHT1, ENCODERRIGHT2)
  {
    
    leftPID = new PID(&left
    
  }

