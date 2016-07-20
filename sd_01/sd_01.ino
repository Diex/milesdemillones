#include <limits.h>
#include <SPI.h>

//#define SEG_DATA   12   
//#define SEG_SH     13   
#define LATCH      6
#define DIG_SH     8 
#define DIG_DATA   7 

#define DIG_0 B10000000
#define DIG_1 B01000000
#define DIG_2 B00100000
#define DIG_3 B00010000
#define DIG_4 B00001000
#define DIG_5 B00000100
#define DIG_6 B00000010
#define DIG_7 B00000001

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define N 7

short digits[] = {DIG_0, DIG_1 , DIG_2 , DIG_3 , DIG_4 , DIG_5 , DIG_6 , DIG_7};

#define NUM_DIGITS 8
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels

uint8_t data[8];

#define SYNC_IN 3
#define DATA_IN 4

# define RED 5
# define GREEN 6
void setup() {
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LOW);
//
  pinMode(SYNC_IN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SYNC_IN), sync, RISING);

  Serial.begin(115200);
}



long count = 0;
long ptime = 0;

void loop() {
//  render();
}


int syncPulses = 0;

void sync(){
  syncPulses++;
  if(syncPulses == 128){
    blink();
    syncPulses = 0;
  }
}


void blink(){
      digitalWrite(RED, HIGH);
      delay(50);
      digitalWrite(RED, LOW);
}

