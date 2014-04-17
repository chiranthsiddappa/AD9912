#include <Wire.h>
#include <string.h>
#include <SPI.h>
#include "AD9912.h"
#include <stdint.h>


#define PB4 0x7
#define SPISCK PB4

#define PB6 23
#define SPIMISO PB6

#define PB7 24
#define SPIMOSI PB7

#define PB5 0x2
#define SPICS PB5

#define PF3 GREEN_LED
#define IO_update PF3

//AD9912 ad9912(SPICS, SPISCK, SPIMOSI, SPIMISO, IO_update);

void setup()
{
  // put your setup code here, to run once:
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.begin(9600);
  //Initialize other DDS pins
  pinMode(IO_update, OUTPUT);
  pinMode(SPISCK, OUTPUT);
  pinMode(SPIMISO, INPUT);
  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPICS, OUTPUT);
  digitalWrite(SPICS, HIGH);
  digitalWrite(SPISCK, LOW);
  digitalWrite(SPIMOSI, LOW);
  //push buttons
  pinMode(PUSH2, INPUT_PULLUP);
  if(ad9912_read_PartID() == 0x1902)
    flash_green();
  else 
    flash_red();
}

uint16_t partID_res;
uint16_t DAC_fsc_read;
uint16_t DAC_fsc_set;
uint32_t FTW_read_msb;
uint32_t FTW_read_lsb;
uint32_t FTW_set_msb;
uint32_t FTW_set_lsb;

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH2) == LOW) {
    delay(20);
    DAC_fsc_set = (DAC_fsc_read == 0x3FF) ? 0x0 : DAC_fsc_read + 0x1;
    //ad9912_DAC_write(DAC_fsc_set);
    DAC_fsc_read = ad9912_DAC_read();
  }
  /*
  Serial.print("DAC FSC: ");
  Serial.print(DAC_fsc_read, HEX);
  Serial.println("");
  Serial.print("FTW: ");
  Serial.println("");
  */
  delay(20);
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

unsigned long long ad9912_read_PartID() {
  uint16_t id;
  unsigned long long instruction = 0x8003;
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  digitalWrite(SPIMOSI, LOW);
  pinMode(SPIMOSI, INPUT);
  id |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 8;
  digitalWrite(SPICS, HIGH);
  delay(0.5);
  instruction = 0x8002;
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPIMOSI, LOW);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  digitalWrite(SPIMOSI, LOW);
  pinMode(SPIMOSI, INPUT);
  id |= shiftIn(SPIMOSI, SPISCK, MSBFIRST);
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  delay(2);
  return id;
}

uint16_t ad9912_DAC_read () {
  uint16_t instruction = 0x0;
  uint16_t data = 0x0;
  instruction |= 0x5 << 13;
  instruction |= 0x40C;
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  digitalWrite(SPIMOSI, LOW);
  pinMode(SPIMOSI, INPUT);
  data |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 8;
  data |= shiftIn(SPIMOSI, SPISCK, MSBFIRST);
  digitalWrite(SPICS, HIGH);
  delay(0.5);
  pinMode(SPIMOSI, OUTPUT);
  return data;
}

void ad9912_DAC_write(uint16_t DAC_val) {
  uint16_t instruction = 0x0;
  instruction |= 0x1 << 13;
  instruction |= 0x40C;
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, DAC_val >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, DAC_val);
  digitalWrite(SPICS, HIGH);
  delay(0.5);
  digitalWrite(SPIMOSI, OUTPUT);
}

void ad9912_FTW_read() {
  uint16_t instruction = 0x0;
  instruction |= 0x7 << 13;
  instruction |= 0x01AB;
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPIMOSI, LOW);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  digitalWrite(SPIMOSI, LOW);
  pinMode(SPIMOSI, INPUT);
  FTW_read_msb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 8;
  FTW_read_msb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST);
  FTW_read_lsb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 24;
  FTW_read_lsb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 16;
  FTW_read_lsb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST) << 8;
  FTW_read_lsb |= shiftIn(SPIMOSI, SPISCK, MSBFIRST);
  digitalWrite(SPICS, HIGH);
  delay(0.5);
  pinMode(SPIMOSI, OUTPUT);
}

void ad9912_instruction(short command, byte bytes, uint32_t data) {
  uint16_t instruction = 0x0;

}

void ad9912_FTW_write(uint32_t FTW_msb, uint32_t FTW_lsb) {
  uint16_t instruction = 0x0;
  instruction |= 0x3 << 13;
  instruction |= 0x01AB;
  digitalWrite(SPICS, HIGH);
  pinMode(SPIMOSI, OUTPUT);
  digitalWrite(SPIMOSI, LOW);
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_msb >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_msb);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_lsb >> 24);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_lsb >> 16);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_lsb >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, FTW_lsb);
  digitalWrite(IO_update, HIGH);
  delay(0.5);
  digitalWrite(IO_update, LOW);
}
