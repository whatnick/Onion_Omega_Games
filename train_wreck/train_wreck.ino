#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <math.h>

/* lcd display */
Adafruit_PCD8544 lcd = Adafruit_PCD8544(9, 10, 11, 13, 12);

/* frame size */
#define MAX_WIDTH  84
#define MAX_HEIGHT 48

/* constants for game params*/
#define THOMAS_START    1
#define THOMAS_STOP     2
#define PERCY_START 1
#define PERCY_STOP 2
#define CRASH_ZONE 5
#define MAX_SPEED 10
#define MIN_SPEED -10

/* Button Definitions */
#define UP_BUTTON 2
#define DOWN_BUTTON 4
#define LEFT_BUTTON 5
#define RIGHT_BUTTON 3

/* Train locations */
int thomas_loc = 0;
int percy_loc = 0;

/* Train speed */
int thomas_speed = 2;
int percy_speed = 2;

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
  read_buttons();
  read_joystick();
  cutoff_speed();
  
  lcd.clearDisplay();
  draw_train_track();
  
  thomas_loc = (thomas_loc + thomas_speed)%MAX_WIDTH;
  if (thomas_loc < 0) {
    thomas_loc = MAX_WIDTH;
  }
  percy_loc = (percy_loc + percy_speed)%MAX_HEIGHT;
  if (percy_loc < 0) {
    percy_loc = MAX_HEIGHT;
  }

  if ((abs(MAX_WIDTH/2 - thomas_loc) < CRASH_ZONE) && 
     (abs(MAX_HEIGHT/2 - percy_loc) < CRASH_ZONE))
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

void cutoff_speed(){
  if (thomas_speed > MAX_SPEED){
    thomas_speed = MAX_SPEED;
  }
  if (thomas_speed < MIN_SPEED){
    thomas_speed = MIN_SPEED;
  }
  if (percy_speed > MAX_SPEED){
    percy_speed = MAX_SPEED;    
  }
  if (percy_speed < MIN_SPEED) {
    percy_speed = MIN_SPEED;
  }
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

void read_joystick() {
  int vert = analogRead(A1);
  int horiz = analogRead(A0);
  if (vert < 100) {
    percy_speed += 1;
  }
  if (vert > 900) {
    percy_speed -= 1;
  }
  if (horiz < 100) {
    thomas_speed -= 1;
  }
  if (horiz > 900) {
    thomas_speed += 1;
  }
}

void read_buttons() {
  int thomas_start = digitalRead(RIGHT_BUTTON);
  int thomas_stop = digitalRead(LEFT_BUTTON);
  int percy_start = digitalRead(UP_BUTTON);
  int percy_stop = digitalRead(DOWN_BUTTON);

  if(thomas_start) {
    thomas_speed += 2;
  }

  if(thomas_stop) {
    thomas_speed -= 2;
  }

  if(percy_start) {
    percy_speed += 2;
  }

  if(percy_stop) {
    percy_speed -= 2;
  }
}

