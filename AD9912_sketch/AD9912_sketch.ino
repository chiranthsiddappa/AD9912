#include <Wire.h>
#include <string.h>
#include <SPI.h>
#include <stdint.h>
#include <inttypes.h>
#include "AD9912.h"
#include "LiquidCrystal_I2C.h"

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

#define I2C2SLA 15
#define I2C2SCL 14

AD9912 ad9912;
LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup()
{
  /*
  pinMode(I2C2SLA, INPUT_PULLUP);
  pinMode(I2C2SCL, INPUT_PULLUP);
  Wire.begin();
  Wire.setModule(3);
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  */
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
  ad9912.init(SPICS, SPISCK, SPIMOSI, SPIMISO, IO_update, 1000000000);
  //  lcd.clearRow(0);
  //  lcd.print("AD9912 Module Init");
  //push buttons
  pinMode(PUSH2, INPUT_PULLUP);
  if(ad9912.read_PartID() == 0x1902) {
    //lcd.print("Chip Found");
    flash_green();
  }
  else {
    //lcd.print("Chip Not Found");
    flash_red();
  }
}

uint16_t partID_res;
uint16_t DAC_fsc_read;
uint16_t DAC_fsc_set;
uint32_t FTW_read_msb;
uint32_t FTW_read_lsb;
uint64_t FTW_read = 0x000ULL;
uint64_t FTW_set = 0x8FFFFFFFFFFFULL;

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH2) == LOW) {
    //    ad9912.setFrequency(190440000);
    ad9912.read_PartID();
  }
  delay(5);
  Serial.print(PB_7, HEX);
  Serial.println("");
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
