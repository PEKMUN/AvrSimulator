#include <stdio.h>
#include <stdint.h>
#include "Simulator.h"

AvrOperator AvrOperatorTable[256] = {
  [0xc] = add, add, add, add,
  [0x1c] = adc, adc, adc, adc,
  [0x20] = and, and, and, and,
  [0x70] = andi, andi, andi, andi, andi, andi, andi, andi,
           andi, andi, andi, andi, andi, andi, andi, andi, 
  [0x96] = adiw,
};

//AVR Register
uint8_t r[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int add(uint8_t *codePtr)
{
  return 0;
}

int adc(uint8_t *codePtr)
{
  return 0;
}

int and(uint8_t *codePtr)
{
  return 0;
}

int andi(uint8_t *codePtr)
{
  return 0;
}

int adiw(uint8_t *codePtr)
{
  return 0;
}