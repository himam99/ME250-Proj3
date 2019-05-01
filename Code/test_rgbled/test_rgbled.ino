int red = 11;
int grn = 10;            //pins for RGBLED
int blu = 9;


//edit this line to test colors
int RGB[] = {0, 255, 0};


int RGBoff[] = {0, 0, 0};

void setup() {
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (grn, OUTPUT);

}

void loop() {
  //colorWrite(RGB);
  colorBlink(RGB, 1);

}

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
