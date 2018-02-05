//**************************************************************//

//****************************************************************
#include <limits.h>;
#include <SPI.h>
#include "LedControl.h"


#define LATCH 6           // PORTB2 (pin 10)
#define COLS_DATA 7       //PORTB3 (pin 11)
#define COLS_SH   8       //PORTB0 (pin 8)

#define col_0 B00000001
#define col_1 B00000010
#define col_2 B00000100
#define col_3 B00001000
#define col_4 B00010000
#define col_5 B00100000
#define col_6 B01000000
#define col_7 B10000000

uint8_t cols[] = {col_0, col_1 , col_2 , col_3 , col_4 , col_5 , col_6 , col_7};

#define NUM_ROWS 128
#define NUM_ROWS_BYTES 16 // 128/8 = 16 bytes
#define NUM_COLS 8
#define NUM_PANELS 8
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels
uint8_t fb[NUM_COLS * NUM_ROWS_BYTES ] = {};

#define BUZZER 10

boolean found = false;
uint16_t value = 0;

const uint16_t key0 = B11111111 << 8 | B11111111;
const uint16_t key1 = B11111111 << 8 | B11111111;
const uint16_t key2 = B11111111 << 8 | B11111111;
const uint16_t key3 = B11111111 << 8 | B11111111;
const uint16_t key4 = B11111111 << 8 | B11111111;
const uint16_t key5 = B11111111 << 8 | B11111111;
const uint16_t key6 = B11111111 << 8 | B11111111;
const uint16_t key7 = B11111111 << 8 | B11111111;

uint16_t key[] = {key0, key1, key2, key3, key4, key5, key6, key7};

uint16_t demo[] = {
  B11111111 << 8 | B11111111,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

uint16_t demo1[] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  B11111111 << 8 | B11111111
};

uint16_t white[] = {
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111,
  B11111111 << 8 | B11111111
};

uint16_t  f1[8];
uint16_t  f2[8];
uint16_t  f3[8];
uint16_t  f4[8];
uint16_t  f5[8];
uint16_t  f6[8];
uint16_t  f7[8];
uint16_t  f8[8];


uint16_t * panels[8] = {f1, f2, f3, f4, f5, f6, f7 , f8}; //{white, white, white, white, white, white, white, white};
uint16_t * temp;

LedControl lc=LedControl(3,5,4,1); 

void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);
  pinMode(COLS_DATA, OUTPUT);
  pinMode(COLS_SH, OUTPUT);

  digitalWrite(SS, HIGH);  // ensure SS stays high
  Serial.begin(115200);
  SPI.begin ();

  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider (SPI_CLOCK_DIV8);
  initBuffer();

  temp = panels[0]; // temp guarda la direccion de panels[0]
  Serial.println(*temp);

  //    for(int col = 8 ; col >= 0 ; col--){
  //          panels[0][col] = random(UINT_MAX);
  //    }

   lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}


void initBuffer() {
  for (int col = 0; col < NUM_COLS; col++) {
    for (int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
    {
      fb[(col * NUM_ROWS_BYTES) + row_byte] = random(0, 255);
    }
  }
}

unsigned long count = 0;
unsigned long ptime = 0;
unsigned long dtime = 30;

// http://unixwiz.net/techtips/reading-cdecl.html



unsigned char reg = 1;

unsigned char generateNoise() {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;

  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;
  lobit = b31 ^ b29 ^ b25 ^ b24;
  newr = (reg << 1) | lobit;
  reg = newr;
  return reg & 1;
} // Changing this value changes the frequency.

uint16_t lastw = 0;

void loop() {
  if(found) return;
  
  
  if ((unsigned long)(millis() - ptime) > dtime) {
    found = true;
    update();
//    tone(BUZZER, random(2000, 3000), 25);
  }

  render();
}


void update()
{
    temp = panels[7];
    for (int col = 7 ; col >= 0 ; col--) {      
      value = random(UINT_MAX);
//      value = UINT_MAX;
//      Serial.print(col);
//      Serial.print("|");
//      Serial.print((uint16_t)key[col]);
//      Serial.print("|");
//      Serial.println(value);
      found = found && key[col] == value;
      panels[7][col] = value;
      lc.setColumn(0, col, value);
    }
    for (int panel = 7 ; panel > 0 ; panel--) {
      panels[panel] = panels[panel - 1];
    }
    panels[0] = temp;
    ptime = millis();

}

void render() {
  for (int column = 0 ; column < NUM_COLS; column++) {
    for (int panel = 7; panel >= 0 ; panel --) {  // shifteo una columna para cada panel. el panel 0 es el ultimo.
      SPI.transfer(panels[panel][column] >> 8);
      SPI.transfer(panels[panel][column]);
    }
    latch(column);
  }
}

void black() {
  for (int column = 0 ; column < NUM_COLS; column++) {
    for (int panel = 0; panel < NUM_PANELS ; panel ++) {  // shifteo una columna para cada panel. el panel 0 es el ultimo.
      SPI.transfer(0);
      SPI.transfer(0);
    }
    latch(column);
  }

}


void latch(int column) {
  shiftOut(COLS_DATA, COLS_SH, LSBFIRST, cols[column]);
  digitalWrite(LATCH, LOW);
  delayMicroseconds(10);
  digitalWrite(LATCH, HIGH);
}




