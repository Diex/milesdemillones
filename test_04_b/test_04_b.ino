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
}


void initBuffer() {
  for (int col = 0; col < NUM_COLS; col++) {
    //      int col = 0;
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

  if (millis() - ptime > 250) {
//    count++;
    ptime = millis();
  }
}


// escanea linea por linea y la dibuja...
void render(long count) {
  for (int x = 0 ; x < NUM_COLS; x++) {
    latch(LOW);   
    for (int y = NUM_ROWS_BYTES ; y >= 0 ; y--) {
          shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST, fb[  (count + (x * NUM_ROWS_BYTES) + y) % (NUM_ROWS_BYTES * NUM_COLS)] );    //
//        if(x == 5) shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST, 0 );     // esto funciona oK
//        if(y == 2) shiftOut(ROWS_DATA, ROWS_SH, LSBFIRST, 0 );    // esto tambien funciona !!!
    }

    shiftOut(COLS_DATA, COLS_SH, MSBFIRST, cols[x]);
    latch(HIGH);
    delayMicroseconds(10);
  }

}


void latch(int st) {
  digitalWrite(LATCH, st);
}




