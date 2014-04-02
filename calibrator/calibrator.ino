#include <Servo.h>
#include <LiquidCrystal.h>

/* Servo pin assignments */
const byte SERVO        =  6;

/* LCD pin assignments */
const byte LCD_RS       =  2;
const byte LCD_ENABLE   =  3;
const byte LCD_D4       =  7;
const byte LCD_D5       =  8;
const byte LCD_D6       =  9;
const byte LCD_D7       = 10;

/* LCD constants */
const byte LCD_COLS = 16;
const byte LCD_ROWS =  2;

/* Button pin assignments */
const byte BUTTON_RIGHT = A1;
const byte BUTTON_DOWN  = 12;
const byte BUTTON_LEFT  = 11;
const byte BUTTON_UP    = A0;

LiquidCrystal lcd(
LCD_RS,
LCD_ENABLE,
LCD_D4,
LCD_D5,
LCD_D6,
LCD_D7
);

Servo servo;

// BOARD 1
/*
const byte STEP[4] = {
32,
42,
52,
65
};

const byte NEUTRAL = 2;*/

// BOARD 2
const byte STEP[4] = {
36,
49,
63,
78
};

const byte NEUTRAL = 2;

int pos = NEUTRAL;    // variable to store the servo position
boolean changed = false;

void setup()
{
  servo.attach(SERVO);
  servo.write(pos);

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  
  for(byte i=0; i< 4;i++){
    delay(2000);
    servo.write(STEP[i]);
    delay(500);
    servo.write(NEUTRAL);
    delay(200);
  }
  servo.detach();
}

void loop() 
{ 

  if(digitalRead(BUTTON_UP) == LOW){
    pos--;
    changed = true;
  }

  if(digitalRead(BUTTON_DOWN) == LOW){
    pos++;
    changed = true;
  }
  
  if(digitalRead(BUTTON_LEFT) == LOW){
    pos-= 10;
    changed = true;
  }

  if(digitalRead(BUTTON_RIGHT) == LOW){
    pos+= 10;
    changed = true;
  }

  if(changed){
    changed = false;
    servo.attach(SERVO);
    lcd.clear();
    lcd.noCursor();
    lcd.setCursor(0,0);
    lcd.print(pos);
    
    servo.write(pos);
    
    delay(250);
    servo.detach();
  }

}
