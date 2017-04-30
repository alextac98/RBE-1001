#include "Drivetrain.h"
#include "Pinout.h"

Drivetrain::Drivetrain():
  leftDrive(DRIVELEFT, isDriveLeftReversed),
  rightDrive(DRIVERIGHT, isDriveRightReversed),
  leftEncoder(ENCODERLEFT1, ENCODERLEFT2),
  rightEncoder(ENCODERRIGHT1, ENCODERRIGHT2)
  {
    //drive init code below
    leftPID = new PID(&leftInputRPM, &leftOutputRPM, &leftSetpointRPM, P_leftRPM, I_leftRPM, D_leftRPM, DIRECT);
    leftPID -> SetMode(AUTOMATIC); //PID disabled by default
    leftPID -> SetSampleTime(PID_UPDATE_TIME);
    leftPID -> SetOutputLimits(-100,100);
    
    rightPID = new PID(&rightInputRPM, &rightOutputRPM, &rightSetpointRPM, P_rightRPM, I_rightRPM, D_rightRPM, DIRECT);
    rightPID -> SetMode(AUTOMATIC);  //PID disabled by default
    rightPID -> SetSampleTime(PID_UPDATE_TIME);
    rightPID -> SetOutputLimits(-100,100);

    leftDistancePID = new PID(&leftInputDIST, &leftOutputDIST, &leftSetpointDIST, P_leftDIST, I_leftDIST, D_leftDIST, DIRECT);
    leftDistancePID -> SetMode(AUTOMATIC); //PID disabled by default
    leftDistancePID -> SetSampleTime(PID_UPDATE_TIME);
    leftDistancePID -> SetOutputLimits(-100,100);
    
    rightDistancePID = new PID(&rightInputDIST, &rightOutputDIST, &rightSetpointDIST, P_rightDIST, I_rightDIST, D_rightDIST, DIRECT);
    rightDistancePID -> SetMode(AUTOMATIC); //PID disabled by default
    rightDistancePID -> SetSampleTime(PID_UPDATE_TIME);
    rightDistancePID -> SetOutputLimits(-100,100);
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----Drive Methods------------------------------------------------------------------------------------------------------------------
void Drivetrain::DrivetrainSetup() {//handles all setup calls
  leftDrive.motorSetup();
  rightDrive.motorSetup();
}

void Drivetrain::setPower(int left, int right) { //from -100 to 100
  leftDrive.setPower(left);
  rightDrive.setPower(right);
}

void Drivetrain::turn(float i) {//in degrees, negative is clockwise
  //write if you have time
}
//-----Encoder Methods----------------------------------------------------------------------------------------------------------------
void Drivetrain::zeroEncoders() {
  leftEncoder.write(0);
  rightEncoder.write(0);
}

float Drivetrain::getLeftDistance() { //in inch
  return (getLeftTicks() * DIST_PER_TICK * LEFT_WHEEL_SLIP_CONSTANT);
}

float Drivetrain::getRightDistance() { //in inch
  return (getRightTicks() * DIST_PER_TICK * RIGHT_WHEEL_SLIP_CONSTANT);
}

float Drivetrain::getAvgDistance(){ //inch
  return ((getLeftDistance() + getRightDistance()) / 2.0);
}

long Drivetrain::getLeftTicks(){
  return leftEncoder.read(); //*-1 if backwards
}

long Drivetrain::getRightTicks(){
  return rightEncoder.read(); //*-1 if backwards
}

void Drivetrain::updateALL(){
  if((millis() - lastRPMUpdate) > PID_UPDATE_TIME)
  {
    RPMUpdate();
  }
  leftInputDIST = getLeftDistance();
  rightInputDIST = getRightDistance();  
  //Update PID
  if((leftPID->GetMode()==AUTOMATIC)) //both will be deactivated or activated together
  {
    rightPID->Compute();
    leftPID->Compute();
    setPower(leftOutputRPM,rightOutputRPM);
  }
  else if(leftDistancePID->GetMode() == AUTOMATIC)
  {
    rightDistancePID->Compute(); //compute PID
    leftDistancePID->Compute(); //compute PID
    setPower(leftOutputDIST,rightOutputDIST);
  }
}

void Drivetrain::RPMUpdate() { //calculates RPM
  float deltaT = (millis() - lastRPMUpdate)/1000.0;
  int deltaR = getRightTicks() - lastRightRPMCount;
  int deltaL = getLeftTicks() - lastLeftRPMCount;
  leftRPM = (deltaL/ENCODER_COUNT_PER_REV)/(deltaT/60.0);
  rightRPM = ((deltaR/ENCODER_COUNT_PER_REV)/(deltaT/60.0));
  lastRPMUpdate = millis();
  lastRightRPMCount = getRightTicks();
  lastLeftRPMCount = getLeftTicks();
}

float Drivetrain::getLeftRPM() {
  return leftRPM;
}

float Drivetrain::getRightRPM() {
  return rightRPM;
}

void Drivetrain::setLeftRPM(float targetRPM) {
  leftSetpointRPM = targetRPM;
  leftPID->SetMode(AUTOMATIC); //Enable PID
}

void Drivetrain::setRightRPM(float targetRPM) {
  rightOutputRPM = targetRPM;
  rightPID -> SetMode(AUTOMATIC); //Enable PID
}

void Drivetrain::setDistance(int newDist) { //inch
  //set targets
  leftSetpointDIST = newDist;
  rightSetpointDIST = newDist;
  //Enable PID
  enableDISTPID();
}
void Drivetrain::setDistance(int left,int right) { //inch
  // set target
  leftSetpointDIST = left;
  rightSetpointDIST = right;
  //Enable PID
  enableDISTPID();
}

boolean Drivetrain::atDistance() { //Both at distance
  return (rightAtDistance() && leftAtDistance());
}

boolean Drivetrain::rightAtDistance() {
  return (abs(getRightDistance() - rightSetpointDIST) < DIST_TOLERANCE);
}

boolean Drivetrain::leftAtDistance() {
  return (abs(getLeftDistance() - leftSetpointDIST) < DIST_TOLERANCE);
}

//-----Private Methods----------------------------------------------------------------------------------------------------------------
void Drivetrain::disablePID(){
  disableRPMPID();
  disableDISTPID();
}

void Drivetrain::disableRPMPID(){
  leftPID -> SetMode(MANUAL);
  rightPID -> SetMode(MANUAL);
}

void Drivetrain::disableDISTPID(){
  leftDistancePID -> SetMode(MANUAL);
  rightDistancePID -> SetMode(MANUAL);
}

void Drivetrain::enablePID(){
  enableRPMPID();
  enableDISTPID();
}

void Drivetrain::enableRPMPID(){
  leftPID -> SetMode(AUTOMATIC);
  rightPID -> SetMode(AUTOMATIC);
}

void Drivetrain::enableDISTPID(){
  leftDistancePID -> SetMode(AUTOMATIC);
  rightDistancePID -> SetMode(AUTOMATIC);
}
