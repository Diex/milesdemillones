#include <limits.h>

#define SEG_DATA   12   //PORTB4 (pin 12)
#define SEG_SH     11     //PORTB1 (pin 9)
#define LATCH      10 // PORTB2 (pin 10)
#define DIG_SH     9          //PORTB0 (pin 8)
#define DIG_DATA   8       //PORTB3 (pin 11)

#define DIG_0 B10000000
#define DIG_1 B01000000
#define DIG_2 B00100000
#define DIG_3 B00010000
#define DIG_4 B00001000
#define DIG_5 B00000100
#define DIG_6 B00000010
#define DIG_7 B00000001

uint8_t digits[] = {DIG_0, DIG_1 , DIG_2 , DIG_3 , DIG_4 , DIG_5 , DIG_6 , DIG_7};

#define NUM_DIGITS 8
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels


void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);
  pinMode(SEG_DATA, OUTPUT);
  pinMode(SEG_DATA, OUTPUT);
  pinMode(DIG_SH, OUTPUT);
  pinMode(DIG_SH, OUTPUT);


  Serial.begin(115200);
  
  latch();  

}



long count = 0;
long ptime = 0;

void loop() {
  render(count);
  count++;
}


// escanea linea por linea y la dibuja...
void render(int count) {
  int number = 0;
  for (int x = 0 ; x < NUM_DIGITS; x++) {
    shiftOut(SEG_DATA, SEG_SH, LSBFIRST, random(0, 255));   
    shiftOut(DIG_DATA, DIG_SH, LSBFIRST, digits[x]);
    latch();
  }
}


short segments[] = { 
  B01000000,  //A
  B00100000,  //B
  B00010000,  //C
  B00000100,  //D
  B00000010 //E
   
};

void latch() {
  digitalWrite(LATCH, LOW);
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}
