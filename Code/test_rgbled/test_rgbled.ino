int red = 11;
int grn = 10;                         //pins for RGBLED
int blu = 9;

int RGB[] = {0, 255, 0};
int off[] = {0, 0, 0};

void setup() {
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (grn, OUTPUT);

}

void loop() {
  //colorWrite(RGB);
  colorBlink(RGB, 1);

}

//function to write the color to a 
void colorWrite(int list[]){
  analogWrite(red, list[0]);
  analogWrite(grn, list[1]);
  analogWrite(blu, list[2]);
}

void colorBlink(int list[], int n){
  while(n>0){
    colorWrite(list);
    delay(250);
    colorWrite(off);
    delay(250);
    n--;
  }
}
