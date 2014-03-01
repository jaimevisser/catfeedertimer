#include <LiquidCrystal.h>

const byte BUTTON_UP    = 2;
const byte BUTTON_DOWN  = 4;
const byte BUTTON_LEFT  = 3;
const byte BUTTON_RIGHT = 5;

const byte BUTTONS[] = {BUTTON_UP,BUTTON_DOWN,BUTTON_LEFT,BUTTON_RIGHT};

const char* BUTTON_NAMES[] = {"UP   ","DOWN ","LEFT ","RIGHT"};

const byte LCD_RS     = 12;
const byte LCD_ENABLE = 11;
const byte LCD_D4     = 10;
const byte LCD_D5     =  9;
const byte LCD_D6     =  8;
const byte LCD_D7     =  7;

const byte LCD_COLS = 16;
const byte LCD_ROWS =  2;

LiquidCrystal lcd(
  LCD_RS,
  LCD_ENABLE,
  LCD_D4,
  LCD_D5,
  LCD_D6,
  LCD_D7
);

void setup()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
}

void loop() {
  lcd.setCursor(0,1);
  for(byte i=0;i<4;i++){
    if(digitalRead(BUTTONS[i]) == LOW)
      lcd.write(BUTTON_NAMES[i]);
  }
  delay(200);
}
