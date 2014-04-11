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
#define DAC_fcs_max 0x03FF;

//test pin
#define curr_test_pin 4

// global type variables
unsigned char spi_data[4];
unsigned int freqWordArray[2];
unsigned int DAC_fsc;
unsigned int readFreqDAC[3];
unsigned int instruction;

// setup and loop here
void setup()
{
  // put your setup code here, to run once:
  //setup DDS registers
  DAC_fsc = DAC_fcs_default;
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.begin(9600);
  // Initialize other DDS pins
  pinMode(IO_update, OUTPUT);
  // set pins for SPI bus here
  pinMode(SPISCK, OUTPUT);
  pinMode(SPIMISO, INPUT);
  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPICS, OUTPUT);
  //Initialize SPI bus
  SPI.begin();
  SPI.setModule(2);
  SPI.setClockDivider(32); // 500 kHz 
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //other pin testing
  pinMode(curr_test_pin, INPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  //All done
  flash_green();
}

void loop()
{
  delay(20);
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH1) == LOW) {
    DDS_spi_init();
  }
  else if(digitalRead(PUSH2) == LOW) {
    DDS_spi_read();
    Serial.println("Current Status");
    Serial.print("DAC Read: ");
    Serial.print(readFreqDAC[2], HEX);
    Serial.println();
  }
  else {
    Serial.print("Part ID: ");
    Serial.print(get_part_id(), HEX);
    Serial.println("");
    delay(20);
  }
}

//Custom functions here

void DDS_spi_init() {
  //4 wire command for SPIMISO
  instruction = 0x000099;
  SPI.transfer(instruction >> 16);
  SPI.transfer(instruction >> 8);
  SPI.transfer(instruction);
}

void DDS_spi_read() {
  //use the global readFreqDAC
  instruction &= 0x0;
  instruction |= 0x5 << 29;
  instruction |= DAC_fsc_2 << 16;
  /*  Serial.print("Instruction DAC read: ");
  Serial.print(instruction >> 16, BIN);
  Serial.println("");
  */
  SPI.transfer(instruction >> 24);
  SPI.transfer(instruction >> 16);
  readFreqDAC[2] = SPI.transfer(0x00) << 8;
  readFreqDAC[2] |= SPI.transfer(0x00);
}

unsigned int get_part_id() {
  //Using a 1-byte transfer
  unsigned int id;
  instruction &= 0x0;
  instruction |= 0x4 << 29;
  instruction |= 0x0003 << 16;
  SPI.transfer(instruction >> 24);
  SPI.transfer(instruction >> 16);
  id = SPI.transfer(0x00) << 8;
  instruction &= 0x0;
  instruction |= 0x8002 << 16;
  SPI.transfer(instruction >> 24);
  SPI.transfer(instruction >> 16);
  id |= SPI.transfer(0x00);
  return id;
}

void DDS_spi_write_freq() {
  
}

void DDS_spi_write_DAC(int DAC_out) {

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

void maxInt() {
  unsigned int maxInt = 0;
  Serial.println("\nMax Int: ");
  delay(20);
  maxInt = 0;
  maxInt -= 1;
  Serial.print(maxInt, HEX);
  Serial.println("");
  delay(10);
}

void maxShort() {
  unsigned short maxShort;
  Serial.print("\nMax Short: ");
  delay(20);
  maxShort = 0;
  maxShort -= 1;
  Serial.print(maxShort, HEX);
  Serial.println("");
  delay(10);
}
