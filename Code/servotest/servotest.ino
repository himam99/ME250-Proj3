#include <Servo.h>
Servo servo;

void setup() {

  //initialize serial line
  Serial.begin(9600);
  
  //initialize servo motor
  servo.attach(9);

  servo.write(35);
  delay(2000);
  servo.write(70);
  delay(2000);
  servo.write(105);
  delay(2000);
  servo.write(140);
  delay(2000);
  servo.write(90);

  
}

void loop() {

}
