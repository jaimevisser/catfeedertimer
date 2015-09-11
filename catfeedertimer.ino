#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>

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

/* LCD pin assignments */
const byte LCD_RS       =  2;
const byte LCD_ENABLE   =  3;
const byte LCD_D4       =  7;
const byte LCD_D5       =  8;
const byte LCD_D6       =  9;
const byte LCD_D7       = 10;

/* Servo pin assignments */
const byte SERVO        =  6;

/* Button pin assignments */
const byte BUTTON_RIGHT = A1;
const byte BUTTON_DOWN  = 12;
const byte BUTTON_LEFT  = 11;
const byte BUTTON_UP    = A0;

/* Servo positions */
// BOARD 1 */

const byte STEP[4] = {
32,
42,
52,
65
};

const byte NEUTRAL = 2;

// BOARD 2 */
/*
const byte STEP[4] = {
36,
49,
63,
78
};

const byte NEUTRAL = 2;

/* LCD constants */
const byte LCD_COLS = 16;
const byte LCD_ROWS =  2;

/* Offset at start time */
unsigned long offset = 00 + (18*SECS_PER_HOUR);

/* Last interaction */
unsigned long lastInteraction = 0;
const unsigned long TIMEOUT = (1000UL) * (SECS_PER_MIN * 5);

/* Timer vars with initial values*/
unsigned long timer[4] = {
  (12*SECS_PER_HOUR),
  (15*SECS_PER_HOUR),
  (12*SECS_PER_HOUR)+(1*SECS_PER_DAY),
  (15*SECS_PER_HOUR)+(1*SECS_PER_DAY)};

const unsigned long TIMER_MAX = (9 * SECS_PER_DAY)+(23 * SECS_PER_HOUR)+(60 * SECS_PER_MIN);

const byte LAST_STATE   = 5;

/* State of the software */
byte state = 4;

/* Selected edit field */
byte selected = 3;

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
  servo.write(NEUTRAL);
  servo.detach();

  //save();
  EEPROM.get(0, timer);

  for(byte i=0;i<4;i++){
    timer[i] = timer[i] % SECS_PER_DAY;
  }

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  printLCD();
  
  lastInteraction = millis();
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
  if(state == LAST_STATE){
    lcd.setCursor(2,1);
    printTime((millis() / 1000UL) + offset);
    checktimers();
  }
  if((lastInteraction > 0) && ((lastInteraction + TIMEOUT) < millis())){
    lastInteraction = 0;
    state = 5;
    selected = 3;
    printLCD();
    save();
  }
}

void checktimers(){
  for(byte i=0;i<4;i++){
    if((timer[i] > 0) && (timer[i] < ((millis() / 1000UL) + offset))){
      timer[i] += SECS_PER_DAY;
      opentimer(i);
      printLCD();
    }
  }
}

void opentimer(byte timer){
  servo.attach(SERVO);
  servo.write(NEUTRAL);
  
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(0,0);
  
  lcd.print("Openen vak ");
  lcd.print(timer+1);
  
  servo.write(STEP[timer]);
  delay(2000);
  servo.write(NEUTRAL);
  delay(2000);
  
  servo.detach();
}

void afterButton(){
  lastInteraction = millis();
  delay(250);
}

void onUp(){
  if(selected < 3){
    if(state < 4){
      if((timer[state] + timeincrement[selected]) < TIMER_MAX){
        timer[state] += timeincrement[selected];
        printLCD();
      }
    }
    else if(state == 4){
      offset += timeincrement[selected];
      printLCD();
    }
  }
  else if(state > 0){
    state--;
    printLCD();
  }
  else if(state == 0){
    state=LAST_STATE;
    printLCD();
  }

  if(state == LAST_STATE){
    save();
  }
}

void onDown(){
  if(selected < 3){
    if(state < 4){
      if(timer[state] >= timeincrement[selected]){
        timer[state] -= timeincrement[selected];
        printLCD();
      }
    }
    else if(state == 4){
      if(offset >= timeincrement[selected]){
        offset -= timeincrement[selected];
        printLCD();
      }
    }
  }
  else if(state < LAST_STATE){
    state++;
    printLCD();
  }
  else if(state == LAST_STATE){
    state = 0;
    printLCD();
  }

  if(state == LAST_STATE){
    save();
  }
}

void onLeft(){
  byte minSelected = state == LAST_STATE ? 3 : 0;
  if(selected > minSelected){
    selected -= 1;
    printLCD();
  }else if(selected == minSelected){
    selected = 3;
    printLCD();
  }
}

void onRight(){
  byte minSelected = state == LAST_STATE ? 3 : 0;
  if(selected < 3){
    selected += 1;
    printLCD();
  }
  else if(selected == 3){
    selected = minSelected;
    printLCD();
  }
}

void printLCD(){
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(0,0);
  unsigned long time;

  if(state < 5){
    if(state < 4){
      lcd.print("Inst. Vak ");
      lcd.print(state+1);
    }
    else if(state == 4){
      lcd.print("Inst. Klok");
    }
  }
  else{
    for(byte i=0;i<4;i++){
      if(timer[i] > 0){
        lcd.print(i+1);
      }
      else{
        lcd.print('.');
      }
    }

    time = nextTimer();
    if(time > 0){
      lcd.print(" > ");
      printTimeMins(time);
    }
  }

  lcd.setCursor(2,1);
  time = (millis() / 1000UL) + offset;
  if(state < 4){
    time = timer[state];
  }

  if(state == LAST_STATE){
    printTime(time);
  }
  else{
    printTimeMins(time);
  }

  if(selected == 3){
    lcd.setCursor(15,0);
    lcd.print("^");
    lcd.setCursor(15,1);
    lcd.write(CHAR_ARROW_DOWN);
  }

  setcursor();
}

void printTime(unsigned long time){
  printTimeMins(time);
  lcd.print(':');
  printPad(numberOfSeconds(time));
}

void printTimeMins(unsigned long time){
  lcd.print("d");
  lcd.print(elapsedDays(time));
  lcd.print('/');
  printPad(numberOfHours(time));
  lcd.print(':'); 
  printPad(numberOfMinutes(time));
}

unsigned long nextTimer(){
  unsigned long next = 0;
  for(byte i=0;i<4;i++){
    if(timer[i] > 0){
      next = max(timer[i], next);
    }
  }
  for(byte i=0;i<4;i++){
    if(timer[i] < next && timer[i] > 0){
      next = timer[i];
    }
  }
  return next;
}

void setcursor(){
  if(state < LAST_STATE){
    if(selected < 3){
      lcd.setCursor(3+(selected*3),1);
      lcd.cursor();
    }
  }
}

void printPad(byte t){
  if(t < 10) lcd.print("0");
  lcd.print(t);
}

void save(){
  EEPROM.put(0,timer);
}






