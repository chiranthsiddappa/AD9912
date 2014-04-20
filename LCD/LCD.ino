#include <string.h>
#include <Wire.h>
#include <inttypes.h>
#include "LiquidCrystal_I2C.h"


#define I2C2SLA PE5
#define I2C2SCL PE4

#define PE5 6
#define PE4 5

LiquidCrystal_I2C lcd(0x3F,20,4);

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(0x30003, INPUT_PULLUP);
  /*
  Wire.begin();
  Wire.setModule(3);
  lcd.init();
  lcd.backlight();
  */
  /*
  lcd.print("01234567890123456789");
  lcd.setCursor(0,1);
  lcd.print("Line 2");
  lcd.setCursor(0,2);
  lcd.print("Line 3");
  lcd.setCursor(0,3);
  lcd.print("Line 4");
  digitalWrite(PA_6, HIGH);
  digitalWrite(PA_7, HIGH);
  */
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(I2C2SLA) == LOW)
    //flash_blue();
  if(digitalRead(I2C2SCL) == LOW)
    //flash_red();
  delay(10);
  Serial.print(PD_0, HEX);
  Serial.print(" ");
  Serial.print(PD_1, HEX);
  Serial.println();
}

void flash_blue() {
  digitalWrite(BLUE_LED, LOW);
  delay(20);
  digitalWrite(BLUE_LED, HIGH);
  delay(20);
  digitalWrite(BLUE_LED, LOW);
}

void flash_red() {
  digitalWrite(RED_LED, LOW);
  delay(20);
  digitalWrite(RED_LED, HIGH);
  delay(20);
  digitalWrite(RED_LED, LOW);
}

void flash_green() {
  digitalWrite(GREEN_LED, LOW);
  delay(20);
  digitalWrite(GREEN_LED, HIGH);
  delay(20);
  digitalWrite(GREEN_LED, LOW);
}
