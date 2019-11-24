#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <math.h>

/* lcd display */
Adafruit_PCD8544 lcd = Adafruit_PCD8544(9, 10, 11, 13, 12);

/* frame size */
#define MAX_WIDTH  84
#define MAX_HEIGHT 48

/* constants */
// TODO: Convert to ENUM's
#define THOMAS_START    1
#define THOMAS_STOP     2
#define PERCY_START 1
#define PERCY_STOP 2

/* Train locations */
int thomas_loc = 0;
int percy_loc = 0;

/* crash detection */
int is_crash = 0;

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(1));
  lcd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  train_move();
}

/*
 * Move trains
 */
void train_move() {
  lcd.clearDisplay();
  draw_train_track();
  

  thomas_loc = (thomas_loc + 2)%MAX_WIDTH;
  percy_loc = (percy_loc + 2)%MAX_HEIGHT;

  if ((abs(MAX_WIDTH/2 - thomas_loc) < 10) && 
     (abs(MAX_HEIGHT/2 - percy_loc) < 10))
  {
    is_crash = 1;
  }

  if(is_crash){
    write_crash();
  }

  lcd.display();

  if(is_crash){
    delay(1000);
    is_crash = 0;
  }
  
  delay(100);
}


void draw_train_track() {
  // Horizontal train track
  lcd.drawLine(0, MAX_HEIGHT/2-2, MAX_WIDTH, MAX_HEIGHT/2-2, BLACK);
  lcd.drawLine(0, MAX_HEIGHT/2+2, MAX_WIDTH, MAX_HEIGHT/2+2, BLACK);

  // Vertical train track
  lcd.drawLine(MAX_WIDTH/2-2, 0, MAX_WIDTH/2-2, MAX_HEIGHT, BLACK);
  lcd.drawLine(MAX_WIDTH/2+2, 0, MAX_WIDTH/2+2, MAX_HEIGHT, BLACK);

  // Draw Whole Train
  draw_train(thomas_loc,MAX_HEIGHT/2-2);
  draw_train(MAX_WIDTH/2-2,percy_loc);
}

void draw_train(int xloc, int yloc) {
  // Draw body
  lcd.fillRect(xloc, yloc-3, 15, 7, BLACK);
  // Draw drivers box
  // Draw wheels
  lcd.fillCircle(xloc+4, yloc+4, 2, BLACK);
  lcd.fillCircle(xloc+11, yloc+4, 2, BLACK);
  // Draw funnel
  lcd.fillRect(xloc+10, yloc-7, 3, 4, BLACK);
  // Draw smoke
}

void write_crash() {
  lcd.setCursor(0,0);
  lcd.setTextColor(BLACK);
  lcd.setTextSize(1);
  lcd.print("CRASH!!");
}

