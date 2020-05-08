/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Tue Oct 01 2019                                           */
/*    Description:  Detecting Distances                                       */
/*                  Use the Range Finder to steer a robot                     */
/*                                                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// BumperA              bumper        A               
// BumperB              bumper        B               
// leftwheel            motor         1               
// rightwheel           motor         10              
// RangeFinderL         sonar         E, F            
// RangeFinderR         sonar         G, H            
// BumperC              bumper        C               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

bool bFlag = true;
bool bSwTriggered = false;  // rear bumper switches
bool bResumeRun = true;

void showInfo();
void readBumpersThread();
void readStopBtnThread();
void readRangerFinderThread();
void speedRampTimerThread();
void drvmotionThread();
void autorunThread();
void runOnEvent_StopBtn();  // callback
void resetParams();

int accummulatedRotations = 0;
double currentDistance = 3210;
double speed = 50;

double kP = 0.8;
double previous_distance = 0;


void motorInit()
{
  leftwheel.setStopping(brake);
  rightwheel.setStopping(brake);  
  leftwheel.setVelocity(speed, rpm);
  rightwheel.setVelocity(speed, rpm);

}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Wait 1 second before driving forward.
  wait(1, seconds);

  // Register event with a callback function.
  BumperC.released(runOnEvent_StopBtn);

  motorInit();
  
  thread thrd1 = thread(speedRampTimerThread);
  thread thrd2 = thread(readRangerFinderThread);
  thread thrd3 = thread(drvmotionThread);
  thread thrd4 = thread(autorunThread);

  leftwheel.spin(forward);
  rightwheel.spin(forward);
  wait(200, msec);  // start at low speed to reduce jerking

  while (1) { 

    showInfo();
    bFlag = !bFlag;  // just for L or R turn
    readBumpersThread();
    vex::this_thread::sleep_for(10);
  }

}

void showInfo()
{
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print("e:%d\n", (int)currentDistance);
    Brain.Screen.setCursor(1, 10);   
    Brain.Screen.print("S:%d\n", (int)speed);
    Brain.Screen.setCursor(1, 20); 
    Brain.Screen.print("r:%d\n", accummulatedRotations);
    vex::this_thread::sleep_for(200);
    Brain.Screen.clearScreen();
    vex::this_thread::sleep_for(5);
}

void autorunThread()
{
  while (1) {
      vex::this_thread::sleep_for(100);

      accummulatedRotations += 1;
      if (accummulatedRotations > 50)
        currentDistance = 199;  // ran for too long or stuck. back up  
      
      if (currentDistance < 220)
      {
        rightwheel.stop();
        leftwheel.stop();
        leftwheel.setVelocity(50, rpm);
        rightwheel.setVelocity(50, rpm);
        leftwheel.spinFor(reverse, 1, turns,false);
        rightwheel.spinFor(reverse, 1, turns); 

        if (true == bFlag)
          leftwheel.spinFor(reverse, 700, degrees);
        else 
          rightwheel.spinFor(reverse, 700, degrees);

        accummulatedRotations = 1;
        leftwheel.spin(forward);
        rightwheel.spin(forward);      
      }
      // reverse-turing done, reset to default speed and direction
  }
}

void speedRampTimerThread()
{
  while(1) {
    vex::this_thread::sleep_for(50);
  }
}

void readRangerFinderThread()
{ // this sonar sensor detects up to about 3210 mm
    double d1 = 3000;
    double d2 = 3000;
    bool toggle = true;

  while(1) {
    if (toggle) {
      d1 = RangeFinderL.distance(vex::distanceUnits::mm);
    }      
    else {
      d2 = RangeFinderR.distance(vex::distanceUnits::mm);
    }
    toggle = !toggle;

    if (currentDistance > d1)
      currentDistance = d1;
    else
      currentDistance = d2;
    // limit it so the speed dervided from it won't be too high
    if(currentDistance > 1000)
      currentDistance = 1000;   

    vex::this_thread::sleep_for(20);
  } 
}

void drvmotionThread()
{ 
  while (1) {
    speed = kP*(currentDistance / 10); // P-control: set potential
    vex::this_thread::sleep_for(30);
  }
}

void readBumpersThread()
{
  bSwTriggered = (BumperA.pressing() || BumperA.pressing());
  vex::this_thread::sleep_for(100);
}

void runOnEvent_StopBtn()
{
  if (bResumeRun) {
    rightwheel.stop();
    leftwheel.stop();
    bResumeRun = false;
  }
  else {
    resetParams();

    bResumeRun = true;
    leftwheel.spin(forward);
    rightwheel.spin(forward);    
  }
}

void resetParams()
{
  speed = 50;
  accummulatedRotations = 0;
  previous_distance = 0;
  //currentDistance = 3210;
}