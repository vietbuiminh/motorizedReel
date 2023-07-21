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
const int rev = 800*2;
int count = 1;

bool boo = false;       // for XLink
bool booManual = false; // for manual control 

void setup() {
  motor.setMaxSpeed(500);
  motor.setAcceleration(300);
}

void loop() {
  int cw = analogRead(cwPin);
  int ccw = analogRead(ccwPin);
  int xLink = analogRead(DOUT); 
  int val = analogRead(analogPin);
  
  if (cw > 700) { // >700 because max voltage is 3V
    digitalWrite(dirPin, HIGH);
  }
  if (ccw > 700) {
    digitalWrite(dirPin, LOW);
  }
  
  // XLink send a 1s 5V from the DOUT port to the analog port. 
  if (xLink > 1000) { // >1000 for 5V signal
    boo = true;
  } else {
    boo = false;
  }

  // Manual button engage overwrite the xLink boolean value
  if (val > 1000) {
    boo = false;
    booManual = true;
  } else {
    booManual = false;
  }
  if (booManual) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  } else {
    if ((boo) && (count <= 5)) {
      int currentRev = rev * count;
      motor.moveTo(currentRev);
      motor.runToPosition();
      count = count + 1;
    } else if ((boo) && (count > 5)) {
      motor.moveTo(0);
      motor.runToPosition();
      count = 1;
    }
  }
}
