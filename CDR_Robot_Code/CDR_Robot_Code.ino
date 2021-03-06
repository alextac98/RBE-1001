#include <Encoder.h>
#include <DFW.h>
#include <Servo.h>
#include <PID_v1.h>
#include <LiquidCrystal.h>

int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.
DFW dfw(ledpindebug, autonomous, teleop, robotShutdown); // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
//DFW dfw(ledpindebug);

#define DRIVELEFT 9
#define DRIVERIGHT 6
Servo rightDrive;
Servo leftDrive;

#define ENCODERRIGHT1 3
#define ENCODERRIGHT2 23
#define ENCODERLEFT1 2
#define ENCODERLEFT2 22
Encoder rightEnc(ENCODERRIGHT1, ENCODERRIGHT2);
Encoder leftEnc(ENCODERLEFT1, ENCODERLEFT2);

double leftSetpoint, leftInput, leftOutput;
const double P_left = .6;
const double I_left = 0;
const double D_left = 0;
PID leftPID(&leftInput, &leftOutput, &leftSetpoint, P_left, I_left, D_left, DIRECT);

double rightSetpoint, rightInput, rightOutput;
const double P_right = .6;
const double I_right = 0;
const double D_right = 0;
PID rightPID(&rightInput, &rightOutput, &rightSetpoint, P_right, I_right, D_right, DIRECT);

typedef enum {
  TOTURN1, TURN1,
  TOTURN2, TURN2,
  PUSH, BACKUP,
  IN,FINISH
} State;
State autoState;
int driveError = 100;

#define LIFTLEFT 8
#define LIFTRIGHT 5
Servo rightLift;
Servo leftLift;
const int maxLiftSpeed = 100;

#define INTAKELIFT 7
#define INTAKE 4
Servo intakeLift;
Servo intake;
const int maxIntakeSpeed = 100;
const int maxOuttakeSpeed = -100;
const int maxIntakeLift = 70;

#define POT 11
double liftSetpoint, liftInput, liftOutput, liftDown, liftUp;
const double P_lift = 2.1;
const double I_lift = 0;
const double D_lift = 0;
const double deadPoint = 5;
PID liftPID(&liftInput, &liftOutput, &liftSetpoint, P_lift, I_lift, D_lift, DIRECT);

LiquidCrystal lcd(40,41,42,43,44,45);
String stillText = "";
String movingText = "";
const int screenWidth = 16;
const int screenHeight = 2;
int ifChanged = 0;
int scrollCursor;

void setup() {
  autoState = TOTURN1;
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  rightDrive.attach(DRIVERIGHT, 1000, 2000);
  leftDrive.attach(DRIVELEFT, 1000, 2000);
  rightDrive.write(90);
  leftDrive.write(90);

  leftPID.SetMode(AUTOMATIC);
  leftPID.SetTunings(P_left, I_left, D_left);
  leftPID.SetSampleTime(1);
  leftPID.SetOutputLimits(-100,100);
   
  rightPID.SetMode(AUTOMATIC);
  rightPID.SetTunings(P_right, I_right, D_right);
  rightPID.SetSampleTime(1);
  rightPID.SetOutputLimits(-100,100);

  rightLift.attach(LIFTRIGHT, 1000, 2000);
  leftLift.attach(LIFTLEFT, 1000, 2000);
  rightLift.write(90);
  leftLift.write(90);

  intakeLift.attach(INTAKELIFT, 1000, 2000);
  intake.attach(INTAKE, 1000, 2000);
  intakeLift.write(90);
  intake.write(90);

  liftPID.SetMode(AUTOMATIC);
  liftPID.SetTunings(P_lift, I_lift, D_lift);
  liftPID.SetSampleTime(1);
  liftPID.SetOutputLimits(-90, 90);

  lcd.begin(screenWidth, screenHeight);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Press Start");
}
void loop() {
  dfw.run();
//  if (dfw.getCompetitionState()!= powerup) { //for testing 
//    teleop(0, dfw);
//    //autoDrive(2910, 0);
//    //autoRedLeft();
//  }
 }

void autonomous(long time, DFW &dfw){
  Serial.print("\r\nAuto time remaining: ");
  Serial.print(time/1000);
  autoRedLeft();
  int timePrinted = (int)time/1000;
  if (timePrinted !=ifChanged){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Autonomous");
    lcd.setCursor(7, 1);
    lcd.print(timePrinted);
    ifChanged = timePrinted;
    
  }
}
void autoRedLeft(){
  switch(autoState) {
    case (TOTURN1):
      autoDrive(6500, 6500);
//      Serial.println("To Turn 1");
//      Serial.println(getLeftEncoder());
//      if (getLeftEncoder() > 4000 - driveError && getLeftEncoder() < 4000 + driveError && getRightEncoder() > 4000-driveError && getRightEncoder() < 4000 + driveError) {
//        //autoState = TURN1;
//        //clearEncoders();
//      }
      break;
    case (TURN1):
      autoDrive(2910,0);
      Serial.println("Turn 1");
      if (getLeftEncoder() > 2910 - driveError && getLeftEncoder() < 2910 + driveError && getRightEncoder() > -driveError && getRightEncoder() < driveError) {
        autoState = TOTURN2;
      }
      break;
    case (TOTURN2):
      Serial.println("To Turn 2");
      autoDrive(2110,2110);
      if (getLeftEncoder() > 2110 - driveError && getLeftEncoder() < 2110 + driveError && getRightEncoder() > 2110 - driveError && getRightEncoder() < 2110 + driveError) {
        autoState = TURN2;
      }
      break;
    case (TURN2):
      Serial.println("Turn 2");
      autoDrive(0,2910);
      if (getLeftEncoder() > - driveError && getLeftEncoder() < driveError && getRightEncoder() > -driveError && getRightEncoder() < driveError) {
        autoState = PUSH;
      }
      break;
    case (PUSH):
      Serial.println("Push");
      autoDrive(2110,2110);
      intake.write(map(maxIntakeSpeed,-100, 100, 0, 180));
      if (getLeftEncoder() > 2110 - driveError && getLeftEncoder() < 2110 + driveError && getRightEncoder() > 2110 - driveError && getRightEncoder() < 2110 + driveError) {
        autoState = BACKUP;
      }
      break;
    case (BACKUP):
      Serial.println("Backup");
      autoDrive(-2110, -2110);
      setLift(liftDown + 50);
      intake.write(map(maxIntakeSpeed,-100, 100, 0, 180));
      if (getLeftEncoder() > -2110 - driveError && getLeftEncoder() < -2110 + driveError && getRightEncoder() > -2110 - driveError && getRightEncoder() < -2110 + driveError) {
        autoState = IN;
      }
      break;
    case (IN):
      Serial.println("In");
      autoDrive(2110,2110);
      setLift(liftDown + 100);
      intake.write(map(maxIntakeSpeed,-100, 100, 0, 180));
      if (getLeftEncoder() > 2110 - driveError && getLeftEncoder() < 2110 + driveError && getRightEncoder() > 2110 - driveError && getRightEncoder() < 2110 + driveError) {
        autoState = FINISH;
      }
      break;
    case (FINISH):
      Serial.println("Finish");
      setLift(liftDown + 100);
      intake.write(map(maxOuttakeSpeed,-100, 100, 0, 180));
      break;
    default:
      break;
  }

}

void autoDrive(int leftTicks, int rightTicks) { //drive forward with PID
  leftSetpoint = leftTicks;
  rightSetpoint = rightTicks;
  leftInput = getLeftEncoder();
  rightInput = getRightEncoder();

  leftPID.Compute();
  rightPID.Compute();

//  if (leftOutput < 25 && leftOutput > -25) {
//    if (leftOutput > 0) {
//      leftOutput = 25;
//    } else {
//      leftOutput = -25;
//    }
//  }
//
//  if (rightOutput < 25 && rightOutput > -25) {
//    if (rightOutput > 0) {
//      rightOutput = 25;
//    } else {
//      rightOutput = -25;
//    }
//  }
  drive(leftOutput, rightOutput);
}

int getLeftEncoder(){
  return leftEnc.read();
}
int getRightEncoder(){
  return rightEnc.read();
}

void clearEncoders(){
  leftEnc.write(0);
  rightEnc.write(0);
}

void teleop (long time, DFW &dfw) {
  Serial.print("\r\nTeleop time remaining: ");
  Serial.print(time/1000);
  arcadeDrive();
  lift();
  inLift();
  in();
  int timePrintedT = time/1000;
  if (timePrintedT !=ifChanged){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Tele-Op");
    lcd.setCursor(6, 1);
    lcd.print(timePrintedT);
    ifChanged = timePrintedT;
  }
}

void inLift() {
  if (dfw.one()){
    Serial.println("lift up");
    intakeLift.write(map(maxIntakeLift, -100, 100, 0, 180));
  } else if (dfw.three()){
    Serial.println("lift down");
    intakeLift.write(map(-maxIntakeLift, -100, 100, 0, 180));
  } else {
    intakeLift.write(90);
  }
}

void in() {
  if (dfw.r2()){
    Serial.println("in");
    intake.write(map(maxIntakeSpeed,-100, 100, 0, 180));
  } else if(dfw.r1()){
    Serial.println("out");
    intake.write(map(maxOuttakeSpeed,-100, 100, 0, 180));
  } else {
    intake.write(90);
  }
}

void calibrateLift(){
  liftDown = map(analogRead(POT), 0, 1023, -100, 100); 
  }

void setLift(int pos){
  liftInput = map(analogRead(POT), 0, 1023, -100, 100);
  liftSetpoint = pos;
  liftPID.Compute();
  liftMotors(liftOutput);
}

void liftMotors(int input) { //from -100 to 100
  rightLift.write(input + 90);
  leftLift.write(180 - (input + 90));
  Serial.println(input);
  Serial.println(liftInput);
}

void lift(){
  
  if (dfw.l1()) {
    Serial.println("up");
    setLift(liftDown + 100);
  } else if (dfw.l2()) {
    Serial.println("down");
    //setLift(liftDown);
  }  else {
    rightLift.write(90);
    leftLift.write(90);
  }
  
}

void arcadeDrive() {
  int vel = map(dfw.joysticklv(), 0, 179, -100, 100);  //max speed
  int dir = map(dfw.joystickrh(), 0, 179, -100, 100);       //direction
  Serial.println(vel);
  drive(vel - dir, vel + dir);
}

void drive(int left, int right){  //from 100 to -100
  int leftSpeed = map(left, -100, 100, 0, 180);
  int rightSpeed = map(right, -100, 100, 0, 180);
  rightDrive.write(rightSpeed);
  leftDrive.write(180-leftSpeed);
}

void robotShutdown(){
  rightDrive.write(90);
  leftDrive.write(90);
  rightLift.write(90);
  leftLift.write(90);
  intake.write(90);
  intakeLift.write(90);
}

