
#include "main.h"
#include "vex.h"
#include <iostream>
#include <math.h>

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Motor1               motor         1
// Motor10              motor         10
// RangeFinderE         sonar         E, F
// BumperC              bumper        C
// LineTrackerA         line          A
// LineTrackerB         line          B
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace std;

double wheelDiameter = 10.16;
double track = 28.4;
int globalRpm = 100;

int main() {
  vexcodeInit();
  cout << "starting..." << endl;
  wallStandOff(20.0, 5.0);
  cout << "end" << endl;
}

// Given a target distance, drive straight for the given distance.
void move(double cm) {
  cout << "moving " << cm << "cm..." << endl;
  double degree = distanceToDegree(cm);
  Motor1.rotateFor(degree, deg, globalRpm, rpm, false);
  Motor10.rotateFor(degree, deg, globalRpm, rpm);
}

// Given a target angle turn the BaseBot a specified angle.
void turnAngle(double angle) {
  cout << "turning " << angle << "degree..." << endl;
  double cm = (track * M_PI * angle) / 360;
  double degree = distanceToDegree(cm);
  Motor1.rotateFor(degree, deg, globalRpm, rpm, false);
  Motor10.rotateFor(-degree, deg, globalRpm, rpm);
}

// Changes Distance in cm to degrees
double distanceToDegree(double cm) {
  return (cm * 360 * 5) / (wheelDiameter * M_PI);
}

// Bumping code
void bump() {
  while (true) {
    cout << BumperC.pressing() << endl;
    if (BumperC.pressing() == 1)
      Motor1.spin(reverse, globalRpm, rpm);
    else
      Motor1.stop();
    vex::task::sleep(100);
  }
}

// make a square with sideLength
void square(double sideLength) {
  cout << "square of " << sideLength << "cm..." << endl;
  for (int i = 0; i < 3; i++) {
    move(sideLength);
    turnAngle(90);
  }
  move(sideLength);
}

// make a six sided star with sideLength
void star(double sideLength) {
  cout << "star of " << sideLength << "cm..." << endl;
  for (int i = 0; i < 4; i++) {
    move(sideLength);
    turnAngle(143);
  }
  move(sideLength);
}

// drives in circle given radius and fraction of circle to drive
void circle(double r, double outerRpm, double fracOfCircle) {
  double innerC = ((r - (.5 * track)) * 2 * M_PI) * fracOfCircle;
  double outerC = ((r + (.5 * track)) * 2 * M_PI) * fracOfCircle;
  double innerD = distanceToDegree(innerC);
  double outerD = distanceToDegree(outerC);
  double innerRpm = innerD / (outerD * (1 / outerRpm));
  cout << "Motor 1:  " << innerD << " degree, " << innerRpm << " rpm" << endl;
  cout << "Motor 10: " << outerD << " degree, " << outerRpm << " rpm" << endl;
  Motor1.rotateFor(innerD, deg, abs(int(innerRpm)), rpm, false);
  Motor10.rotateFor(outerD, deg, abs(int(outerRpm)), rpm);
}

void maze() {
  cout << "starting maze..." << endl;
  circle(45.0, globalRpm, .25);
  circle(-25.0, globalRpm, -.25);
  double degree = distanceToDegree(12);
  Motor1.rotateFor(degree, deg, globalRpm, rpm, false);
  Motor10.rotateFor(degree, deg, globalRpm, rpm);
  turnAngle(45);
  move(40.0);
  degree = distanceToDegree(21);
  Motor1.rotateFor(-degree, deg, globalRpm, rpm);
  Motor10.rotateFor(degree, deg, globalRpm, rpm);
  move(-30.0);
}

// Proportional Control
// turn speed = k*(sens1-sens2)
void lineFollow(double k) {
  double turnSpeed;
  Motor1.spin(fwd);
  Motor10.spin(fwd);
  cout << LineTrackerA.reflectivity(pct) << "   "
       << LineTrackerB.reflectivity(pct) << endl;
  vex::task::sleep(1000);
  while (((LineTrackerA.reflectivity(pct) > 20) ||
          (LineTrackerB.reflectivity(pct) > 20))) {
    turnSpeed = k * (LineTrackerA.reflectivity() - LineTrackerB.reflectivity());
    cout << LineTrackerA.reflectivity(pct) << "   "
         << LineTrackerB.reflectivity(pct) << endl;
    Motor1.setVelocity(-100 - turnSpeed, rpm);
    Motor10.setVelocity(-100 + turnSpeed, rpm);
    vex::task::sleep(30);
  }
  cout << LineTrackerA.reflectivity(pct) << "   "
       << LineTrackerB.reflectivity(pct) << endl;
  turnAngle(90);
}

// Standoff distance from object
void standOff(double cm, double k) {
  Motor1.spin(fwd);
  Motor10.spin(fwd);
  while (true) {
    cout << "distance: " << RangeFinderE.distance(distanceUnits::cm) << endl;
    double diff = RangeFinderE.distance(distanceUnits::cm) - cm;
    double turnSpeed = k * (diff);
    Motor1.setVelocity(-turnSpeed, rpm);
    Motor10.setVelocity(-turnSpeed, rpm);
    vex::task::sleep(30);
  }
}

void wallStandOff(double cm, double k) {
  Motor1.spin(fwd);
  Motor10.spin(fwd);
  while (true) {
    cout << "distance: " << RangeFinderE.distance(distanceUnits::cm) << endl;
    double diff = RangeFinderE.distance(distanceUnits::cm) - cm;
    double turnSpeed = k * (diff);
    Motor1.setVelocity(-100 - turnSpeed, rpm);
    Motor10.setVelocity(-100 + turnSpeed, rpm);
    vex::task::sleep(30);
  }
}

void DetectObject(void) {
  // takes a snapshot and searches for sig_s1
  // you’ll want to use the signature that you defined above
  vision_1.takeSnapshot(sig_s1);
  // print the coordinates of the center of the object
  // printf stands for ’print formatted’ and the %d tells it to print
  // in integer format. The syntax can be found in online tutorials.
  if (vision_1.objectCount > 0) {
printf("x: %d, y %d\n", vision_1.largestObject.centerX,
vision_1.largestObject.centerY));
  }
}

void objectDirection(){

}

void objectDistance(){
  
}