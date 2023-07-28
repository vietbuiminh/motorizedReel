/*
 * Use AccelStepper Library
 * Keep track of a rev
 * Limit on a certain conversion depth
 * Run back to 0
 */
#include <AccelStepper.h>

#define dirPin 2
#define stepPin 3
#define interfaceType 1

AccelStepper motor = AccelStepper(interfaceType, stepPin, dirPin);

int analogPin = A0;
int cwPin = A1;
int ccwPin = A2;
int DOUT = A3;
const int rev = 800;
const int DISTANCE_MOVE = 300;
int count = 0;

bool boo = false;       // for XLink
bool booManual = false; // for manual control 

void setup() {
  Serial.begin(9600);
  motor.setMaxSpeed(500);
  motor.setAcceleration(300);
}

void loop() {
  int cw = analogRead(cwPin);
  int ccw = analogRead(ccwPin);
  int xLink = analogRead(DOUT); 
  int val = analogRead(analogPin);
  
  // XLink send a 1s 5V from the DOUT port to the analog port. 
  if (xLink > 1000) { // >1000 for 5V signal
    boo = true;
  } else {
    boo = false;
  }

  // Manual button engage overwrite the xLink boolean value
  if (val > 1000) {
    boo = false;
    count = 0;
    booManual = true;
  } else {
    booManual = false;
  }
  if (booManual) {
    int nextPosition = motor.currentPosition();
    motor.setAcceleration(100000000);
    if (cw > 700) { // >700 because max voltage is 3V
      motor.moveTo(nextPosition + DISTANCE_MOVE);
      motor.runToPosition();
    }
    if (ccw > 700) {
      motor.moveTo(nextPosition - DISTANCE_MOVE);
      motor.runToPosition();
    }
    // Save new position as position 0
    setOriginPosition();
    printCountPosition(count);
  } else {
    motor.setAcceleration(300);
    
    printCountPosition(count);
    if ((boo) && (count < 3)) {
      int currentRev = rev * (count + 1);
      motor.moveTo(currentRev);
      motor.runToPosition();
      count = count + 1;
    } else if ((boo) && (count >= 3)) {
      motor.moveTo(0);
      motor.runToPosition();
      count = 0;
    }
  }
}
void setOriginPosition() {
    motor.setCurrentPosition(0);
    motor.moveTo(0);
    motor.runToPosition();
}
void printCountPosition(int count) {
    Serial.print(count);
    Serial.print(" currentPos: ");
    Serial.println(motor.currentPosition());
}
