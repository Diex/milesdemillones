/**************************************************************
  miles de millones
  CCK 2018
**************************************************************/
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


#define BUZZER 10

uint16_t value = 0;
boolean found = false;

//const uint16_t key0 = B11111111 << 8 | B11111111;
//const uint16_t key1 = B11111111 << 8 | B11111111;
//const uint16_t key2 = B11111111 << 8 | B11111111;
//const uint16_t key3 = B11111111 << 8 | B11111111;
//const uint16_t key4 = B11111111 << 8 | B11111111;
//const uint16_t key5 = B11111111 << 8 | B11111111;
//const uint16_t key6 = B11111111 << 8 | B11111111;
//const uint16_t key7 = B11111111 << 8 | B11111111;


// la clave la generamos de forma natural tirando una moneda de 1 peso
// sol 1 / escudo 0
const uint16_t key0 = B10000101 << 8 | B11001001; // juan muiño
const uint16_t key1 = B11000001 << 8 | B11000110; // rodrigo alcon
const uint16_t key2 = B00001101 << 8 | B11011101; // juan hoff
const uint16_t key3 = B00010110 << 8 | B00110011;
const uint16_t key4 = B01001011 << 8 | B10001100;
const uint16_t key5 = B00011001 << 8 | B11001011;
const uint16_t key6 = B01101111 << 8 | B00100011;
const uint16_t key7 = B11110001 << 8 | B10010000; // diex

uint16_t key[] = {key0, key1, key2, key3, key4, key5, key6, key7};

//uint16_t white[] = {
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111,
//  B11111111 << 8 | B11111111
//};

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

LedControl lc = LedControl(3, 5, 4, 1);

void setup() {

  pinMode(LATCH, OUTPUT);
  pinMode(COLS_DATA, OUTPUT);
  pinMode(COLS_SH, OUTPUT);

  digitalWrite(SS, HIGH);  // ensure SS stays high
  Serial.begin(115200);
  SPI.begin ();

  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider (SPI_CLOCK_DIV8);


  temp = panels[0]; // temp guarda la direccion de panels[0]

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  getRandomSeed();
  
}

void getRandomSeed()
{
  uint16_t seed = 0;
  for(int i = 0; i < 1E3; i++) seed += seed ^ analogRead(random(6));
  randomSeed(seed);
}

// http://unixwiz.net/techtips/reading-cdecl.html
unsigned long count = 0;
unsigned long ptime = 0;
unsigned long dtime = 60;


void loop() {
  if (found) return;
  if ((unsigned long)(millis() - ptime) > dtime) {
    found = true;
    update();
    tone(BUZZER, random(2000, 3000), 25);
  }
  render();
}


void update()
{
  temp = panels[7];
  for (int col = 7 ; col >= 0 ; col--) {
    value = random(UINT_MAX);
    found = found && key[col] == value;  // find the key
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




