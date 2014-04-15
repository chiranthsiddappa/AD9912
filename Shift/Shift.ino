#include <Wire.h>
#include <string.h>
#include <SPI.h>

#define PartID 0x1902

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

//DDS Register Addresses
//LSB
#define FTW0_1 0x01A6
#define FTW0_2 0x01A7
#define FTW0_3 0x01A8
#define FTW0_4 0x01A9
//MSB
#define FTW0_start_1 0x01AA
#define FTW0_start_2 0x01AB
#define DAC_fsc_1 0x040B
#define DAC_fsc_2 0x040C

//DDS Default values and maxes
#define DAC_fcs_default 0x01FF
#define DAC_fcs_max 0x03FF

// global type variables
unsigned char spi_data[4];
unsigned int freqWordArray[2];
unsigned int DAC_fsc;
unsigned int readFreqDAC[3];
unsigned int instruction;

void setup()
{
  // put your setup code here, to run once:
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
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

unsigned int partID_res;

void loop()
{
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH2) == LOW) {
    partID_res = read_PartID();
    delay(20);
  }
  Serial.print("Part ID: ");
  Serial.print(partID_res, HEX);
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

unsigned int read_PartID() {
  unsigned int id;
  instruction = 0x8003;
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
  digitalWrite(SPICS, LOW);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(SPIMOSI, SPISCK, MSBFIRST, instruction);
  digitalWrite(SPIMOSI, LOW);
  pinMode(SPIMOSI, INPUT);
  id |= shiftIn(SPIMOSI, SPISCK, MSBFIRST);
  digitalWrite(SPICS, HIGH);
  delay(2);
  return id;
}
