//**************************************************************//

//****************************************************************
#include <limits.h>;

#define LATCH 6

#define ROWS_DATA 12
#define ROWS_SH 9

#define COLS_DATA 11
#define COLS_SH 8


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
// voy a dividir esas filas en algo manejable y util
// es decir que lo acomodo como bytes en el array para que luego sea facil shiftear
// por eso tomo la medida de "short"
#define NUM_ROWS_BYTES 128 / sizeof(uint8_t)
// nunca puede ser mas de 8 porque las columnas 
// se multiplexan en paralelo
#define NUM_COLS 8 
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels
uint8_t fb[NUM_COLS * NUM_ROWS_BYTES ] = {};


void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);
  pinMode(ROWS_DATA, OUTPUT);
  pinMode(COLS_DATA, OUTPUT);
  pinMode(COLS_SH, OUTPUT);
  pinMode(ROWS_SH, OUTPUT);

  Serial.begin(115200);
  
  initBuffer();
  debugBuffer();
}


void initBuffer(){
  // la posicion en el array siempre va a ser positiva por eso uso unsigned int
  for (unsigned int i = 0; i < NUM_COLS * NUM_ROWS; i++) {
    fb[i] = 0x1;
  }
}

void loop() {

  debugBuffer();
  delay(200);
}

void debugBuffer(){
    Serial.print("buffer size: ");
    Serial.println(sizeof(fb));
    Serial.write(fb, NUM_COLS * NUM_ROWS_BYTES);
}

//void shiftRowsForCol(unsigned int col, boolean print) {
//  if (print) Serial.print("col: ");
//  if (print) Serial.println(col);
//  // para todos los paneles...
//  for (int panel = 8; panel >= 0 ; panel --) {
//    // shifteo
//    if (print) Serial.print("panel: ");
//    if (print) Serial.print( panel);
//    if (print)     Serial.print(" : ");
//    uint16_t value = fb.data[(panel * NUM_COLS) + col];
//    shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST, value);
//    if (print)     Serial.println(value, BIN);
//  }
//
//  if (print)   Serial.println("--------------");
//}
//
//void draw(uint16_t data, uint16_t panel, uint16_t col) {
//  fb.data[(panel * NUM_COLS) + col] = data;
//}
//
//void scanCol(uint16_t col) {
//  shiftOut(COLS_DATA, COLS_SH, MSBFIRST, cols[col]);
//}
//
//void latch(uint16_t st) {
//  digitalWrite(LATCH, st);
//}



//
//uint16_t crossdata[] = {
//  B10000001,
//  B01000010,
//  B00100100,
//  B00011000,
//  B00011000,
//  B00100100,
//  B01000010,
//  B10000001
//};
//
//uint16_t arrowdata[] = {
//  B11000000,
//  B11100000,
//  B11110000,
//  B11111110,
//  B11111100,
//  B11110000,
//  B11100000,
//  B11000000
//};
//
//
//
//void cross() {
//  for (int col = 0; col < 8 ; col++) {
//    draw(crossdata[col], 0, col);
//  }
//}
//
//void arrow() {
//  for (int col = 0; col < 8 ; col++) {
//    draw(arrowdata[col], 0, col);
//  }
//}




