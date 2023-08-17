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

int DOUT = A3;
const int rev = -400;
const int LIMIT = 1;
int count = 0;

bool boo = false;       // for XLink
bool firstRun = true;

void setup() {
  Serial.begin(9600);
  motor.setMaxSpeed(100);
  motor.setAcceleration(100);
  pinMode(12, OUTPUT);
}

void loop() {
  if (firstRun) {
    digitalWrite(12, HIGH);
    firstRun = false;
    delay(10000); // wait for 10s for first start/reset Arduino
    digitalWrite(12, LOW);
  }
  int xLink = analogRead(DOUT); 
  
  // XLink send a 1s 5V from the DOUT port to the analog port. 
  if (xLink > 1000) { // >1000 for 5V signal
    boo = true;
    
  } else {
    boo = false;
  }
  if ((boo) && (count < LIMIT)) {
    digitalWrite(12, HIGH);
    int currentRev = rev * (count + 1);
    motor.moveTo(currentRev);
    motor.runToPosition();
    count = count + 1;
    digitalWrite(12, LOW);
  } else if ((boo) && (count >= LIMIT)) {
    digitalWrite(12, HIGH);
    motor.moveTo(0);
    motor.runToPosition();
    count = 0;
    digitalWrite(12, LOW);
  }
}
//
//void setOriginPosition() {
//    motor.setCurrentPosition(0);
//    motor.moveTo(0);
//    motor.runToPosition();
//}
//void printCountPosition(int count) {
//    Serial.print(count);
//    Serial.print(" currentPos: ");
//    Serial.println(motor.currentPosition());
//}
