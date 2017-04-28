#ifndef DRIVETRAIN_H
#define  DRIVETRAIN_H

#include <Encoder.h>
#include <Arduino.h>
#include <PID_v1.h>
#include "Motor.h"

class Drivetrain{
  
//////////////////////////////////////////////////////////

  public:
//-----Basic Drive Methods--------------------------------  
    Drivetrain();
    void DrivetrainSetup();
    void setPower(int, int);
//-----Encoder Methods------------------------------------    
    void zeroEncoders();
    
    float getRightDistance();
    float getLeftDistance();
    float getAvgDistance();
    
    long getLeftTicks();
    long getRightTicks();
    
    void RPMUpdate();
    
    float getLeftRPM();
    float getRightRPM();
    
    void setLeftRPM(float);
    void setRightRPM(float);

    void setDistance(int);
    void setDistance(int, int);

    boolean atDistance();
    boolean rightAtDistance();
    boolean leftAtDistance();
    
//////////////////////////////////////////////////////////

  private:
//-----Drive Motors---------------------------------------
    Motor leftDrive;
    Motor rightDrive;
//-----Drive Encoders-------------------------------------
    Encoder leftEncoder;
    Encoder rightEncoder;
//-----PID Controllers------------------------------------
    PID* leftPID;
    PID* rightPID;
    PID* leftDistancePID;
    PID* rightDistancePID;
//-----PID Variables and Constants------------------------
    //PID for RPM Variables and Constants
    const double P_leftRPM = 0.6;
    const double I_leftRPM = 0;
    const double D_leftRPM = 0;
    double leftInputRPM;
    double leftSetpointRPM;
    double leftOutputRPM;
    
    const double P_rightRPM = 0.6;
    const double I_rightRPM = 0;
    const double D_rightRPM = 0;
    double rightInputRPM;
    double rightSetpointRPM;
    double rightoutputRPM;
    //PID for Distance Variables and Constants
    const double P_leftDIST = 60;
    const double I_leftDIST = 0;
    const double D_leftDIST = 0;
    double leftInputDIST;
    double leftSetpointDIST;
    double leftOutputDIST;
    
    const double P_rightDIST = 60;
    const double I_rightDIST = 0;
    const double D_rightDIST = 0;
    double rightInputDIST;
    double rightSetpointDIST;
    double rightoutputDIST;
    
//-----Drive Train Constants------------------------------
    const float WHEEL_DIA = 3; //in inches
    const float PI_VAL = 3.141592654;
    const float CIRCUMFERENCE = WHEEL_DIA * PI_VAL;
    const float ENCODER_COUNT_PER_REV = 2 * 90;
    const float DIST_PER_TICK =  CIRCUMFERENCE/ENCODER_COUNT_PER_REV;
    const float LEFT_WHEEL_SLIP_CONSTANT = 1; //accounts for wheel slip, 1 means no wheel slip
    const float RIGHT_WHEEL_SLIP_CONSTANT = 1; //accounts for wheel slip, 1 means no wheel slip

    const int RPM_UPDATE_HZ = 200; //in hz
    const int RPM_UPDATE_TIME = 1000/RPM_UPDATE_HZ; //in seconds

    long lastRPMUpdate;
    long lastRightRPMCount;
    long lastLeftRPMCount;

    double leftRPM;
    double rightRPM;
//-----Private Methods------------------------------------
    void setPowerManual(int, int); //left, right from -100 to 100

//////////////////////////////////////////////////////////

};

#endif
