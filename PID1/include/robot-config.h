using namespace vex;

extern brain Brain;

// VEXcode devices
extern bumper BumperA;
extern bumper BumperB;
extern motor leftwheel;
extern motor rightwheel;
extern sonar RangeFinderL;
extern sonar RangeFinderR;
extern bumper BumperC;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );