#include <Servo.h>
Servo servo;

void setup() {

  //initialize serial line
  Serial.begin(9600);
  
  //initialize servo motor
  servo.attach(9);

  servo.write(120);
  /*Serial.println("Bin #1");
  delay(5000);
  servo.write(45);
  Serial.println("Bin #2");
  delay(5000);
  servo.write(60);
  Serial.println("Bin #3");
  delay(5000);
  servo.write(95);
  Serial.println("Trash");
  */
}

void loop() {

}
