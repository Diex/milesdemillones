//**************************************************************//

//****************************************************************
#include <limits.h>;
#include <SPI.h>


//#define LATCH 6 // PORTB2 (pin 10)
//#define ROWS_DATA 12   //PORTB4 (pin 12)
//#define ROWS_SH 9     //PORTB1 (pin 9)
//
//#define COLS_DATA 11       //PORTB3 (pin 11)
//#define COLS_SH 8          //PORTB0 (pin 8)


#define LATCH 6 // PORTB2 (pin 10)

//#define ROWS_DATA 12   //PORTB4 (pin 12)
//#define ROWS_SH 9     //PORTB1 (pin 9)

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
// como es una pantalla cada bit es un pixel (blanco o negro)
// solo se que mide tantos x tantos pixels
uint8_t fb[NUM_COLS * NUM_ROWS_BYTES ] = {};


void setup() {
  // TODO test para chequear que este bien conectado.
  pinMode(LATCH, OUTPUT);

//  pinMode(ROWS_DATA, OUTPUT);
//  pinMode(ROWS_SH, OUTPUT);
  pinMode(COLS_DATA, OUTPUT);
  pinMode(COLS_SH, OUTPUT);


  digitalWrite(SS, HIGH);  // ensure SS stays high
  SPI.begin ();
  Serial.begin(115200);
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

void loop() {
  render(count);
  count ++;
  return;
  
  if (millis() - ptime > 250) {
    count++;
    randomizeSome(100);
    ptime = millis();
  }
}
// escanea linea por linea y la dibuja...
void render(long count) {
  for (int x = 0 ; x < NUM_COLS; x++) {
    for (int y = (NUM_ROWS_BYTES  - 1 ) ; y >= 0 ; y --) {
        SPI.transfer(B10000001);
        SPI.transfer(B10000001);
    }
    shiftOut(COLS_DATA, COLS_SH, LSBFIRST, cols[0]);
    latch();  
  }
}


void randomizeSome(int howMany){
    for(int i = 0; i < howMany; i++){
      fb[random(0, NUM_ROWS_BYTES * NUM_COLS)] = random(0, 255);
    }
}

void latch() {
  digitalWrite(LATCH, LOW);
  delayMicroseconds(10);
  digitalWrite(LATCH, HIGH);
}




