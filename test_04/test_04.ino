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
//#define NUM_ROWS 8
// voy a dividir esas filas en algo manejable y util
// es decir que lo acomodo como bytes en el array para que luego sea facil shiftear
// por eso tomo la medida de "short"
#define NUM_ROWS_BYTES 16 // 128/8 = 16 bytes
//#define NUM_ROWS_BYTES 1 // 128/8 = 16 bytes
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
//      for(int col = 0; col < NUM_COLS; col++){
      int col = 0;
      for(int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
      {
              fb[(col * NUM_ROWS_BYTES) + row_byte] = 0xFFFF;
      }    
//    }
}

long count = 0;
void loop() {

  debugBuffer();
//    for (uint8_t col = 0; col < 8; col++) {
//    latch(LOW);
//    
//    char top = B10101010;
//    char bottom = B01010101;  
//    uint16_t r = ((col + count) % 2) == 0 ? top << 8 | bottom :  bottom << 8 | top;
//    
//    for (int b = 0; b < NUM_ROWS_BYTES; b++) {
//      shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST, r);
//    }
//    shiftOut(COLS_DATA, COLS_SH, MSBFIRST, cols[col]);
//    latch(HIGH);
//
//  }
  count++;
}

void debugBuffer(){
    Serial.print("buffer size: ");    
    Serial.println(NUM_ROWS_BYTES * NUM_COLS);
    Serial.print("buffer length: ");    
    Serial.println((sizeof(fb) / sizeof(*fb)));
    
    for(int col = 0; col < NUM_COLS; col++){      
      for(int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
      {
        for(int bits = 7; bits >= 0; bits--){  // LSBFIRST
          uint8_t b = (fb[(col * NUM_ROWS_BYTES) + row_byte] >> bits) & 1;
          Serial.print(b);
        }
        Serial.print('|');
      }    
      Serial.println();
    }
}

void render(){
 for(int x = 0 ; x < NUM_COLS; x++){
   hscan(x);
 }
}

void hscan(int x){
  latch(LOW);
  for(int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte++){
      shiftOut((uint8_t) ROWS_DATA, ROWS_SH, LSBFIRST, fb[(x * NUM_ROWS_BYTES) + row_byte]); // 
  }
  
  shiftOut((uint8_t)COLS_DATA, COLS_SH, MSBFIRST, cols[x]); 
  latch(HIGH);

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
void latch(uint16_t st) {
  digitalWrite(LATCH, st);
}



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




