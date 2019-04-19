#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
 Serial.begin(9600);

 if (tcs.begin()) {
   Serial.println("Found sensor");
 } else {
   Serial.println("No TCS34725 found ... check your connections");
   whil
}

void loop() {
  // put your main code here, to run repeatedly:

}
