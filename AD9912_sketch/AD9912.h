/* AD9912.h -  SPI interface library for Analog Devices AD9912 DDS Chip
   Written by Chiranth Siddappa
*/

#ifndef AD9912_h
#define AD9912_h

#include <inttypes.h>
#include <Wire.h>

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
#endif
