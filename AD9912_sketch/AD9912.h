/* AD9912.h -  SPI interface library for Analog Devices AD9912 DDS Chip
   Written by Chiranth Siddappa
*/

#ifndef AD9912_h
#define AD9912_h

#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>

#define ad9912_PartID 0x1902
#define ad9912_read 0x1
#define ad9912_write 0x0

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
  void init(uint SPICS, uint SPISCK, uint SPIMOSI, uint SPIMISO, uint IO_update, uint32_t clkFreq);
  uint16_t read_PartID();
  uint64_t instruction(short command, uint16_t address, char bytes, uint64_t data);
  uint16_t DAC_read();
  void DAC_write(uint16_t DAC_val);
  uint64_t FTW_read();
  void FTW_write(uint64_t FTW);
  void setFrequency(uint32_t frequency);
  uint32_t getFrequency();
  void updateClkFreq(uint64_t clkFreq);
 private:
  // global type variables
  uint _SPISCK;
  uint _SPIMISO;
  uint _SPIMOSI;
  uint _SPICS;
  uint _IO_update;
  uint32_t _fs;
};
#endif
