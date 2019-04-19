//initialize servo + stepper motors and color sensor
#include <Servo.h>
Servo servo;
#include <Stepper.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//CONFIG

//ALL PINS GO HERE
int red = 13;
int grn = 12;
int blu = 11;
Stepper stpr(512, 4, 6, 5, 7);
int servopin = 9;
//TCS: SDA -> A4, SCL -> A5. This can't be changed (unless we really wanted to but we don't)


//initialize variables
int stepspeed = 40;                                             //speed that the stepper operates
float sense[3];                                                 //initialize the 3 values put out by the TCS
int blinknum = 5;                                               //how many times the LED will blink when signalling (unless custom number used)
int servopos[] = {30, 60, 90, 120};                             //locations of the bins
char color = "none";                                            //initialize color to none to use as placeholder

//initialize colors
//make it easier for status updates using one of these 'default' colors
//less bright than other colors displayed (in theory)
int RGBwhite[] = {128, 128, 128};     //used for "done" (would pick green but it's a ball color) 
int RGBred[] = {128, 0, 0};           //used for "not done" or alerts or whatever


//color thresholds + values to display as structs:
//row 1: String name of color
//row 2: array {red low, red high, grn low, grn hi, blu low, blu hi} for color read thresh
//row 3: array {red, grn, blue} color to send to rgbled

struct ballColor{
  char color;
  int thresh[6];
  int RGB[3];
} ;

struct ballList{
  ballColor balls;
};

ballColor orange = {
  "orange",
  {139, 149, 52, 62, 36, 46},
  {0, 255, 255}
};

ballColor yellow = {
  "yellow",
  {58,68,106,116,55,65},
  {0, 255, 255}
};

ballColor green = {
  "green",
  {48, 58, 111, 121, 63, 73},
  {0, 255, 0}
};

ballColor blue = {
  "blue",
  {41, 51, 90, 100, 95, 105},
  {0, 255, 255}
};

ballColor pink = {
  "pink",
  {150, 160, 42, 52, 41, 71},
  {170, 0, 255}
};

ballColor balls[5];

//setup bins as an array of 3 structs
//might change this to matrix/multidimensional array when im not so tired

struct bin_type{
  char* item[3];
};

bin_type bin[3];

bin_type binContent = {
  {"pink", "pink", "yellow", "blue"}
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!IF TESTING DO NOT CHANGE ANYTHING BELOW THIS POINT!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


void setup() {

  //initialize serial line
  Serial.begin(9600);
  
  //initialize servo motor
  servo.attach(servopin);
  servo.write(0);
 
  //initialize DC motor
  stpr.setSpeed(stepspeed);

  //for color sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");} 
  else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  //initialize pinouts
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (grn, OUTPUT);
  pinMode (A4, INPUT);
  pinMode (A5, INPUT);

  //Create an array of structs called "balls" from the previously generated structs.
  //I'm aware this is a bit redundant, but it helps keep the user defined variables
  //all in one place up above, as well as more concise since the whole struct can be
  //defined at once instead of needing to be done as each individual value.
  balls[0] = orange;
  balls[1] = yellow;
  balls[2] = green;
  balls[3] = blue;
  balls[4] = pink;

  //Similar to above but with the bins
  //Use for 3 identical bins with the same content
  for(int i=0; i<4; i++){
    bin[i] = binContent;
  }
}


void loop() {
  stpr.step(683); //stepper motor rotates 120deg (IF WE DO 3 HOLES, WE WILL HAVE 0.3 DEGREE OFF FOR EACH BALL
  delay(1500);
  
  //color sensor and its read out
  tcs.setInterrupt(false);  // turn on LED
  delay(50);  // takes 50ms to read
  tcs.getRGB(&sense[0], &sense[1], &sense[2]);
  tcs.setInterrupt(true);

  colorBlink(RGBred, 3);

  for(int i = 0; i<5; i++){                       //for every color possible
    if(checkColor(balls[i].thresh, sense) ){      //if the sensed color is within the threshold
        color = balls[i].color;                   //set the color string to the name of the color
        colorBlink(balls[i].RGB, blinknum);       //blink the led with the sensed color
    }
  }
  
  Serial.println("R: " + String(sense[0]) + "\t G: " + String(sense[1]) + "\t B:" + String(sense[2])); //print the RGB values to the serial monitor
  Serial.println(color);                          //print the sensed color to the serial monitor

  for(int i = 0; i<3; i++){                       //for each bin (3 total)
    for(int j = 0; i<3; i++){                     //for each color desired in the bin (3 total)
      if(bin[i].item[j] == color){                //if the sensed ball matches one of the colors
        servo.write(servopos[i]);                 //move the servo to the bin
        bin[i].item[j] = "";                      //"cross off" the ball that was guided by changing the array value to null
        break;                                    //exit the for loop 
      }
    }
  }

}

void colorWrite(int list[]){
  analogWrite(red, list[0]);
  analogWrite(grn, list[1]);
  analogWrite(blu, list[2]);
}

void colorBlink(int list[], int n){
  while(n>0){
  colorWrite(list);
  delay(250);
  n -= 1;
  }
}

int checkColor(int colorList[], float sensedList[]){
  int sum = 0;
  for(int i=0; i<3; i++){
    if(colorList[(2*i)] <= sensedList[i] && sensedList[i] <= colorList[(2*i + 1)]){
      sum++;
    }
  }
  if(sum = 3){
    return true;
  }
}
