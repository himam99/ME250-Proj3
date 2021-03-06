//initialize servo + stepper motors and color sensor
#include <Servo.h>
Servo servo;
#include <Stepper.h>

Stepper stpr(512, A0, A2, A1, A3); //in the order in1, 3, 2, 4
int servopin = 3;
int n = 0;

int servopos[] = {40, 65, 80, 100};                      //locations of the bins in degrees

void setup() {
  Serial.begin(9600);
  stpr.setSpeed(40);

  servo.attach(servopin);
 
}

void loop() {

  n=0;
  
  while(n<4){
    
    Serial.println("Rotating stepper...");
    stpr.step(683);

    delay(4000);
    Serial.println("Setting servo to position " + String(n) + "...");
    servo.write(servopos[n]);

    n++;
  }

  //servo.write(servopos[0]);
  


}
