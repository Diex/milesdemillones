//**************************************************************//

//****************************************************************
#include <limits.h>;
#include <SPI.h>


#define LATCH 6 // PORTB2 (pin 10)
#define COLS_DATA 7       //PORTB3 (pin 11)
#define COLS_SH   8          //PORTB0 (pin 8)

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
}


void initBuffer() {
  for (int col = 0; col < NUM_COLS; col++) {
    for (int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
    {
      fb[(col * NUM_ROWS_BYTES) + row_byte] = random(0, 255);
    }
  }
}

long count = 0;

long ptime = 0;
long dtime = 100;

// http://unixwiz.net/techtips/reading-cdecl.html


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
uint16_t f1[8];
uint16_t f2[8];
uint16_t f3[8];
uint16_t f4[8];
uint16_t f5[8];
uint16_t f6[8];
uint16_t f7[8];
uint16_t f8[8];
uint16_t f9[8];

uint16_t * panels[9] = {f1, f2, f3, f4, f5, f6, f7, demo, f9};

unsigned char reg = 1;

unsigned char generateNoise(){
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

void loop() {
  render(count);
  if (millis() - ptime > dtime) {
//    black();
//    delay(150);
    uint16_t *temp = panels[0]; // copia el contenido
    for(int row = 8 ; row > 0 ; row--){
          temp[row] =  random(UINT_MAX); //(count % 8 == row) ? UINT_MAX : 0;//generateNoise() << 8 | generateNoise();//
    }

    panels[0] = panels[1]; // copia el contenido
    panels[1] = panels[2]; // copia el contenido
    panels[2] = panels[3]; // copia el contenido
    panels[3] = panels[4]; // copia el contenido
    panels[4] = panels[5]; // copia el contenido
    panels[5] = panels[6]; // copia el contenido
    panels[6] = panels[7]; // copia el contenido
    panels[7] = temp;
        
    count ++;
    ptime = millis();
  }


  //  delay(100);
  //  if (millis() - ptime > del ) {
  //    count++;
  //    swapPanel((count - 1) % 8, count % 8);
  //    if(count%8 == 0) randomizePanel(0);
  //    ptime = millis();
  //  }
}

void swapPanels(uint16_t from, uint16_t to) {

  //    &to = from;

}


void render(long count) {
  for (int column = 0 ; column < NUM_COLS; column++) {
    for (int panel = 0; panel < NUM_PANELS ; panel ++) {  // shifteo una columna para cada panel. el panel 0 es el ultimo.
      SPI.transfer(panels[panel][column] >> 8);
      SPI.transfer(panels[panel][column]);
    }
    latch(column);
  }
}

void black(){
  

  for (int column = 0 ; column < NUM_COLS; column++) {
    for (int panel = 0; panel < NUM_PANELS ; panel ++) {  // shifteo una columna para cada panel. el panel 0 es el ultimo.
      SPI.transfer(0);
      SPI.transfer(0);
    }
    latch(column);
  }

}

void addByte(short num) {
  for (int i = NUM_ROWS_BYTES * NUM_COLS;  i > 0 ; i--) {
    fb[i] = fb[i - 1];
  }
  fb[0] = num;
}

void add128(uint8_t * num) {
  for (int col = NUM_COLS; col > 0; col--) { // la 0 no.
    swapColumn(col, col - 1);
  }
  for (int b = 0 ;  b < 16; b++) {
    fb[b] = num[b];
  }
}

void randomizePanel(int w) {
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = 0; y < 2; y++) {
      fb[(w * 2) + y + (x * NUM_ROWS_BYTES)] = random(0, 255);
    }
  }
}

void swapColumn(int dest, int src) {
  for (int byt = NUM_ROWS_BYTES; byt > 0; byt--) {
    fb[(dest * NUM_ROWS_BYTES) + byt] = fb[(src  * NUM_ROWS_BYTES)  + byt];
  }
}

void swapPanel(int src, int dest) {
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = 0; y < 2; y++) {
      fb[(dest * 2) + y + (x * NUM_ROWS_BYTES)] = fb[(src * 2) + y + (x * NUM_ROWS_BYTES)];
    }
  }
}


void randomizeSome(int howMany) {
  for (int i = 0; i < howMany; i++) {
    fb[random(0, NUM_ROWS_BYTES * NUM_COLS)] = random(0, 255);
  }
}

void latch(int column) {
  shiftOut(COLS_DATA, COLS_SH, LSBFIRST, cols[column]);
  digitalWrite(LATCH, LOW);
  delayMicroseconds(10);
  digitalWrite(LATCH, HIGH);
}




