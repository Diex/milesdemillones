//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************
#include <limits.h>;

#define LATCH 6
#define DATA 11
#define COL_SH 8
#define ROW_SH 9

//int rowLatch = 13;
//int rowClock = 12;
//int rowData = 11;
//
//int colLatch = 8;
//int colClock = 9;
//int colData = 10;

#define col_0 B00000001
#define col_1 B00000010
#define col_2 B00000100
#define col_3 B00001000
#define col_4 B00010000
#define col_5 B00100000
#define col_6 B01000000
#define col_7 B10000000

char  cols[] = {col_0, col_1 ,col_2 ,col_3 ,col_4 ,col_5 ,col_6 ,col_7};

// voy a dibujar la data por filas (en vertical)
#define numPanels 8
#define numCols 8

struct panel{
  uint16_t data[numCols];
} fb [numPanels];


void setup() {
	pinMode(LATCH, OUTPUT);
	pinMode(DATA, OUTPUT);
	pinMode(COL_SH, OUTPUT);
	pinMode(ROW_SH, OUTPUT);

      cross();
}



void loop() {   
  
  
    
  for(int col = 0; col < 8; col++){
    latch(LOW);
    shiftOut(DATA, ROW_SH, MSBFIRST, B01010101);   
    shiftOut(DATA, COL_SH, MSBFIRST, 255 - cols[col]); 
    latch(HIGH);
    delay(1);
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
  
void cross(){
  for(int col = 0; col <8 ; col++){
    draw(crossdata[col] << 8, 0, col);
  }
}


void draw(uint16_t data, uint16_t panel, uint16_t col){
  fb[panel].data[col] = data;
}

void scanCol(int col){
  shiftOut(DATA, COL_SH, MSBFIRST, cols[col]); 
}

void latch(int st){
 digitalWrite(LATCH, st);
}


