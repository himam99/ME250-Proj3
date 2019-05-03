//ME 250 Spring 2019
//Project 3: Color Sorter
// Hammad Imam/Group 17

//initialize servo + stepper motors and color sensor
#include <Servo.h>
Servo servo;
#include <Stepper.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//CONFIG

//ALL PINS GO HERE
int red = 11;                         //
int grn = 10;                         //pins for RGBLED
int blu = 9;                          //
Stepper stpr(512, A0, A2, A1, A3);    //in the order in1, 3, 2, 4
int servopin = 3;                     //servo pin
//TCS: SDA -> A4, SCL -> A5.  This can't be changed (unless we really wanted to but we don't)

//initialize variables
int stepspeed = 40;                                       //speed that the stepper operates
int wheeldelay = 1500;                                    //time after the wheel rotates that the code is delayed
float sense[3];                                           //initialize an array to store the 3 values put out by the TCS
int servopos[] = {50, 75, 100, 125};                      //locations of the bins in degrees
String color = "none";                                    //initialize color to none to use as placeholder
int ballsTotal = 17;                                      //total balls to be sorted (used to make sure no balls get left in hopper/device)  
int ballsToSort = 12;                                     //this counts how many balls need to be processed, should be 0 when all balls are done
int range = 10;                                           //range from "centerpoint" defined in struct that a ball's RGB can be


//initialize colors
//make it easier for status updates using one of these 'default' colors
//less bright than other colors displayed (in theory)
int RGBwhite[] = {255, 255, 255};     //used for "done" (would pick green but it's a ball color) 
int RGBred[] = {255, 0, 0};           //used for "not done" or alerts or whatever
int RGBoff[] = {0, 0, 0,};            //used for turning the led off 


//ball color struct with name of color, threshold for sensing, and color for LED:
//row 1: String name of color
//row 2: array {red, grn, blu} for what the ball color should show up as
//row 3: array {red, grn, blu} color to send to rgbled

struct ballColor{
  String colorName;
  int color[3];
  int RGB[3];
} ;

ballColor orange = {
  "orange",
  {105, 80, 60},
  {255, 200, 0}
};

ballColor yellow = {
  "yellow",
  {75, 90, 75},
  {165, 255, 0}
};

ballColor green = {
  "green",
  {70, 90, 70},
  {0, 255, 0}
};

ballColor blue = {
  "blue",
  {55, 95, 90},
  {0, 255, 255}
};

ballColor pink = {
  "pink",
  {120, 60, 60},
  {200, 0, 255}
};

//create struct array "balls" with 5 elements (filled in later)
ballColor balls[5];

//bin struct with array of 4 names, one for each item in a bin
struct bin_type{
  char* item[4];
};

//create struct array "bin" with 3 elements, one for each bin
bin_type bin[3];

//create array used to define each element of struct array "bin" (filled in later)
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
  servo.attach(servopin);         //attach it to previously defined servo pin
  servo.write(servopos[3]);       //write it to the trash bin to start
 
  //initialize DC motor
  stpr.setSpeed(stepspeed);       //set the speed of the stepper rotation

  //initialize pins
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (grn, OUTPUT);
  pinMode (A4, INPUT);
  pinMode (A5, INPUT);

  //for color sensor
  if (tcs.begin()) {
    Serial.println("Found TCS");} 
  else {
    Serial.println("No TCS34725 found ... check your connections");
    while(1); // halt!
  }
  
  //Create an array of structs called "balls" from the previously generated structs.
  balls[0] = orange;
  balls[1] = yellow;
  balls[2] = green;
  balls[3] = blue;
  balls[4] = pink;

  //Writes all 3 elements of the bin[] array to the same contents
  for(int i=0; i<3; i++){
    bin[i] = binContent;
  }
}

void loop() {
  if( (ballsTotal%3) == 0){                          //finds modulo 3 of balls to sort (possibilities are 0, 1, or 2)
      stpr.step(682);                                 //if it's 0, rotate by 120 deg (rounded down)
  }                                                   
  else{
    stpr.step(683);                                   //otherwise rotate by 120 deg (rounded up)
  }                                                   //this part makes sure that it stays 360 deg every time without drifting

  Serial.println("\n\nRotating wheel.");              //write to the serial monitor that the wheel is rotating
  
  //color sensor and its read out
  tcs.setInterrupt(false);                            //turn on LED for TCS
  delay(50);                                          //take 50ms to read
  tcs.getRGB(&sense[0], &sense[1], &sense[2]);        //assign the read values to the array sense[]
  tcs.setInterrupt(true);                       

  colorBlink(RGBred, 3);                              //blink the led to show that the scan is starting

  //check color of the ball
  for(int i=0; i<5; i++){                             //for every color possible
    Serial.println(balls[i].colorName);               //display the color being checked to the serial monitor
    if( (checkColor(balls[i].color, sense)) ){        //if the sensed color matches a ball (see below for details on checkColor function)
        Serial.println("Ball found!");                //display that a ball was found
        color = balls[i].colorName;                   //set the color string to the name of the color
        colorBlink(RGBwhite, 1);                      //blink the led white once
        colorBlink(balls[i].RGB, 3);                  //blink the led with the sensed color
        ballsTotal--;                                 //subtract one from the total number of balls
        break;                                        //exit the for loop 
    }
   else{                                              //otherwise,
      color = "none";                                 //set the color string to "none"
      colorBlink(RGBred, 1);                          //blink the led red once to show that it's working
    }
  }

  //print to serial monitor
  Serial.println("\nR: " + String(sense[0]) + "\t G: " + String(sense[1]) + "\t B:" + String(sense[2]));  //print the RGB values to the serial monitor
  Serial.println("Ball detected: " + String(color) + "\n");                                               //print the sensed ball to the serial monitor

  //routing the ball
  [&]{
    for(int i = 0; i<3; i++){                                                                           //for each bin (3 total)
      for(int j = 0; j<4; j++){                                                                         //for each spot in the bin (4 total)
        Serial.println("Bin " + String((i+1)) + ", Item " + String(j+1) + " is " + bin[i].item[j]);     //write what is in that spot
        if(String(bin[i].item[j]) == String(color)){                                                    //if the sensed ball matches one of the colors
          servo.write(servopos[i]);                                                                     //move the servo to the bin
          Serial.println("Servo set to bin " + String(i+1));                                            //display what bin the servo is set to
          bin[i].item[j] = "done";                                                                      //"cross off" the spot by changing the array value to "done"
          ballsToSort--;                                                                                //subtract one from the number of balls sorted
          Serial.println("Bin " + String(i+1) + ", item " + String(j+1) + " done."); 
          Serial.println("Balls Left to Sort: " + String(ballsToSort));                                 //display the spot in the bin is complete, display # of balls left to sort
          colorBlink(RGBwhite, 3);                                                                      //flash that a ball has been sorted correctly
          return;                                                                                       //exit both for loops 
        }                                       
      }
    }
                                                //"if" nothing has been found in the for loops
    servo.write(servopos[3]);                   //move the servo to the trash bin
    Serial.println("Servo set to trash");       //display that the servo has been moved to the trash
    return;                                     //exit
  }();

  Serial.println("Balls left in hopper: " + String(ballsTotal));  //display how many balls are left in the hopper

  if( (ballsToSort==0) && (ballsTotal == 0)){         //if all balls have been sorted and there are no balls 
    Serial.println("\n\nDone.");                //print that the sorting is done
    colorWrite(RGBwhite);                       //turn the LED white
    while(1);                                   //stop running
  }
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
//FUNCTIONS USED IN CODE ABOVE 

>>>>>>> parent of b5b4068... minor changes to CAD and code
=======
/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$FUNCTIONS USED IN CODE ABOVE$$$$$$$$$$ 
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$DO NOT CHANGE THESE$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

>>>>>>> parent of a7b54f3... Fixed comments in code
//function to write the color from a list to a RGBLED
void colorWrite(int list[]){    //takes a list of 3 values to write to LED
  analogWrite(red, list[0]);
  analogWrite(grn, list[1]);    //takes each list element and writes it to an led
  analogWrite(blu, list[2]);
}

//function to blink the LED from a list and a number
void colorBlink(int list[], int n){   //takes a list of 3 values to write to LED and a number of blinks 
  while(n>0){                         //while the number of "blinks" is above 1
    colorWrite(list);                 //turn the LED on to a color
    delay(250);                       //wait 1/4 of a second
    colorWrite(RGBoff);               //turn the LED off
    delay(250);                       //wait 1/4 of a second
    n--;                              //subtract 1 from the number of "blinks left"
  }
}

//function to check the color of a ball
int checkColor(int colorList[], float sensedList[]){    //takes a list of what colors should be sensed and a list of what colors are sensed
  int sum = 0;                                          //initialize a sum to 0    
  for(int i=0; i<3; i++){                               //for each of 3 colors
    if( abs(colorList[i]-sensedList[i]) < range ) {     //if the "should be" color is within range of the "sensed" color
        sum++;                                          //add one to the sum
    }
  }
  if(sum == 3){                                         //if the sum is 3 (true for all 3 of RGB)
    return true;                                        //return true
  }
  else{                                                 //if the sum is less than 3 (at least one of RGB was not within range)
    return false;                                       //return false
  }
}


/*
 ________                                         __    ________ 
/        |                                      _/  |  /        |
$$$$$$$$/______    ______   _____  ____        / $$ |  $$$$$$$$/ 
   $$ | /      \  /      \ /     \/    \       $$$$ |      /$$/  
   $$ |/$$$$$$  | $$$$$$  |$$$$$$ $$$$  |        $$ |     /$$/   
   $$ |$$    $$ | /    $$ |$$ | $$ | $$ |        $$ |    /$$/    
   $$ |$$$$$$$$/ /$$$$$$$ |$$ | $$ | $$ |       _$$ |_  /$$/     
   $$ |$$       |$$    $$ |$$ | $$ | $$ |      / $$   |/$$/      
   $$/  $$$$$$$/  $$$$$$$/ $$/  $$/  $$/       $$$$$$/ $$/ 
    __       __  ________         ______   _______    ______  
/  \     /  |/        |       /      \ /       |  /      \ 
$$  \   /$$ |$$$$$$$$/       /$$$$$$  |$$$$$$$/  /$$$$$$  |
$$$  \ /$$$ |$$ |__          $$____$$ |$$ |____  $$$  \$$ |
$$$$  /$$$$ |$$    |          /    $$/ $$      \ $$$$  $$ |
$$ $$ $$/$$ |$$$$$/          /$$$$$$/  $$$$$$$  |$$ $$ $$ |
$$ |$$$/ $$ |$$ |_____       $$ |_____ /  \__$$ |$$ \$$$$ |
$$ | $/  $$ |$$       |      $$       |$$    $$/ $$   $$$/ 
$$/      $$/ $$$$$$$$/       $$$$$$$$/  $$$$$$/   $$$$$$/  
                                                                                                                                                                                                                                                                
 */
