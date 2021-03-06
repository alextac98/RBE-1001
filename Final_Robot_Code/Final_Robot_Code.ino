/* This is the RBE 1001 Template as of
 *
 * 3/28/17
 *
 * This Template
 * is designed to run the autonomous and teleop sections of the final
 * competition. Write and test your autonomous and teleop code on your
 * own and place the code in auto.cpp or teleop.cpp respectively.
 * The functions will be called by the competition framework based on the
 * time and start button. DO NOT change this file, your code will be called
 * by the framework. The framework will pass your code a reference to the DFW
 * object as well as the amount of MS remaining.
 */
#include <DFW.h>
#include "Drivetrain.h"
#include "Intake.h"
#include "Lift.h"
#include <LiquidCrystal.h>

int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.
DFW dfw(ledpindebug, autonomous, teleop, robotShutdown); // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds

Drivetrain drive;
Intake intake;
Lift lift;

LiquidCrystal lcd(40,41,42,43,44,45);
String stillText = "";
String movingText = "";
const int screenWidth = 16;
const int screenHeight = 2;
int ifChanged = 0;
int scrollCursor;

boolean isLiftUp;

void setup() {
	Serial.begin(9600); // Serial output begin. Only needed for debug
	dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"

  drive.DrivetrainSetup();
  intake.intakeSetup();
  lift.liftSetup();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Press Start");
  isLiftUp = false;
}

void loop() {
	dfw.run();
//  if (dfw.getCompetitionState()!= powerup) { //for testing 
//    teleop(0, dfw);
//    //autoDrive(2910, 0);
//    //autoRedLeft();
//  }
}
//-----Autonomous Code--------------------------------------------------------------------------------
void autonomous(long time,DFW &dfw) { // function definition
  Serial.print("\r\nAuto time remaining: ");
  Serial.print(time);
  int timePrintedT = time/1000;
  if (timePrintedT !=ifChanged){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Autonomous");
    lcd.setCursor(6, 1);
    lcd.print(timePrintedT);
    ifChanged = timePrintedT;
  }

  autoBackup();
  
}

void autoBackup(){
  intake.intakeIn();
  drive.setDistance(350);
}
//-----Teleop Code------------------------------------------------------------------------------------
void teleop( long time,DFW &dfw) { // function definition
  Serial.print("\r\nTeleop time remaining: ");
  Serial.println(time);
  int timePrintedT = time/1000;
  if (timePrintedT !=ifChanged){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Tele-Op");
    lcd.setCursor(6, 1);
    lcd.print(timePrintedT);
    ifChanged = timePrintedT;
  }
  arcadeDrive();
  teleLift();
  teleIntake();
}

void teleLift(){
  if (dfw.l1()){
    lift.up();
    //Serial.println("
  } else if (dfw.l2()){
    lift.hold();
  } else {
    lift.down();
  }
}

void teleIntake(){
  if (dfw.r2()){
    intake.liftUp();
  } else if (dfw.r1()){
    intake.liftDown();
  } else {
    intake.liftOff();
  }

  if (dfw.three()) {
    intake.intakeIn();
  } else if (dfw.one()){
    intake.intakeOut();
  } else if (dfw.two()){
    intake.intakeOff();
  }
}

void arcadeDrive(){
  int vel = map(dfw.joysticklv(), 0, 179, -100, 100);  //max speed
  int dir = map(dfw.joystickrh(), 0, 179, -100, 100);  //direction
  drive.setPower(vel - dir, vel + dir);
}

void tankDrive(){
  drive.setPower(map(dfw.joysticklv(), 0, 179, -100, 100), map(dfw.joystickrv(), 0, 179, -100, 100));
}
//-----Shared Code------------------------------------------------------------------------------------
//-----Shutdown Code----------------------------------------------------------------------------------
void robotShutdown(){
	
}

