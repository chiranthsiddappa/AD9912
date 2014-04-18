#include "AD9912.h"
#include "Arduino.h"
#include <stdint.h>
#include <inttypes.h>
#include <SPI.h>

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
