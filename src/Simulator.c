#include <stdio.h>
#include <stdint.h>
#include "Simulator.h"

AvrOperator AvrOperatorTable[256] = {
  [0x0c]  = add, add, add, add,
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
  [0xef] = ser,
  [0x9c] = mul, mul, mul, mul,
  [0x02] = muls,
  [0x94] = lsr, lsr,
  [0xfa] = bst, bst,
  [0xf8] = bld, bld,
  [0x03] = mulsu,
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

/**
 * Instruction:
 * 		COM Rd
 *		1001 010d dddd 0000
 * where
 *		0 <= ddddd <= 31
 */
int com(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = 0xff - r[rd];
	return 0;
}

/**
 * Instruction:
 * 		NEG Rd
 *		1001 010d dddd 0001
 * where
 *		0 <= ddddd <= 31
 */
int neg(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = 0x00 - r[rd];
	return 0;
}

/**
 * Instruction:
 * 		DEC Rd
 *		1001 010d dddd 1010
 * where
 *		0 <= ddddd <= 31
 */
int dec(uint8_t *codePtr)
{
	uint8_t rd;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	r[rd] = r[rd] - 1;
	return 0;
}

/**
 * Instruction:
 * 		INC Rd
 *		1001 010d dddd 0011
 * where
 *		0 <= ddddd <= 31
 */
int inc(uint8_t *codePtr)
{
	uint8_t rd;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	r[rd] = r[rd] + 1;
	return 0;
} 

/**
 * Instruction:
 * 		CBR Rd, K
 *		0111 KKKK dddd KKKK
 * where
 *		0 <= KKKKKKKK <= 255
 *     16 <= dddd <= 31
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
int cbr(uint8_t *codePtr)
{
	uint8_t rd, k;
  
	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] & (0xff - k);
	return 0;
}

/**
 * Instruction:
 * 		SBR Rd, K
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
int sbr(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = r[rd] | k;
	return 0;
}

/**
 * Instruction:
 * 		TST Rd
 *		0010 00dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int tst(uint8_t *codePtr)
{
  uint16_t rd;
  
  rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
  
  r[rd] = r[rd] & r[rd];
  return 0;
}

/**
 * Instruction:
 * 		CLR Rd
 *		0010 01dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int clr(uint8_t *codePtr)
{
  uint16_t rd;
  
  rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
  
  r[rd] = r[rd] | r[rd];
  return 0;
}

/**
 * Instruction:
 * 		SER Rd
 *		1110 1111 dddd 1111
 * where
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
int ser(uint8_t *codePtr)
{
	uint8_t rd;
  
	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	
	r[rd] = 0xff;
	return 0;
}

/**
 * Instruction:
 * 		MUL Rd,Rr
 *		1001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int mul(uint8_t *codePtr)
{
	uint8_t rd, rr;
	uint16_t result;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	
	result = r[rd] * r[rr];
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		MULS Rd, Rr
 *		0000 0010 dddd rrrr
 * where
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
 *
 *      16 <= rrrr <= 31
 * 		rrrr is {
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
int muls(uint8_t *codePtr)
{
	uint8_t rd, rr;
	int16_t result;
	
	rd = ((*codePtr & 0xf0) >> 4) + 16;
	rr = (*codePtr & 0x0f) + 16;

	result = (int8_t)r[rd] * (int8_t)r[rr];

	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		MULSU Rd, Rr
 *		0000 0010 0ddd 0rrr
 * where
 *      16 <= ddd <= 23
 * 		ddd is {
 *			000 => 16,
 *			001 => 17, 
 *			010 => 18,
 *			011 => 19, 
 *			100 => 20,
 *			101 => 21, 
 *			110 => 22,
 *			111 => 23, 
 *		}
 *
 *      16 <= rrr <= 23
 * 		rrr is {
 *			000 => 16,
 *			001 => 17, 
 *			010 => 18,
 *			011 => 19, 
 *			100 => 20,
 *			101 => 21, 
 *			110 => 22,
 *			111 => 23, 
 *		}
 */
int mulsu(uint8_t *codePtr)
{
	uint8_t rd, rr;
	uint16_t result;
	
	rd = ((*codePtr & 0x70) >> 4) + 16;
	rr = (*codePtr & 0x07) + 16;

	result = (int8_t)r[rd] * r[rr];
	
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		LSL Rd
 *		0000 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int lsl(uint8_t *codePtr)
{
	uint8_t rd;
	uint16_t temp;
  
	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
	
	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp;
	sreg->C = (temp & 0x100) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		LSR Rd
 *		1001 010d dddd 0110
 * where
 *		0 <= ddddd <= 31
 */
int lsr(uint8_t *codePtr)
{
	uint8_t rd, temp;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	temp = r[rd];
	r[rd] = r[rd] >> 1;
	sreg->C = temp & 0x1;
	return 0;
}

/**
 * Instruction:
 * 		ROL Rd
 *		0001 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 */
int rol(uint8_t *codePtr)
{
	uint8_t rd;
	uint16_t temp;
  
	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
	
	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp + sreg->C;
	sreg->C = (temp & 0x100) >> 8;
	return 0;
}

/**
 * Instruction:
 * 		ROR Rd
 *		1001 010d dddd 0111
 * where
 *		0 <= ddddd <= 31
 */
int ror(uint8_t *codePtr)
{
	uint8_t rd, temp;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	temp = r[rd];
	r[rd] = (r[rd] >> 1) + (sreg->C << 7);
	sreg->C = temp & 0x1;
	return 0;
}

/**
 * Instruction:
 * 		ASR Rd
 *		1001 010d dddd 0101
 * where
 *		0 <= ddddd <= 31
 */
int asr(uint8_t *codePtr)
{
	uint8_t rd, temp;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	temp = r[rd];
	r[rd] = (r[rd] >> 1) + (r[rd] & 0x80) ;
	sreg->C = temp & 0x1;
	return 0;
}

/**
 * Instruction:
 * 		SWAP Rd
 *		1001 010d dddd 0010
 * where
 *		0 <= ddddd <= 31
 */
int swap(uint8_t *codePtr)
{
	uint8_t rd;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	
	r[rd] = ((r[rd] & 0x0f) << 4) + ((r[rd] & 0xf0) >> 4);
	return 0;
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 */
int bset(uint8_t *codePtr)
{
	uint8_t s;
  
	s = (*codePtr & 0x70) >> 4;
	
	switch(s)
	{
		case 0b000: 
			sreg->C = 1;
			break;
		case 0b001: 
			sreg->Z = 1;
			break;
		case 0b010: 
			sreg->N = 1;
			break;
		case 0b011: 
			sreg->V = 1;
			break;
		case 0b100: 
			sreg->S = 1;
			break;
		case 0b101: 
			sreg->H = 1;
			break;
		case 0b110: 
			sreg->T = 1;
			break;
		case 0b111: 
			sreg->I = 1;
			break;
		default: 
			printf("error!");
	} 
	return 0;
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 */
int bclr(uint8_t *codePtr)
{
	uint8_t s;
  
	s = (*codePtr & 0x70) >> 4;
	
	switch(s)
	{
		case 0b000: 
			sreg->C = 0;
			break;
		case 0b001: 
			sreg->Z = 0;
			break;
		case 0b010: 
			sreg->N = 0;
			break;
		case 0b011: 
			sreg->V = 0;
			break;
		case 0b100: 
			sreg->S = 0;
			break;
		case 0b101: 
			sreg->H = 0;
			break;
		case 0b110: 
			sreg->T = 0;
			break;
		case 0b111: 
			sreg->I = 0;
			break;
		default: 
			printf("error!");
	} 
	return 0;
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 */
int bst(uint8_t *codePtr)
{
	uint8_t rd, b;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	b = *codePtr & 0x7;
	
	switch(b)
	{
		case 0b000: 
			sreg->T = r[rd] & 0x1;
			break;
		case 0b001: 
			sreg->T = (r[rd] & 0x2) >> 1;
			break;
		case 0b010: 
			sreg->T = (r[rd] & 0x4) >> 2;
			break;
		case 0b011: 
			sreg->T = (r[rd] & 0x8) >> 3;
			break;
		case 0b100: 
			sreg->T = (r[rd] & 0x10) >> 4;
			break;
		case 0b101: 
			sreg->T = (r[rd] & 0x20) >> 5;
			break;
		case 0b110: 
			sreg->T = (r[rd] & 0x40) >> 6;
			break;
		case 0b111: 
			sreg->T = (r[rd] & 0x80) >> 7;
			break;
		default: 
			printf("error!");
	} 
	return 0;
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 */
int bld(uint8_t *codePtr)
{
	uint8_t rd, b;
  
	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	b = *codePtr & 0x7;
	
	switch(b)
	{
		case 0b000: 
			r[rd] = (r[rd] & 0xfe) + sreg->T;
			break;
		case 0b001: 
			r[rd] = (r[rd] & 0xfd) + (sreg->T << 1);
			break;
		case 0b010: 
			r[rd] = (r[rd] & 0xfb) + (sreg->T << 2);
			break;
		case 0b011: 
			r[rd] = (r[rd] & 0xf7) + (sreg->T << 3);
			break;
		case 0b100: 
			r[rd] = (r[rd] & 0xef) + (sreg->T << 4);
			break;
		case 0b101: 
			r[rd] = (r[rd] & 0xdf) + (sreg->T << 5);
			break;
		case 0b110: 
			r[rd] = (r[rd] & 0xbf) + (sreg->T << 6);
			break;
		case 0b111: 
			r[rd] = (r[rd] & 0x7f) + (sreg->T << 7);
			break;
		default: 
			printf("error!");
	} 
	return 0;
}

/**
 * Instruction:
 * 		SEC None
 *		1001 0100 0000 1000
 */
int sec(uint8_t *codePtr)
{
	sreg->C = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLC None
 *		1001 0100 1000 1000
 */
int clc(uint8_t *codePtr)
{
	sreg->C = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEN None
 *		1001 0100 0010 1000
 */
int sen(uint8_t *codePtr)
{
	sreg->N = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLN None
 *		1001 0100 1010 1000
 */
int cln(uint8_t *codePtr)
{
	sreg->N = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEZ None
 *		1001 0100 0001 1000
 */
int sez(uint8_t *codePtr)
{
	sreg->Z = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLZ None
 *		1001 0100 1001 1000
 */
int clz(uint8_t *codePtr)
{
	sreg->Z = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEI None
 *		1001 0100 0111 1000
 */
int sei(uint8_t *codePtr)
{
	sreg->I = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLI None
 *		1001 0100 1111 1000
 */
int cli(uint8_t *codePtr)
{
	sreg->I = 0;
	return 0;
}

/**
 * Instruction:
 * 		SES None
 *		1001 0100 0100 1000
 */
int ses(uint8_t *codePtr)
{
	sreg->S = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLS None
 *		1001 0100 1100 1000
 */
int cls(uint8_t *codePtr)
{
	sreg->S = 0;
	return 0;
}

/**
 * Instruction:
 * 		SEV None
 *		1001 0100 0011 1000
 */
int sev(uint8_t *codePtr)
{
	sreg->V = 1;
	return 0;
}

/**
 * Instruction:
 * 		CLV None
 *		1001 0100 1011 1000
 */
int clv(uint8_t *codePtr)
{
	sreg->V = 0;
	return 0;
}

/**
 * Instruction:
 * 		SET None
 *		1001 0100 0110 1000
 */
int set(uint8_t *codePtr)
{
	sreg->T = 1;
	return 0;
} 

/**
 * Instruction:
 * 		CLT None
 *		1001 0100 1110 1000
 */
int clt(uint8_t *codePtr)
{
	sreg->T = 0;
	return 0;
} 

/**
 * Instruction:
 * 		SEH None
 *		1001 0100 0101 1000
 */
int seh(uint8_t *codePtr)
{
	sreg->H = 1;
	return 0;
} 

/**
 * Instruction:
 * 		CLH None
 *		1001 0100 1101 1000
 */
int clh(uint8_t *codePtr)
{
	sreg->H = 0;
	return 0;
} 