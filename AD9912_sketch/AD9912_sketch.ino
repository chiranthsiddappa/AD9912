#include <Wire.h>
#include <stdint.h>
#include <inttypes.h>
#include <SPI.h>
#include "AD9912.h"
#include "LiquidCrystal_I2C.h"
#include <math.h>

#define PB4 0x7
#define SPISCK PB4

#define PB6 23
#define SPIMISO PB6

#define PB7 0xF
#define SPIMOSI PB7

#define PB5 0x2
#define SPICS PB5

#define PF3 GREEN_LED
#define IO_update PF3

#define PD0 0x17
#define PD1 0x18
#define I2C2SLA PD1
#define I2C2SCL PD0

AD9912 ad9912;
LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup()
{
  pinMode(I2C2SLA, INPUT_PULLUP);
  pinMode(I2C2SCL, INPUT_PULLUP);
  Wire.begin();
  Wire.setModule(3);
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  // put your setup code here, to run once:
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.begin(9600);
  //Initialize other DDS pins
  pinMode(IO_update, OUTPUT);
  pinMode(SPISCK, OUTPUT);
  //  pinMode(SPIMISO, INPUT);
  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPICS, OUTPUT);
  digitalWrite(SPICS, HIGH);
  digitalWrite(SPISCK, LOW);
  digitalWrite(SPIMOSI, LOW);
  ad9912.init(SPICS, SPISCK, SPIMOSI, SPIMISO, IO_update, 1000000000, 10000);
  lcd.clearRow(0);
  lcd.print("AD9912 Module Init");
  //push buttons
  pinMode(PUSH2, INPUT_PULLUP);
  lcd.setCursor(0,1);
  if(ad9912.read_PartID() == 0x1902) {
    lcd.print("Chip Found");
    flash_green();
  }
  else {
    lcd.print("Chip Not Found");
    flash_red();
  }
  ad9912.setCurrent(0.0316);
  ad9912.DAC_write(0x0);
  Serial.begin(9600);
}

uint16_t partID_res;
uint16_t DAC_fsc_read;
uint16_t DAC_fsc_set;
uint32_t userFreq = 180000000;
uint32_t freqCursPos = 0;
uint32_t DACCursPos = 0;

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH2) == LOW) {
    ad9912.setFrequency(userFreq);
    LCDDisplayCurrentSettings();
    userFreq += 1000000;
    delay(100);
  }
  delay(5);
  dispDACCurs();
}

void flash_green() {
  digitalWrite(GREEN_LED, LOW);
  delay(20);
  digitalWrite(GREEN_LED, HIGH);
  delay(20);
  digitalWrite(GREEN_LED, LOW);
}

void flash_red() {
  digitalWrite(RED_LED, LOW);
  delay(20);
  digitalWrite(RED_LED, HIGH);
  delay(20);
  digitalWrite(RED_LED, LOW);
}

uint ad9912_frequency_sweep() {
  uint64_t orig_FTW = ad9912.FTW_read();
  uint32_t FTW_set;
  for(FTW_set = 0x0; FTW_set <= 0xFFFFFFFFFFEul; FTW_set += 536870912l) {
    ad9912.FTW_write(FTW_set);
    if(digitalRead(PUSH2) == LOW) {
      ad9912.FTW_write(orig_FTW);
      return 0;
    }
  }
  ad9912.FTW_write(orig_FTW);
  return 1;
}

void LCD_clear_row(short row) {
  for(int i = 1; i <= 20; i++) {
    lcd.setCursor(row, i);
    lcd.print(" ");
  }
}

void LCDDisplayCurrentSettings() {
  uint8_t i;
  uint32_t freq = ad9912.getFrequency();
  float current = ad9912.getCurrent() * 1000;
  uint32_t DAC = ad9912.DAC_read();
  lcd.noCursor();
  lcd.noBlink();
  for(i = 0; i < 3; i++) {
    lcd.clearRow(i);
  }
  lcd.setCursor(0,0);
  lcd.print("Freq: ");
  for(i = 0; i < 8 - length(freq); i++) {
    lcd.print(" ");
  }
  lcd.print(freq);
  lcd.print("Hz");
  lcd.setCursor(0,1);
  lcd.print("DAC: ");
  for(i = 0; i < 3 - length(DAC); i++) {
    lcd.print(" ");
  }
  lcd.print(DAC, HEX);
  lcd.setCursor(0,2);
  lcd.print("Curr: ");
  for(i = 0; i < 2 - length(current); i++) {
    lcd.print(" ");
  }
  lcd.print(current, 4);
  lcd.print("mA");
}

uint32_t length(uint64_t x) {
  float flength = floor(log10(abs(x))) + 1;
  return (uint32_t) flength;
}

void printUserFreq() {
  uint8_t i;
  lcd.noCursor();
  lcd.clearRow(0);
  lcd.setCursor(0,0);
  lcd.print("Freq: ");
  for(i = 0; i < 8 - length(userFreq); i++) {
    lcd.print(" ");
  }
  lcd.print("Hz");
}

void dispFreqCurs() {
  lcd.setCursor(14 - freqCursPos, 0);
  lcd.cursor();
}

void dispDACCurs() {
  lcd.setCursor(8 - DACCursPos, 1);
  lcd.cursor();
}
