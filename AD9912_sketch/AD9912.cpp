/* AD9912.h -  SPI interface library for Analog Devices AD9912 DDS Chip
   Written by Chiranth Siddappa
*/


#include "AD9912.h"
#include "Arduino.h"
#include <stdint.h>
#include <inttypes.h>
#include <SPI.h>

void AD9912::init(uint SPICS, uint SPISCK, uint SPIMOSI, uint SPIMISO, uint IO_update, uint64_t fs) {
  _SPICS = SPICS;
  _SPISCK = SPISCK;
  _SPIMOSI = SPIMOSI;
  _SPIMISO = SPIMISO;
  _IO_update = IO_update;
  _fs = fs;
}

uint16_t AD9912::read_PartID() {
  uint16_t id;
  id = AD9912::instruction(0x1, 0x3, 2, 0x0);
  return id;
}

uint64_t AD9912::instruction(short command, uint16_t address, char bytes, uint64_t data) {
  uint16_t instruction = 0x0;
  uint64_t return_data = 0x0;
  short nthByte;
  uint32_t shiftAmount;
  if(bytes > 3)
    instruction |= 3 << 13;
  else if(bytes > 0)
    instruction |= (bytes - 1) << 13;
  else
    return 0;
  instruction |= command << 15;
  instruction |= address & 0x7FF;
  Serial.print("Instruction: ");
  Serial.print(instruction, BIN);
  Serial.println();
  delay(100);
  pinMode(_SPIMOSI, OUTPUT);
  digitalWrite(_SPICS, HIGH);
  digitalWrite(_SPICS, LOW);
  shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, instruction >> 8);
  if(command & 0x1) {
    digitalWrite(_SPIMOSI, LOW);
    pinMode(_SPIMOSI, INPUT);
    for(nthByte = 0, shiftAmount = (bytes - 1)*8; nthByte < bytes; nthByte++, shiftAmount -= 8) 
      return_data |= (uint64_t) shiftIn(_SPIMOSI, _SPISCK, MSBFIRST) << shiftAmount;
  }
  else {
    for(nthByte = 0, shiftAmount = (bytes - 1)*8; nthByte < bytes; nthByte++, shiftAmount -= 8)
      shiftOut(_SPIMOSI, _SPISCK, MSBFIRST, data >> shiftAmount);
    return_data = 0x0;
  }
  digitalWrite(_SPICS, HIGH);
  pinMode(_SPIMOSI, OUTPUT);
  return return_data;
}

uint16_t AD9912::DAC_read() {
  uint16_t data;
  data = AD9912::instruction(0x1, 0x40C, 2, 0x0);
  return data;
}

void AD9912::DAC_write(uint16_t DAC_val) {
  AD9912::instruction(0x0, 0x40C, 2, DAC_val);
}

uint64_t AD9912::FTW_read() {
  uint64_t FTW;
  FTW = AD9912::instruction(0x1, 0x1AB, 6, 0x0);
  return FTW;
}

void AD9912::FTW_write(uint64_t FTW) {
  AD9912::instruction(0x0, 0x1AB, 6, FTW);
}

void AD9912::setFrequency(uint64_t fDDS) {
  uint64_t FTW;
  FTW = (uint64_t) (281474976710656 * (fDDS / (double) _fs));
  AD9912::FTW_write(FTW);
}

void AD9912::updateClkFreq(uint64_t fs) {
  _fs = fs;
}
