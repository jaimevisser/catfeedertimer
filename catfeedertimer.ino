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

const unsigned long TIMER_MAX = (9 * SECS_PER_DAY)+(23 * SECS_PER_HOUR)+(60 * SECS_PER_MIN);
const byte STATE_SELECT = 5;
const byte STATE_CLOCK  = 4;
const byte LAST_STATE   = 5;

byte state = 0;
byte selected = 0;

const byte CHAR_ARROW_DOWN = 0;
byte CHAR_ARROW_DOWN_DATA[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100
};

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

  lcd.createChar(0, CHAR_ARROW_DOWN_DATA);

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
  if(selected < 3){
    if(state < 4){
      if((timer[state] + timeincrement[selected]) < TIMER_MAX){
        timer[state] += timeincrement[selected];
        printBottomLine();
      }
    }
    else if(state == 4){
      offset += timeincrement[selected];
      printBottomLine();
    }
  }
  else if(state > 0){
    state--;
    printBoth();
  }
  else if(state == 0){
    state=LAST_STATE;
    printBoth();
  }
}

void onDown(){
  if(selected < 3){
    if(state < 4){
      if(timer[state] >= timeincrement[selected]){
        timer[state] -= timeincrement[selected];
        printBottomLine();
      }
    }
    else if(state == 4){
      if(offset >= timeincrement[selected]){
        offset -= timeincrement[selected];
        printBottomLine();
      }
    }
  }
  else if(state < LAST_STATE){
    state++;
    printBoth();
  }
  else if(state == LAST_STATE){
    state = 0;
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
  if(selected < 3){
    selected += 1;
    printBottomLine();
  }
}

void printBoth(){
  lcd.noCursor();
  lcd.clear();
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
  if(state < 5){
    if(state < 4){
      lcd.print("Inst. Vak ");
      lcd.print(state+1);
      lcd.setCursor(15,0);
      lcd.print("^");
    }
    else if(state == 4){
      lcd.print("Inst. Klok");
      lcd.setCursor(15,0);
      lcd.print("^");
    }
  }
  else{
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
  }


}

void printbl(){
  lcd.setCursor(0,1);

  unsigned long time; 
  if(state < 4){
    time = timer[state];
  }
  else{
    time = (millis() / 1000UL) + offset;
  }

  lcd.print("d");
  lcd.print(elapsedDays(time));
  lcd.print('/');
  printPad(numberOfHours(time));
  lcd.print(':'); 
  printPad(numberOfMinutes(time));
  if(state > 4){
    lcd.print(':');
    printPad(numberOfSeconds(time));
  }

  lcd.setCursor(15,1);
  lcd.write(CHAR_ARROW_DOWN);
}

void setcursor(){
  if(state < 5){
    if(selected < 3){
      lcd.setCursor(1+(selected*3),1);
    }
    else{
      lcd.setCursor(15,1);
    }
  }

  if(state < 6){
    lcd.cursor();
  }
}

void printPad(byte t){
  if(t < 10) lcd.print("0");
  lcd.print(t);
}





















