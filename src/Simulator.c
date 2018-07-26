#include <stdio.h>
#include <stdint.h>
#include "Simulator.h"

AvrOperator AvrOperatorTable[256] = {
  [0xc]  = add, add, add, add,
  [0x1c] = adc, adc, adc, adc,
  [0x20] = and, and, and, and,
  [0x70] = andi, andi, andi, andi, andi, andi, andi, andi,
           andi, andi, andi, andi, andi, andi, andi, andi, 
  [0x96] = adiw,
};

//AVR Register
uint8_t r[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/**
 * Instruction:
 * 		ADD Rd,Rr
 *		0000 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int add(uint8_t *codePtr)
{
  uint8_t rd, rr;
  
  rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
  
  r[rd] = r[rd] + r[rr];
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

/**
 * Instruction:
 * 		ADIW Rd + 1:Rd, K
 *		1001 0110 KKdd KKKK
 * where
 *		0 <= KKKKKK <= 63
 * 		dd is {
 *			00 => 24,
 *			01 => 26, 
 *			10 => 28, 
 *			11 => 30
 *		}
 */
int adiw(uint8_t *codePtr)
{
  uint8_t rd, k;
  uint16_t word;
  
  k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
  rd = ((*codePtr & 0x30) >> 3) + 24;
 
  word = (((uint16_t)r[rd+1]) << 8) | r[rd];
  word += k;

  r[rd] = word;
  r[rd+1] = (word & 0xff00) >> 8;

  return 0;
}