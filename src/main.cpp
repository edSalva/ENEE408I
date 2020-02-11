#include "ENEE408I.h"

Ultrasound leftUltrasound   (left_ping_pin);
Ultrasound centerUltrasound (center_ping_pin);
Ultrasound rightUltrasound  (right_ping_pin);
MotorControl leftMotor  (LeftMotorA_pin,  LeftMotorB_pin,  LeftMotorPWM_pin);
MotorControl rightMotor (RightMotorA_pin, RightMotorB_pin, RightMotorPWM_pin);
// PID leftPID (LEFT_KP, LEFT_KI, LEFT_KD);
// PID rightPID (RIGHT_KP, RIGHT_KI, RIGHT_KD);
char lastCmd;

void setup() {
  Serial.begin(9600); // Set baud-rate
  leftMotor.setSpeed(START_SPEED);
  rightMotor.setSpeed(START_SPEED);
  lastCmd = 'h';
}

int i = 0;
char cmd[256];

void loop() {

  // put your main code here, to run repeatedly:
  int left, center, right;
  char line[256];

  left   = leftUltrasound.getDistance();
  center = centerUltrasound.getDistance();
  right  = rightUltrasound.getDistance();

  sprintf(line, "Left: %3d | Center: %3d | Right: %3d", left, center, right);

  if (center <= STOP_DISTANCE_CENTER) { // If Danger at center...
     // Check whether we should go left or right
     if (left <= right && right > STOP_DISTANCE_SIDE) { // Right has more room than left, so go right.
       leftMotor.forward();
       rightMotor.forward();
       strcat(line, " -> R");
     }
     else if (left > STOP_DISTANCE_SIDE){ // Left has more room than right, so go left.
       leftMotor.backward();
       rightMotor.backward();
       strcat(line, " -> L");
     }
     else { // Too close so go back
       leftMotor.backward();
       rightMotor.forward();
       strcat(line, " -> B");
     }
   }
   else if (left <= STOP_DISTANCE_SIDE) { // Issue @ left, so go right
     leftMotor.forward();
     rightMotor.forward();
     strcat(line, " -> R");
   }
   else if (right <= STOP_DISTANCE_SIDE) { // Issue @ right, so go left
     leftMotor.backward();
     rightMotor.backward();
     strcat(line, " -> L");
   }
   else {
     char dir = lastCmd;
     while (Serial.available() > 0) {
       cmd[i] = Serial.read();
       if(cmd[i] == '\n') {
         cmd[i] = '\0';
         i = 0;
         if(sscanf(cmd, "Dir%c", &dir)) {
          lastCmd = dir;
         }
        }
        else {
          ++i;
        }
      }
       switch (dir) {
         case 'F':
         case 'f':
          leftMotor.forward();
          rightMotor.backward();
          strcat(line, " -> F");
          break;
        case 'R':
        case 'r':
          leftMotor.forward();
          rightMotor.forward();
          strcat(line, " -> R");
          break;
        case 'L':
        case 'l':
          leftMotor.backward();
          rightMotor.backward();
          strcat(line, " -> L");
          break;
        case 'B':
        case 'b':
          leftMotor.backward();
          rightMotor.forward();
          strcat(line, " -> B");
          break;
        case 'H':
        case 'h':
          leftMotor.halt();
          rightMotor.halt();
          strcat(line, " -> Stopped");
          break;
        default:
          break;
       }
     }

  Serial.println(line);
  delay(100);

}
