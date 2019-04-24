#include <Servo.h>
#include <Stepper.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//code for servo motor
 Servo servo; //name of the servo is servo
 
//code for stepper motor
 const int howManySteps = 512; //should be always 512
 Stepper DC(howManySteps, 4, 6, 5, 7); //declaring the steper motor named DX 
 int red = 13; 
 int grn = 12;
 int blu = 11;

//declaration of boolean values for color
 bool pink = false;
 bool peach = false;
 bool yellow = false;
 bool green = false;
 bool blue = false;
 bool none = false;

void setup() {
 Serial.begin(9600);
//for servo motor
 servo.attach(9);
 servo.write(0);
 
//for DC motor
 DC.setSpeed(40); //speed of DC is set to 40

//for color sensor
 if (tcs.begin()) {
   Serial.println("Found sensor");
 } else {
   Serial.println("No TCS34725 found ... check your connections");
   while (1); // halt!
 }
 pinMode (red, OUTPUT);
 pinMode (blu, OUTPUT);
 pinMode (grn, OUTPUT);
 pinMode (A4, INPUT);
 pinMode (A5, INPUT);
}

void loop() {
  DC.step(683); //dc motor makes one rotation (IF WE DO 3 HOLES, WE WILL HAVE 0.3 DEGREE OFF FOR EACH BALL
//color sensor and its read out
 float r, g, b;
 tcs.setInterrupt(false);  // turn on LED
 delay(60);  // takes 50ms to read but we don't need it anymore
 tcs.getRGB(&r, &g, &b);
 tcs.setInterrupt(true);

 Serial.println(String(r) + "\t" + String(g) + "\t" + String(b));

//resistor: 220 ohm
 if (r > 138 && g < 55 && b<60 && b > 40){ //pink
    
 analogWrite(red, 255); 
 analogWrite(grn, 100); 
 analogWrite(blu, 180); 
 pink = true;
 peach = false;
 yellow = false;
 green = false;
 blue = false;
 none = false;
 Serial.println("pink");
 delay(1000);

 }
 else if (r > 130 && g < 68 && b<50 && b > 30){ //peach
 analogWrite(red, 255); 
 analogWrite(grn, 218); 
 analogWrite(blu, 185);
 pink = false;
 peach = true;
 yellow = false;
 green = false;
 blue = false;
 none = false;
 Serial.println("peach");
 delay(1000);
 }
 else if (r < 104 && r > 50 && g < 120 && g > 85 && b < 79 && b > 59){ //green
 analogWrite(red, 0); 
 analogWrite(grn, 251); 
 analogWrite(blu, 0);
 pink = false;
 peach = false;
 yellow = false;
 green = true;
 blue = false;
 none = false;
 Serial.println("green");
 delay(1000);
 }
 else if (r < 96 && r > 61 && g < 115 && g > 87 && b < 65  && b > 52){ //lime green (yellow?)
 analogWrite(red, 154); 
 analogWrite(grn, 205); 
 analogWrite(blu, 50);
 pink = false;
 peach = false;
 yellow = true;
 green = false;
 blue = false;
 none = false;
 Serial.println("yellow");
 delay(1000);
  }
 else if (r < 92 && r > 36 && g < 97 && g > 85 && b > 80){ //blue
 analogWrite(red, 0); 
 analogWrite(grn, 0); 
 analogWrite(blu, 255);
 pink = false;
 peach = false;
 yellow = false;
 green = false;
 blue = true;
 none = false;
 Serial.println("blue");
 delay(1000);
 }
 else{
 analogWrite(red, 0); 
 analogWrite(grn, 0); 
 analogWrite(blu, 0);
 pink = false;
 peach = false;
 yellow = false;
 green = false;
 blue = false;
 none = true;
 Serial.println("none");
 delay(1000);
 }
//delay(1000);
//servo motor rotation
if(pink == true){
 servo.write(30);
}
if(peach == true){
 servo.write(60);
}
if(yellow == true){
 servo.write(90);
}
if(green == true){
 servo.write(120);
}
if(blue ==  true){
 servo.write(150);
}
if(none == true){
 servo.write(0);
}

// servo.write(30);
// delay(1000);
// servo.write(60);
// delay(1000);
}
