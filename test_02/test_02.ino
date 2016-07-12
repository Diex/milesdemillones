//**************************************************************//

//****************************************************************
#include <limits.h>;

#define LATCH 6

#define ROW_DATA 12
#define ROW_SH 9

#define COL_DATA 11
#define COL_SH 8


#define col_0 B00000001
#define col_1 B00000010
#define col_2 B00000100
#define col_3 B00001000
#define col_4 B00010000
#define col_5 B00100000
#define col_6 B01000000
#define col_7 B10000000

char  cols[] = {col_0, col_1 , col_2 , col_3 , col_4 , col_5 , col_6 , col_7};


#define numPanels 1
#define numCols 8

struct panel {
  uint16_t data[numCols];
} fb [numPanels];


void setup() {
  pinMode(LATCH, OUTPUT);
  pinMode(ROW_DATA, OUTPUT);
  pinMode(COL_DATA, OUTPUT);
  pinMode(COL_SH, OUTPUT);
  pinMode(ROW_SH, OUTPUT);

  //  cross();
//  arrow();



}


uint16_t count = 0;
uint32_t ptime = 0;
void loop() {

    for (uint8_t col = 0; col < 8; col++) {
    latch(LOW);
    
    char top = B10101010;
    char bottom = B01010101;  
    uint16_t r = ((col + count) % 2) == 0 ? top << 8 | bottom :  bottom << 8 | top;
    draw(r, 0, col);
    
    renderColumn(col);
    scanCol(col);
    latch(HIGH);
    delay(1);
  }

  
//  
//  
  if(millis() - ptime > 500) {
    ptime = millis();
    count++;
  }
//
//


}

void renderColumn(uint8_t col) {
  for (int panel = 0; panel < numPanels; panel ++) {
    shiftOut(ROW_DATA, ROW_SH, LSBFIRST, fb[panel].data[col]);
  }
}

uint16_t crossdata[] = {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

uint16_t arrowdata[] = {
  B11000000,
  B11100000,
  B11110000,
  B11111110,
  B11111100,
  B11110000,
  B11100000,
  B11000000
};



void cross() {
  for (int col = 0; col < 8 ; col++) {
    draw(crossdata[col], 0, col);
  }
}

void arrow() {
  for (int col = 0; col < 8 ; col++) {
    draw(arrowdata[col], 0, col);
  }
}

void draw(uint16_t data, uint16_t panel, uint16_t col) {
  fb[panel].data[col] = data;
}

void scanCol(int col) {
  shiftOut(COL_DATA, COL_SH, MSBFIRST, cols[col]);
}

void latch(int st) {
  digitalWrite(LATCH, st);
}


