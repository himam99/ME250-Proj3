#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
 Serial.begin(9600);

 if (tcs.begin()) {
   Serial.println("Found sensor");
 } else {
   Serial.println("No TCS34725 found ... check your connections");
   while (1); // halt!
 }
}

void loop() {
 float r, g, b;
 tcs.setInterrupt(false);  // turn on LED
 delay(60);  // takes 50ms to read but we don't need it anymore
 tcs.getRGB(&r, &g, &b);
 tcs.setInterrupt(true);

 Serial.println(String(r) + "\t" + String(g) + "\t" + String(b));
}
