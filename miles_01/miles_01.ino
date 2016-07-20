//**************************************************************//

//****************************************************************
#include <limits.h>;
#include <SPI.h>


//#define LATCH 6 // PORTB2 (pin 10)
//#define ROWS_DATA 12   //PORTB4 (pin 12)
//#define ROWS_SH 9     //PORTB1 (pin 9)
//#define COLS_DATA 11       //PORTB3 (pin 11)
//#define COLS_SH 8          //PORTB0 (pin 8)

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
//#define NUM_ROWS 8
// voy a dividir esas filas en algo manejable y util
// es decir que lo acomodo como bytes en el array para que luego sea facil shiftear
// por eso tomo la medida de "short"
#define NUM_ROWS_BYTES 16 // 128/8 = 16 bytes
//#define NUM_ROWS_BYTES 1 // 128/8 = 16 bytes
// nunca puede ser mas de 8 porque las columnas
// se multiplexan en paralelo
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
int del = 30 ;
void loop() {
  render(count);
  if (millis() - ptime > del ) {
    count++;
//    for (int p = NUM_PANELS; p > 0 ; p--) {
//      swapPanel(p - 1, p);
//    }
//    randomizePanel(0);
    swapPanel((count - 1) % 8, count % 8);
    if(count%8 == 0) randomizePanel(0);

    ptime = millis();
  }
}
// escanea linea por linea y la dibuja...
void render(long count) {
  for (int x = 0 ; x < NUM_COLS; x++) {
    for (int y = (NUM_ROWS_BYTES  - 1 ) ; y >= 0 ; y --) {
      SPI.transfer(fb[(x * NUM_ROWS_BYTES) + y]);
    }
    shiftOut(COLS_DATA, COLS_SH, LSBFIRST, cols[x]);
    latch();
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

void latch() {
  digitalWrite(LATCH, LOW);
  delayMicroseconds(10);
  digitalWrite(LATCH, HIGH);
}




