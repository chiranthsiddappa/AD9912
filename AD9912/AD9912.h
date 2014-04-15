/* AD9912.h -  SPI interface library for Analog Devices AD9912 DDS Chip
   Written by Chiranth Siddappa
*/

#ifndef ADD912_h
#include <SPI.h>
#include <Wire.h>
#include <stdint.h>

#define PartID 0x1902

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

class AD9912 {
// global type variables
  uint SPISCK;
  uint SPIMISO;
  uint SPIMOSI;
  uint SPICS;
  uint IO_update;
unsigned char spi_data[4];
unsigned int freqWordArray[2];
unsigned int DAC_fsc;
unsigned int readFreqDAC[3];
unsigned int instruction;
// uint16_t read_PartID();

uint16_t read_PartID() {
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
};
#endif
