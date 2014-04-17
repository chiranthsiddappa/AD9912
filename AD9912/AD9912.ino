#include <Wire.h>
#include <string.h>
#include <SPI.h>
//#include "AD9912.h"


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
  flash_green();
}

uint16_t partID_res;
uint16_t DAC_fsc_read;

void loop()
{
  // put your main code here, to run repeatedly:
  /*
  if(digitalRead(PUSH2) == LOW) {
    partID_res = ad9912_read_PartID();
    delay(20);
  }
  Serial.print("Part ID: ");
  Serial.print(partID_res, HEX);
  Serial.println("");
  delay(20);
  */
  if(digitalRead(PUSH2) == LOW) {
    DAC_fsc_read = ad9912_DAC_read();
    ad9912_read_PartID();
    delay(20);
  }
  Serial.print("DAC FSC: ");
  Serial.print(DAC_fsc_read, HEX);
  Serial.println("");
  delay(20);
}

void flash_green() {
  digitalWrite(GREEN_LED, LOW);
  delay(20);
  digitalWrite(GREEN_LED, HIGH);
  delay(20);
  digitalWrite(GREEN_LED, LOW);
}

uint16_t ad9912_read_PartID() {
  uint16_t id;
  uint32_t instruction = 0x8003;
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
  digitalWrite(SPIMOSI, OUTPUT);
  return data;
}
