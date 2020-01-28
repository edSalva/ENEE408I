#include "ENEE408I.h"

Ultrasound leftUltrasound   (left_ping_pin);
Ultrasound centerUltrasound (center_ping_pin);
Ultrasound rightUltrasound  (right_ping_pin);
MotorControl leftMotor  (LeftMotorA_pin,  LeftMotorB_pin,  LeftMotorPWM_pin);
MotorControl rightMotor (RightMotorA_pin, RightMotorB_pin, RightMotorPWM_pin);

void go_stop() {
  leftMotor.halt();
  rightMotor.halt();
}

void go_forward() {
  leftMotor.forward();
  rightMotor.backward();
}

void go_backward() {
  leftMotor.backward();
  rightMotor.forward();
}

void go_left() {
  leftMotor.backward();
  rightMotor.backward();
}

void go_right() {
  leftMotor.forward();
  rightMotor.forward();
}

void setup() {
  Serial.begin(9600); // Set baud-rate
  leftMotor.setPWM(MOTOR_PWM);
  rightMotor.setPWM(MOTOR_PWM);

  go_forward();
  delay(1000);
  go_stop();
}

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
       go_right();
       strcat(line, " -> Right");
     }
     else if (left > STOP_DISTANCE_SIDE){ // Left has more room than right, so go left.
       go_left();
       strcat(line, " -> Left");
     }
     else {
       go_backward();
       strcat(line, " -> Back");
     }
   }
   else if (left <= STOP_DISTANCE_SIDE) { // Issue @ left, so go right
     go_right();
     strcat(line, " -> Right");
   }
   else if (right <= STOP_DISTANCE_SIDE) { // Issue @ right, so go left
     go_left();
     strcat(line, " -> Left");
   }
   else {
     go_forward();
     strcat(line, " -> Forward");
   }

  Serial.println(line);
  delay(100);
}
