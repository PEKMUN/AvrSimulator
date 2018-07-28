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
  [0x18] = sub, sub, sub, sub,
  [0x50] = subi, subi, subi, subi, subi, subi, subi, subi,
			subi, subi, subi, subi, subi, subi, subi, subi,
  [0x08] = sbc, sbc, sbc, sbc,
  [0x40] = sbci, sbci, sbci, sbci, sbci, sbci, sbci, sbci,
			sbci, sbci, sbci, sbci, sbci, sbci, sbci, sbci,
  [0x97] = sbiw,
  [0x28] = or,
  [0x60] = ori, ori, ori, ori, ori, ori, ori, ori,
			ori, ori, ori, ori, ori, ori, ori, ori,
  [0x24] = eor, eor, eor, eor,
  //[0x94] = negOrCom, negOrCom,
  
};

//AVR SRAM
uint8_t sram[SRAM_SIZE];

//AVR Register
uint8_t *r = &sram[0];
SregRegister *sreg = (SregRegister*)&sram[0x5f];
uint8_t *sph = &sram[0x5e];
uint8_t *spl = &sram[0x5d];

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

/**
 * Instruction:
 * 		ADC Rd,Rr
 *		0001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int adc(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	
	r[rd] = r[rd] + r[rr] + sreg->C;
  return 0;
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int and(uint8_t *codePtr)
{
  uint8_t rd, rr;
  
  rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
  
  r[rd] = r[rd] & r[rr];
  return 0;
}

/**
 * Instruction:
 * 		ANDI Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *    16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17, 
 *			0010 => 18,
 *			0011 => 19, 
 *			0100 => 20,
 *			0101 => 21, 
 *			0110 => 22,
 *			0111 => 23, 
 *			1000 => 24,
 *			1001 => 25, 
 *			1010 => 26,
 *			1011 => 27, 
 *			1100 => 28,
 *			1101 => 29, 
 *			1110 => 30,
 *			1111 => 31 
 *		}
 */
int andi(uint8_t *codePtr)
{
  uint8_t rd, k;
  
  rd = ((codePtr[0] & 0xf0) >> 4) + 16;
  k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);
 
  r[rd] = r[rd] & k;
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

/**
 * Instruction:
 * 		SUB Rd,Rr
 *		0001 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int sub(uint8_t *codePtr)
{
	uint8_t rd, rr;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - r[rr];
	return 0;
}

/**
 * Instruction:
 * 		SUBI Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17, 
 *			0010 => 18,
 *			0011 => 19, 
 *			0100 => 20,
 *			0101 => 21, 
 *			0110 => 22,
 *			0111 => 23, 
 *			1000 => 24,
 *			1001 => 25, 
 *			1010 => 26,
 *			1011 => 27, 
 *			1100 => 28,
 *			1101 => 29, 
 *			1110 => 30,
 *			1111 => 31 
 *		}
 */
int subi(uint8_t *codePtr)
{
	uint8_t rd, k;
  
	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - k;
	return 0;
}

/**
 * Instruction:
 * 		SBC Rd,Rr
 *		0000 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int sbc(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	
	r[rd] = r[rd] - r[rr] - sreg->C;
	return 0;
}

/**
 * Instruction:
 * 		SBCI Rd, K
 *		0100 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17, 
 *			0010 => 18,
 *			0011 => 19, 
 *			0100 => 20,
 *			0101 => 21, 
 *			0110 => 22,
 *			0111 => 23, 
 *			1000 => 24,
 *			1001 => 25, 
 *			1010 => 26,
 *			1011 => 27, 
 *			1100 => 28,
 *			1101 => 29, 
 *			1110 => 30,
 *			1111 => 31 
 *		}
 */
int sbci(uint8_t *codePtr)
{
	uint8_t rd, k;
	
	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] - k - sreg->C;
	return 0;
}

/**
 * Instruction:
 * 		SBIW Rd + 1:Rd, K
 *		1001 0111 KKdd KKKK
 * where
 *		0 <= KKKKKK <= 63
 * 		dd is {
 *			00 => 24,
 *			01 => 26, 
 *			10 => 28, 
 *			11 => 30
 *		}
 */ 
int sbiw(uint8_t *codePtr)
{
	uint8_t rd, k;
	uint16_t word;
  
	k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
	rd = ((*codePtr & 0x30) >> 3) + 24;

	word = (((uint16_t)r[rd+1]) << 8) | r[rd];
	word -= k;

	r[rd] = word;
	r[rd+1] = (word & 0xff00) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		OR Rd,Rr
 *		0010 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int or(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	
	r[rd] = r[rd] | r[rr];
	return 0;
}

/**
 * Instruction:
 * 		ORI Rd, K
 *		0110 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *      16 <= dddd <= 31
 * 		dddd is {
 *			0000 => 16,
 *			0001 => 17, 
 *			0010 => 18,
 *			0011 => 19, 
 *			0100 => 20,
 *			0101 => 21, 
 *			0110 => 22,
 *			0111 => 23, 
 *			1000 => 24,
 *			1001 => 25, 
 *			1010 => 26,
 *			1011 => 27, 
 *			1100 => 28,
 *			1101 => 29, 
 *			1110 => 30,
 *			1111 => 31 
 *		}
 */
int ori(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] | k;
	return 0;
}

/**
 * Instruction:
 * 		EOR Rd,Rr
 *		0010 01rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int eor(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	
	r[rd] = (~(r[rd]) & r[rr]) | (r[rd] & ~(r[rr]));
	return 0;
}

int itIsCom(uint8_t *codePtr)
{
	uint8_t bitZero;
	
	bitZero = *codePtr & 0x1;
	
	if(bitZero == 0x1)
		return 1;
	else
		return 0;
}

/*int negOrCom(uint8_t *codePtr)
{
	if(itIsCom(codePtr))
		return com(codePtr);
	else
		return neg(codePtr);
}

int com(uint8_t *codePtr)
{
	return 0;
}

int neg(uint8_t *codePtr)
{
	return 0;
}*/