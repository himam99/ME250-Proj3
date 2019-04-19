//initialize servo + stepper motors and color sensor
#include <Servo.h>
#include <Stepper.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//CONFIG

//pins
int red = 13;
int grn = 12;
int blu = 11;
Stepper stpr(512, 4, 6, 5, 7);
int servopin = 9;
int stepspeed = 40;

//color thresholds + servo thresholds
//arrays of the form: [name, red low, red hi, grn low, grn hi, blu low, blu hi, servo pos]
//where "red low" is the lower bound for the red light detected, etc


void setup() {


}

void loop() {
  // put your main code here, to run repeatedly:

}
