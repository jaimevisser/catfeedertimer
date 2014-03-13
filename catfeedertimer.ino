#include <LiquidCrystal.h>
#include <Servo.h>

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) ((_time_) % SECS_PER_MIN)  
#define numberOfMinutes(_time_) (((_time_) / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (((_time_) % SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ((_time_) / SECS_PER_DAY)

const unsigned long timeincrement[3] = {
  SECS_PER_DAY,
  SECS_PER_HOUR,
  SECS_PER_MIN
};

const byte LCD_RS       = 12;
const byte LCD_ENABLE   = 11;
const byte LCD_D4       = 10;
const byte LCD_D5       =  9;
const byte LCD_D6       =  8;
const byte LCD_D7       =  7;

const byte SERVO        =  6;

const byte BUTTON_RIGHT =  5;
const byte BUTTON_DOWN  =  4;
const byte BUTTON_LEFT  =  3;
const byte BUTTON_UP    =  2;

const byte BUTTONS[] = {
  BUTTON_UP,BUTTON_DOWN,BUTTON_LEFT,BUTTON_RIGHT};

const byte LCD_COLS = 16;
const byte LCD_ROWS =  2;

unsigned long offset = 00 + (6*SECS_PER_HOUR);
unsigned long timer[4] = {
  (12*SECS_PER_HOUR),
  (12*SECS_PER_HOUR)+(1*SECS_PER_DAY),
  (12*SECS_PER_HOUR)+(2*SECS_PER_DAY),
  (12*SECS_PER_HOUR)+(3*SECS_PER_DAY)};

const byte STATE_SELECT = 5;
const byte STATE_CLOCK  = 4;

byte state = 0;
byte selected = 0;

LiquidCrystal lcd(
LCD_RS,
LCD_ENABLE,
LCD_D4,
LCD_D5,
LCD_D6,
LCD_D7
);

Servo servo;

void setup()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();

  servo.attach(SERVO);

  servo.write(90);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  printBoth();
}

void loop() {
  if(digitalRead(BUTTON_UP) == LOW){
    onUp();
    afterButton();
  }
  else if(digitalRead(BUTTON_DOWN) == LOW){
    onDown();
    afterButton();
  }
  else if(digitalRead(BUTTON_LEFT) == LOW){
    onLeft();
    afterButton();
  }
  else if(digitalRead(BUTTON_RIGHT) == LOW){
    onRight();
    afterButton();
  }

}

void afterButton(){
  delay(300);
}

void onUp(){
  if(state < 4 && selected < 2){
    timer[state] += timeincrement[selected];
    printBottomLine();
  }
  else if(state < 5 && selected == 3){
    selected = state;
    state = STATE_SELECT;
    printBoth();
  }
}

void onDown(){
  if((state < 4) && (timer[state] >= timeincrement[selected])){
    timer[state] -= timeincrement[selected];
    printBottomLine();
  }
  else if(state == 5){
    state = selected;
    selected = 0;
    printBoth();
  }
}

void onLeft(){
  if(selected > 0){
    selected -= 1;
    printBottomLine();
  }
}

void onRight(){
  if(state < 5){
    if(selected < 3){
      selected += 1;
      printBottomLine();
    }
  }
  else if(state == 5){
    if(selected < 4){
      selected += 1;
      printBottomLine();
    }
  }
}

void printBoth(){
  lcd.noCursor();
  printtl();
  printbl();
  setcursor();
}


void pintTopLine(){
  lcd.noCursor();
  printtl();
  setcursor();
}

void printBottomLine(){
  lcd.noCursor();
  printbl();
  setcursor();
}

void printtl(){
  lcd.setCursor(0,0);
  for(byte i=0;i<4;i++){
    if(i == state){
      lcd.print('*');
    }
    else if(timer[i] > 0){
      lcd.print('O');
    }
    else{
      lcd.print('.');
    }
  }
  
  lcd.print('C');

}

void printbl(){
  lcd.setCursor(1,1);

  unsigned long time; 
  if(state < 5){
    time = timer[state];
  }
  else{
    time = (millis() / 1000UL) + offset;
  }

  lcd.print("dag ");
  printPad(elapsedDays(time));
  lcd.print('/');
  printPad(numberOfHours(time));
  lcd.print(':'); 
  printPad(numberOfMinutes(time));
  if(state > 4){
    lcd.print(':');
    printPad(numberOfSeconds(time));
  }
  else{
    lcd.print(" ^");
  }
}

void setcursor(){
  if(state < 5){
    lcd.setCursor(5+(selected*3),1);
  }
  else if(state == STATE_SELECT){
    lcd.setCursor(selected,0);
  }

  if(state < 6){
    lcd.cursor();
  }
}

void printPad(byte t){
  if(t < 10) lcd.print("0");
  lcd.print(t);
}














