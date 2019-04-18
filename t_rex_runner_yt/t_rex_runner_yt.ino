/*
 * Initial code for T-Rex runner derived from Youtube video:
 * https://www.youtube.com/watch?v=dquGH3xHE5k
 * LCD5110_Graph library from : 
 * http://www.rinkydinkelectronics.com/download.php?f=LCD5110_Graph.zip
 * Designed to run on Onion Omega2 + Arduino dock + Funduino Shield + Nokia 5110
 * Onion Omega2 : https://onion.io/omega2/
 * Onion Arduino Dock: https://onion.io/store/arduino-dock-r2/
 * Funduino Shield : https://www.sparkfun.com/products/9760 (or similar from eBay)
 * Nokia 5110 : https://www.sparkfun.com/products/10168 (or similar from eBay)
 */

#include <LCD5110_Graph.h>
LCD5110 lcd(9,10,11,12,13);

#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

extern uint8_t googledino[];
extern uint8_t googledino16x16[];
extern uint8_t googledinoDEAD16x16[];
extern uint8_t ground[];
extern uint8_t cactus[];
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
extern unsigned char TinyFont[];
unsigned long lastU=0, lastS=0, lastC=0,lastGO=0;
int xd=2,yd=24;
int xc=84,yc=24;
int k=-1;
int GO=0;
int score=0,hscore=0;

void setup() {
 lcd.InitLCD();
 lcd.setFont(SmallFont);
}

void loop() {
  if ((analogRead(1)<100||yd!=24)&&millis()-lastS>(unsigned long)30/sqrt(yd-4)){
    lastS=millis();
    yd+=k;
    if (yd==24||yd==5){k*=-1;}
    }

  if (millis()-lastC>10){lastC=millis();xc--; if (xc==-6){xc=84;}}

  if (xc>-1&&xc<18&&yd>14)GO=1;
  
  if (millis()-lastU>50){
    score++;
    lastU=millis();
    lcd.clrScr();
    
    lcd.drawBitmap(0,40,ground,84,8);
    lcd.drawBitmap(xd,yd,googledino16x16,16,16);
    lcd.drawBitmap(xc,yc,cactus,6,16);
    lcd.drawLine(0,40,83,40);
    
    lcd.printNumI(score,RIGHT,0);lcd.print("HI",0,0);lcd.printNumI(hscore,15,0);
    if (GO) {lastGO=millis();
    lcd.print("GAME_OVER",CENTER,8);lcd.drawBitmap(xd,yd,googledinoDEAD16x16,16,16);lcd.update();
    while(analogRead(1)>100||millis()-lastGO<1000);
    GO=0;xd=2;yd=24;xc=84;yc=24; k=-1;
    if (score>hscore){hscore=score;}score=0;}

    
    lcd.update();
  }
}
