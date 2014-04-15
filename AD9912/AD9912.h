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
 public:
  AD9912(uint SPI_CS, uint SPI_SCK, uint SPI_MOSI, uint SPI_MISO, uint DDS_IO_update);
  uint16_t read_PartID();
 private:
  // global type variables
  uint _SPISCK;
  uint _SPIMISO;
  uint _SPIMOSI;
  uint _SPICS;
  uint IO_update;
  unsigned char spi_data[4];
  unsigned int freqWordArray[2];
  unsigned int DAC_fsc;
  unsigned int readFreqDAC[3];
  unsigned int instruction;
  // uint16_t read_PartID();

};

AD9912::AD9912(uint SPI_CS, uint SPI_SCK, uint SPI_MOSI, uint SPI_MISO, uint DDS_IO_update) {
  _SPICS = SPI_CS;
  pinMode(SPI_CS, OUTPUT);
  _SPISCK = SPI_SCK;
  pinMode(SPI_SCK, OUTPUT);
  _SPIMOSI = SPI_MOSI;
  pinMode(SPI_MOSI, OUTPUT);
  _SPIMISO = SPI_MISO;
  pinMode(SPI_MISO, INPUT);
  digitalWrite(_SPICS, HIGH);
  digitalWrite(_SPISCK, LOW);
  digitalWrite(_SPIMOSI, LOW);
}

uint16_t AD9912::read_PartID() {
  uint16_t id;
  uint32_t instruction = 0x8003;
  digitalWrite(_SPICS, HIGH);
  pinMode(_SPIMOSI, OUTPUT);
  digitalWrite(_SPICS, LOW);
  shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, instruction);
  digitalWrite(_SPIMOSI, LOW);
  pinMode(_SPIMOSI, INPUT);
  id |= shiftIn(_SPIMOSI, _SPISCK, MSBFIRST) << 8;
  digitalWrite(_SPICS, HIGH);
  delay(0.5);
  instruction = 0x8002;
  pinMode(_SPIMOSI, OUTPUT);
  digitalWrite(_SPIMOSI, LOW);
  digitalWrite(_SPICS, LOW);
  shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, instruction >> 8);
  shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, instruction);
  digitalWrite(_SPIMOSI, LOW);
  pinMode(_SPIMOSI, INPUT);
  id |= shiftIn(_SPIMOSI, _SPISCK, MSBFIRST);
  digitalWrite(_SPICS, HIGH);
  delay(2);
  return id;
}
#endif
