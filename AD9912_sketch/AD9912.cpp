/* AD9912.h -  SPI interface library for Analog Devices AD9912 DDS Chip
   Written by Chiranth Siddappa
*/


#include "AD9912.h"
#include "Arduino.h"
#include <stdint.h>
#include <inttypes.h>
#include <SPI.h>

void AD9912::init(uint SPICS, uint SPISCK, uint SPIMOSI, uint SPIMISO, uint IO_update) {
  _SPICS = SPICS;
  _SPISCK = SPISCK;
  _SPIMOSI = SPIMOSI;
  _SPIMISO = SPIMISO;
  _IO_update = IO_update;
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

uint64_t AD9912::instruction(short command, uint16_t address, char bytes, uint64_t data) {
  uint64_t instruction = 0x0;
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
