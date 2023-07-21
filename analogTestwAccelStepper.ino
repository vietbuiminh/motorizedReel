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
int count = 1;

bool boo;

void setup() {
  motor.setMaxSpeed(1000);
  motor.setAcceleration(500);
}

void loop() {
  int cw = analogRead(cwPin);
  int ccw = analogRead(ccwPin);
  int xLink = analogRead(DOUT);
  /*Serial.print("CW");
  Serial.println(cw);
  Serial.print("CCW");
  Serial.println(ccw);*/
//  Serial.println(xLink);
  if (cw > 700) {
    digitalWrite(dirPin, HIGH);
  }
  if (ccw > 700) {
    digitalWrite(dirPin, LOW);
  }
  
  int val = analogRead(analogPin);
  if ((val > 1000) || (xLink > 1000)) {
    boo = true;
  } else {
    boo = false;
  }
  //Serial.println(boo);
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
