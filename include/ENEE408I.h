#include <Arduino.h>
#include "PID.h"

// Ping sensor pins
#define center_ping_pin 4
#define left_ping_pin   7
#define right_ping_pin  8

// Motor pins
#define LeftMotorA_pin    11
#define RightMotorA_pin   13
#define LeftMotorB_pin    10
#define RightMotorB_pin   12
#define LeftMotorPWM_pin  6
#define RightMotorPWM_pin 5

// PID constants
#define LEFT_KP 1
#define LEFT_KI 0
#define LEFT_KD 0
#define RIGHT_KP -1
#define RIGHT_KI 0
#define RIGHT_KD 0

//Other constants
#define START_SPEED 40
#define SPEED_MAX 255
#define STOP_DISTANCE_SIDE 10
#define STOP_DISTANCE_CENTER 20

//Motor controller class def
class MotorControl{
  public:
    byte INA_pin;
    byte INB_pin;
    byte PWM_pin;

    byte INA_dir;
    byte INB_dir;
    int PWM_val;

  public:
    MotorControl(byte INA_pin, byte INB_pin, byte PWM_pin){
      this->INA_pin = INA_pin;
      this->INB_pin = INB_pin;
      this->PWM_pin = PWM_pin;
      pinMode(this->INA_pin, OUTPUT);
      pinMode(this->INB_pin, OUTPUT);
      pinMode(this->PWM_pin, OUTPUT);
    }

   void setSpeed(int PWM_val){
      this->PWM_val = PWM_val;
      analogWrite(this->PWM_pin, this->PWM_val);
   }

   void forward(){
      digitalWrite(this->INA_pin, HIGH);
      digitalWrite(this->INB_pin, LOW);
   }

   void backward(){
      digitalWrite(this->INA_pin, LOW);
      digitalWrite(this->INB_pin, HIGH);
   }

   void halt() {
      //analogWrite(this->PWM_pin, 0);
      digitalWrite(this->INA_pin, LOW);
      digitalWrite(this->INB_pin, LOW);
   }
};

class Ultrasound
{
  private:
    int _pin;
  public:
    Ultrasound(int pin) {
      _pin = pin;
    }
    long getDistance() {
      long duration;

      pinMode(_pin, OUTPUT);

      // Short LOW pulse
      digitalWrite(_pin, LOW);
      delayMicroseconds(2);
      // Clean HIGH pulse
      digitalWrite(_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_pin, LOW);

      pinMode(_pin, INPUT);
      duration = pulseIn(_pin, HIGH);


      return duration / 29 / 2;
    }
};
