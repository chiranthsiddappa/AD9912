#include <string.h>
#include <Wire.h>
#include <inttypes.h>
#include "LiquidCrystal_I2C.h"


#define I2C3SLA PD1
#define I2C3SCL PD0

#define PD1 15
#define PD0 14


void setup()
{
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Wire.begin();
  Wire.setModule(3);
  pinMode(I2C3SLA, INPUT_PULLUP);
  pinMode(I2C3SCL, INPUT_PULLUP);
  Serial.begin(115200);
  LiquidCrystal_I2C lcd(0x3F,20,4);
  lcd.init();
  lcd.backlight();
  lcd.print("01234567890123456789");
  lcd.setCursor(0,1);
  lcd.print("Line 2");
  lcd.setCursor(0,2);
  lcd.print("Line 3");
  lcd.setCursor(0,3);
  lcd.print("Line 4");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(I2C3SLA) == LOW)
    flash_blue();
  if(digitalRead(I2C3SCL) == LOW)
    flash_red();
  delay(10);
  Serial.print(PD_0, DEC);
  Serial.print(" ");
  Serial.print(RED_LED, DEC);
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
