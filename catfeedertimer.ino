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
  if(state < 4){
    if(digitalRead(BUTTON_UP) == LOW){
      timer[state] += timeincrement[selected];
      printBottomLine();
    }
    else if(timer[state] - timeincrement[selected] >= 0 && digitalRead(BUTTON_DOWN) == LOW){
      timer[state] -= timeincrement[selected];
      printBottomLine();
    }
    else if(selected > 0 && digitalRead(BUTTON_LEFT) == LOW){
      selected -= 1;
      printBottomLine();
    }
    else if(selected < 3 && digitalRead(BUTTON_RIGHT) == LOW){
      selected += 1;
      printBottomLine();
    }
  }
  delay(300);
}

void printBoth(){
  lcd.noCursor();
  printtl();
  printbl();
  setcursor();
  lcd.cursor();
}


void pintTopLine(){
  lcd.noCursor();
  printtl();
  setcursor();
  lcd.cursor();
}

void printBottomLine(){
  lcd.noCursor();
  printtl();
  setcursor();
  lcd.cursor();
}

void printtl(){
  lcd.setCursor(0,0);
  for(byte i=0;i<4;i++){
    if(timer[i] > 0){
      lcd.print('O');
    }
    else{
      lcd.print('*');
    }
  }

}

void printbl(){
  lcd.setCursor(1,1);

  unsigned long time = (millis() / 1000UL) + offset;
  if(state < 5){
    time = timer[state];
  }

  lcd.print("dag ");
  printPad(elapsedDays(time));
  lcd.print('/');
  printPad(numberOfHours(time));
  lcd.print(':'); 
  printPad(numberOfMinutes(time));
  lcd.print(':');
  printPad(numberOfSeconds(time));
}

void setcursor(){
  lcd.setCursor(6+(selected*3),1);
}

void printPad(byte t){
  if(t < 10) lcd.print("0");
  lcd.print(t);
}




