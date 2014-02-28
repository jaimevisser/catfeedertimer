#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
int buttonUp = 2;
int buttonDown = 4;
int buttonLeft = 3;
int buttonRight = 5;

void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);
}

void loop() {
  lcd.setCursor(0,1);
  if(digitalRead(buttonUp) == LOW){
    lcd.write("Up   ");
  }
  if(digitalRead(buttonDown) == LOW){
    lcd.write("Down ");
  }
  if(digitalRead(buttonLeft) == LOW){
    lcd.write("Left ");
  }
  if(digitalRead(buttonRight) == LOW){
    lcd.write("Right");
  }
  delay(200);
}
