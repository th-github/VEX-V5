#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
bumper BumperA = bumper(Brain.ThreeWirePort.A);
bumper BumperB = bumper(Brain.ThreeWirePort.B);
motor leftwheel = motor(PORT1, ratio18_1, false);
motor rightwheel = motor(PORT10, ratio18_1, true);
sonar RangeFinderL = sonar(Brain.ThreeWirePort.E);
sonar RangeFinderR = sonar(Brain.ThreeWirePort.G);
bumper BumperC = bumper(Brain.ThreeWirePort.C);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}