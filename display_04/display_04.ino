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


void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);
  pinMode(DIG_DATA, OUTPUT);
  pinMode(DIG_SH, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider (SPI_CLOCK_DIV8);

  pinMode(SYNC_IN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SYNC_IN), sync, RISING);

  Serial.begin(115200);
  
  latch();  
  setData();

}



long count = 0;
long ptime = 0;

void loop() {
  render();
}


int syncPulses = 0;

void sync(){
  syncPulses++;
  if(syncPulses == 128 ){
    setData();
    syncPulses = 0;
  }
}

unsigned int w = 0;
void setData(){
    w++;
    data[w%8] = decodeHexa(random(16));
}

void render() {
  for (int x = 0 ; x < NUM_DIGITS; x++) {
    digitalWrite(LATCH, LOW);
    SPI.transfer(data[x]);
    shiftOut(DIG_DATA, DIG_SH, LSBFIRST, digits[x]);
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(200);
  }
}


uint8_t decodeHexa(int data){
  switch(data){
  
    case 0:
    return zero();
    
    case 1:
    return one();
    
    case 2:
    return two();
    
    case 3:
    return three();
    
    case 4:
    return four();
    
    case 5:
    return five();
    
    case 6:
    return six();
    
    case 7:
    return seven();
    
    case 8:
    return eigth();
    
    case 9:
    return nine();
    
    case 10:
    return a();
    
    case 11:
    return b();
    
    case 12:
    return c();
    
    case 13:
    return d();
    
    case 14:
    return e();
    
    case 15:
    return f();
    
  }

}


short segments[] = { 
  B10000000,  //A
  B01000000,  //B
  B00100000,  //C
  B00010000,  //D
  B00001000,  //E
  B00000100,  //F
  B00000010   //G   
};


uint8_t zero(){
 return  segments[A] | segments[B] | segments[C] | segments[D] | segments[E] | segments[F];
}

uint8_t one(){
 return  segments[B] | segments[C];
}

uint8_t two(){
 return  segments[A] | segments[B] | segments[G] | segments[E] | segments[D];
}

uint8_t three(){
 return  segments[A] | segments[B] | segments[G] | segments[C] | segments[D];
}

uint8_t four(){
 return  segments[F] | segments[G] | segments[B] | segments[C] ;
}

uint8_t five(){
 return  segments[A] | segments[F] | segments[G] | segments[C] | segments[D];
}

uint8_t six(){
 return  segments[A] | segments[F] | segments[G] | segments[E] | segments[D] | segments[C];
}

uint8_t seven(){
 return  segments[A] | segments[B] | segments[C];
}

uint8_t eigth(){
 return  segments[A] | segments[B] | segments[C] | segments[D] | segments[E] | segments[F] | segments[G];
}

uint8_t nine(){
 return  segments[A] | segments[F] | segments[G] | segments[B] | segments[C] ;
}

uint8_t a(){
 return  segments[A] | segments[B] | segments[C] | segments[E] | segments[F] | segments[G];
}

uint8_t b(){
 return   segments[C] | segments[D] | segments[E] | segments[F] | segments[G];
}

uint8_t c(){
 return  segments[A] | segments[D] | segments[E] | segments[F] ;
}

uint8_t d(){
 return  segments[B] | segments[C] | segments[D] | segments[E] | segments[G];
}

uint8_t e(){
 return  segments[A] | segments[D] | segments[E] | segments[F] | segments[G];
}

uint8_t f(){
 return  segments[A] | segments[E] | segments[F] | segments[G];
}
















void latch() {
  digitalWrite(LATCH, LOW);
  digitalWrite(LATCH, HIGH);
//    digitalWrite(LATCH, LOW);
}
