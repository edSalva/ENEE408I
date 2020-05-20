#include "ENEE408I.h"

Ultrasound leftUltrasound   (left_ping_pin);
Ultrasound centerUltrasound (center_ping_pin);
Ultrasound rightUltrasound  (right_ping_pin);
MotorControl leftMotor  (LeftMotorA_pin,  LeftMotorB_pin,  LeftMotorPWM_pin);
MotorControl rightMotor (RightMotorA_pin, RightMotorB_pin, RightMotorPWM_pin);
PID leftPID (LEFT_KP, LEFT_KI, LEFT_KD);
PID rightPID (RIGHT_KP, RIGHT_KI, RIGHT_KD);
char lastCmd[256];

void setup() {
  Serial.begin(9600); // Set baud-rate
  leftMotor.setSpeed(START_SPEED);
  rightMotor.setSpeed(START_SPEED);
  strcpy(lastCmd, "h");
}

void process(const char *cmd) {
  switch (cmd[0]) {
    case 'F':
    case 'f':
    leftMotor.setSpeed(START_SPEED);
    rightMotor.setSpeed(START_SPEED);
     leftMotor.forward();
     rightMotor.backward();
     Serial.println("F");
     break;
   case 'R':
   case 'r':
   leftMotor.setSpeed(START_SPEED);
   rightMotor.setSpeed(START_SPEED);
     leftMotor.forward();
     rightMotor.forward();
     Serial.println("R");
     break;
   case 'L':
   case 'l':
   leftMotor.setSpeed(START_SPEED);
   rightMotor.setSpeed(START_SPEED);
     leftMotor.backward();
     rightMotor.backward();
     Serial.println("L");
     break;
   case 'B':
   case 'b':
   leftMotor.setSpeed(START_SPEED);
   rightMotor.setSpeed(START_SPEED);
     leftMotor.backward();
     rightMotor.forward();
     Serial.println("B");
     break;
   case 'H':
   case 'h':
   leftMotor.setSpeed(START_SPEED);
   rightMotor.setSpeed(START_SPEED);
     leftMotor.halt();
     rightMotor.halt();
     Serial.println("Stopped");
     break;
   case 'e':
     int err;
     int lSpeed, rSpeed;
     sscanf(cmd, "e%d", &err);

       lSpeed = (int) leftPID.update(err);
       if(lSpeed > 0) {
         leftMotor.forward();
       } else {
         leftMotor.backward();
         lSpeed = -lSpeed;
       }
       lSpeed = (lSpeed > SPEED_MAX) ? SPEED_MAX : lSpeed;
       leftMotor.setSpeed(lSpeed);

       rSpeed = (int) rightPID.update(err);
       if(rSpeed > 0) {
         rightMotor.backward();
       } else {
         rightMotor.forward();
         rSpeed = -rSpeed;
       }
       rSpeed = (rSpeed > SPEED_MAX) ? SPEED_MAX : rSpeed;
       rightMotor.setSpeed(rSpeed);
       char line[256];
       sprintf(line, "F(%3d, %3d)", leftMotor.PWM_val, rightMotor.PWM_val);
       Serial.println(line);
     break;
    }
}

int i = 0;
char cmd[256];

void loop() {

  // put your main code here, to run repeatedly:
  int left, center, right;

  left   = leftUltrasound.getDistance();
  center = centerUltrasound.getDistance();
  right  = rightUltrasound.getDistance();


  if (center <= STOP_DISTANCE_CENTER) { // If Danger at center...
     // Check whether we should go left or right
     if (left <= right && right > STOP_DISTANCE_SIDE) { // Right has more room than left, so go right.
       leftMotor.forward();
       rightMotor.forward();
     }
     else if (left > STOP_DISTANCE_SIDE){ // Left has more room than right, so go left.
       leftMotor.backward();
       rightMotor.backward();
     }
     else { // Too close so go back
       leftMotor.backward();
       rightMotor.forward();
     }
   }
   else if (left <= STOP_DISTANCE_SIDE) { // Issue @ left, so go right
     leftMotor.forward();
     rightMotor.forward();
   }
   else if (right <= STOP_DISTANCE_SIDE) { // Issue @ right, so go left
     leftMotor.backward();
     rightMotor.backward();
   }
   else {
     process(lastCmd);
     while (Serial.available() > 0) {
       cmd[i] = Serial.read();
       if(cmd[i] == '\n') {
         cmd[i] = '\0';
         i = 0;
         strcpy(lastCmd,cmd);
         process(cmd);
        }
        else {
          ++i;
        }
    }
  }

  delay(100);

}
