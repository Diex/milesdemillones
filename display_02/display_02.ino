#include <limits.h>

#define SEG_DATA   12   
#define SEG_SH     13   
#define LATCH      10 
#define DIG_SH     9 
#define DIG_DATA   6 

#define DIG_0 B10000000
#define DIG_1 B01000000
#define DIG_2 B00100000
#define DIG_3 B00010000
#define DIG_4 B00001000
#define DIG_5 B00000100
#define DIG_6 B00000010
#define DIG_7 B00000001

short digits[] = {DIG_0, DIG_1 , DIG_2 , DIG_3 , DIG_4 , DIG_5 , DIG_6 , DIG_7};

#define NUM_DIGITS 8
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels


void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);
  pinMode(SEG_DATA, OUTPUT);
  pinMode(SEG_SH, OUTPUT);
  pinMode(DIG_DATA, OUTPUT);
  pinMode(DIG_SH, OUTPUT);


  Serial.begin(115200);
  
  latch();  
  setRands();
}



long count = 0;
long ptime = 0;

void loop() {
  render(count);
//  count++;
}


short rands[8];

void setRands(){
  for(int i =0 ; i < 8 ; i++){
    rands[i] = random(255);
  }
}
// escanea linea por linea y la dibuja...
void render(int count) {
//  int number = 0;
  for (int x = 0 ; x < NUM_DIGITS; x++) {
    shiftOut(SEG_DATA, SEG_SH, LSBFIRST, rands[x]);   
    shiftOut(DIG_DATA, DIG_SH, LSBFIRST, digits[x]);
    latch();
    delay(5);
  }
}


short segments[] = { 
  B00000000,  //A
  B01000000,  //B
  B00100000,  //C
  B00010000,  //D
  B00001000,  //E
  B00000100,  //F
  B00000010   //G   
};

short zero(){
 return  segments[0] | segments[1] | segments[2] | segments[3] | segments[4] | segments[5] | segments[6];
}


void latch() {
  digitalWrite(LATCH, LOW);
  digitalWrite(LATCH, HIGH);
//    digitalWrite(LATCH, LOW);
}
