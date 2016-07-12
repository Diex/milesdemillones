//**************************************************************//

//****************************************************************
#include <limits.h>;

#define LATCH 6 // PORTB2 (pin 10)
#define ROWS_DATA 12   //PORTB4 (pin 12)
#define ROWS_SH 9     //PORTB1 (pin 9)

#define COLS_DATA 11       //PORTB3 (pin 11)
#define COLS_SH 8          //PORTB0 (pin 8)



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

//  shiftOut(COLS_DATA, COLS_SH, MSBFIRST, cols[0]);
}


void initBuffer() {
  for (int col = 0; col < NUM_COLS; col++) {
    //      int col = 0;
    for (int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
    {
      fb[(col * NUM_ROWS_BYTES) + row_byte] = 0xFF; //random(0, 255);
    }
  }
}

long count = 0;
void loop() {

//  debugBuffer();
  render();

}

void debugBuffer() {
  Serial.print("buffer size: ");
  Serial.println(NUM_ROWS_BYTES * NUM_COLS);
  Serial.print("buffer length: ");
  Serial.println((sizeof(fb) / sizeof(*fb)));

  for (int col = 0; col < NUM_COLS; col++) {
    for (int row_byte = 0; row_byte < NUM_ROWS_BYTES; row_byte ++)
    {
      for (int bits = 7; bits >= 0; bits--) { // LSBFIRST
        uint8_t b = (fb[(col * NUM_ROWS_BYTES) + row_byte] >> bits) & 1;
        Serial.print(b);
      }
      Serial.print('|');
    }
    Serial.println();
  }
}

// escanea linea por linea y la dibuja...
void render() {
  latch(LOW);
  for (int x = 0 ; x < NUM_COLS; x++) {
      shiftOut(COLS_DATA, COLS_SH, MSBFIRST, 255 - cols[x]);    
  
      for(int y = NUM_ROWS_BYTES; y >= 0 ; y--){
        shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST,  255);    //fb[(x * NUM_ROWS_BYTES) + y]
      }
  }
  latch(HIGH);
}



void fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  uint8_t i;

  for (i = 0; i < 8; i++)  {
    
    if (bitOrder == LSBFIRST){
      // Check the content of this data bit
      // Shift data so this bit is LSB, and mask it with 1 so we only look at this bit.
      if ((val >> i) & 0x1 == 1)
      {
        // set data pin high, like PORTB |= 1<<4;
            PORTB |= _BV(dataPin);        
        // when pin 4 of portB is your data pin
        // Doing an OR will make pin B4 always high
      }
      else
      {
        // set data pin low, like PORTB &= ~(1<<4);
        PORTB &= ~_BV(dataPin);
        // Doing an AND with the inverse means all pins except B4 will be unchanged
      } 
    }else{
      // Check the content of this data bit
      // Shift data so this bit is LSB, and mask it with 1 so we only look at this bit.
      if ((val) & (0xFF >> i) == 1)
      {
        // set data pin high, like PORTB |= 1<<4;
            PORTB |= _BV(dataPin);        
        // when pin 4 of portB is your data pin
        // Doing an OR will make pin B4 always high
      }
      else
      {
        // set data pin low, like PORTB &= ~(1<<4);
        PORTB &= ~_BV(dataPin);
        // Doing an AND with the inverse means all pins except B4 will be unchanged
      } 
    
    }
     

  }
  
  
  PORTB |= _BV(clockPin);   
  PORTB &= ~_BV(clockPin);
  

////    digitalWrite(dataPin, !!(val & (1 << i)));
//    else
//      digitalWrite(dataPin, !!(val & (1 << (7 - i))));

//    digitalWrite(clockPin, HIGH);
//    digitalWrite(clockPin, LOW);
  
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
void latch(int st) {
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




