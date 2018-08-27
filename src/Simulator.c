#include <stdio.h>
#include <stdint.h>
#include "Simulator.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

AvrOperator avrOperatorTable[256] = {
  [0x00] = nop,
  [0x01] = movw,
  [0x02] = muls,
  [0x03] = mulsuORfmulORfmulsORfmulsu,
  [0x04 ... 0x07] = cpc,
  [0x08 ... 0x0b] = sbc,
  [0x0c ... 0x0f] = add,
  [0x10 ... 0x13] = cpse,
  [0x14 ... 0x17] = cp,
  [0x18 ... 0x1b] = sub,
  [0x1c ... 0x1f] = adc,
  [0x20 ... 0x23] = and,
  [0x24 ... 0x27] = eor,
  [0x28] = or,
  [0x29 ... 0x2b] = NULL,
  [0x2c ... 0x2f] = mov,
  [0x30 ... 0x3f] = cpi,
  [0x40 ... 0x4f] = sbci,
  [0x50 ... 0x5f] = subi,
  [0x60 ... 0x6f] = ori,
  [0x70 ... 0x7f] = andi,
	[0x80 ... 0x81] = lddyORlddz,
  [0x82 ... 0x83] = stdyORstdz,
	[0x84 ... 0x85] = lddyORlddz,
  [0x86 ... 0x87] = stdyORstdz,
	[0x88 ... 0x89] = lddyORlddz,
  [0x8a ... 0x8b] = stdyORstdz,
	[0x8c ... 0x8d] = lddyORlddz,
  [0x8e ... 0x8f] = stdyORstdz,
  [0x90 ... 0x91] = instructionWith1001000,
	[0x92 ... 0x93] = instructionWith1001001,
  [0x94 ... 0x95] = instructionWith1001010,
  [0x96] = adiw,
  [0x97] = sbiw,
  [0x98] = cbi,
  [0x99] = sbic,
  [0x9a] = sbi,
  [0x9b] = sbis,
  [0x9c ... 0x9f] = mul,
  [0xa0 ... 0xa1] = lddyORlddz,
  [0xa2 ... 0xa3] = stdyORstdz,
	[0xa4 ... 0xa5] = lddyORlddz,
  [0xa6 ... 0xa7] = stdyORstdz,
	[0xa8 ... 0xa9] = lddyORlddz,
  [0xaa ... 0xab] = stdyORstdz,
	[0xac ... 0xad] = lddyORlddz,
  [0xae ... 0xaf] = stdyORstdz,
	[0xb0 ... 0xb7] = in,
	[0xb8 ... 0xbf] = out,
  [0xc0 ... 0xcf] = rjmp,
  [0xd0 ... 0xdf] = rcall,
  [0xe0 ... 0xef] = ldi,
  [0xf0 ... 0xf3] = brbs,
  [0xf4 ... 0xf7] = brbc,
  [0xf8 ... 0xf9] = bld,
  [0xfa ... 0xfb] = bst,
  [0xfc ... 0xfd] = sbrc,
  [0xfe ... 0xff] = sbrs,
};

AvrOperator avr1001010Table[16] = {
  [0x0] = com,
  [0x1] = neg,
  [0x2] = swap,
  [0x3] = inc,
  [0x4] = NULL,
  [0x5] = asr,
  [0x6] = lsr,
  [0x7] = ror,
  [0x8] = instructionWith1001010x,
  [0x9] = instructionWith0x9__9,
  [0xa] = dec,
  [0xb] = NULL,
  [0xc ... 0xd] = jmp,
  [0xe ... 0xf] = call,
};

AvrOperator avr1001000Table[16] = {
  [0x0] = NULL,
  [0x1] = ldzPostInc,
  [0x2] = ldzPreDec,
  [0x3] = NULL,
  [0x4] = lpmUnchange,
  [0x5] = lpmPostInc,
  [0x6] = NULL,
  [0x7] = NULL,
  [0x8] = NULL,
  [0x9] = ldyPostInc,
  [0xa] = ldyPreDec,
  [0xb] = NULL,
  [0xc] = ldxUnchanged,
	[0xd] = ldxPostInc,
  [0xe] = ldxPreDec,
	[0xf] = pop,
};

AvrOperator avr1001001Table[16] = {
  [0x0] = NULL,
  [0x1] = stzPostInc,
  [0x2] = stzPreDec,
  [0x3] = NULL,
  [0x4] = xch,
  [0x5] = las,
  [0x6] = lac,
  [0x7] = lat,
  [0x8] = NULL,
  [0x9] = styPostInc,
  [0xa] = styPreDec,
  [0xb] = NULL,
  [0xc] = stxUnchanged,
	[0xd] = stxPostInc,
  [0xe] = stxPreDec,
	[0xf] = push,
};

//AVR SRAM
uint8_t sram[SRAM_SIZE];
uint8_t flashMemory[FLASH_SIZE];

//AVR Register
uint8_t *r = sram;
SregRegister *sreg = (SregRegister*)&sram[0x5f];
uint8_t *io = &sram[0x20];
uint16_t *xRegPtr = (uint16_t *)&sram[26];
uint16_t *yRegPtr = (uint16_t *)&sram[28];
uint16_t *zRegPtr = (uint16_t *)&sram[30];
uint8_t *sph = &sram[0x5e];
uint8_t *spl = &sram[0x5d];
uint8_t *eind = &sram[0x5c];
uint8_t *pc = flashMemory;
uint8_t *flash = flashMemory;

int simulateOneInstruction(uint8_t *codePtr)
{
	return avrOperatorTable [*(codePtr + 1)](codePtr);
}

void initialiseSram()
{
	uint16_t size;
	for(size=0x0;size<0x900;size++)
  {
		sram[size] = 0;
	}
}

void dumpSram(int startAddr, int byteCount) {
  int i;
  for(i = startAddr; i < startAddr + byteCount; i += 16) {
    printf("0x%03x  %02x %02x %02x %02x %02x %02x %02x %02x "               \
           "%02x %02x %02x %02x %02x %02x %02x %02x\n",                     \
           i, sram[i], sram[i+1], sram[i+2], sram[i+3],                     \
           sram[i+4], sram[i+5], sram[i+6], sram[i+7],                      \
           sram[i+8], sram[i+9], sram[i+10], sram[i+11],                    \
           sram[i+12], sram[i+13], sram[i+14], sram[i+15]);
  }
}

int instructionWith1001010(uint8_t *codePtr)
{
	uint8_t low4bit;
	low4bit = *codePtr & 0xf;

  return avr1001010Table [low4bit](codePtr);
}

int instructionWith1001010x(uint8_t *codePtr)
{
	uint8_t is0x95, ins, temp, temp1, isbclr;
  temp1 = temp = *codePtr;
  is0x95 = (codePtr[1] & 0x1) | (codePtr[0] & 0x0);
  ins = temp & 0xf0;
	isbclr = *codePtr & 0x80;

  if(is0x95)
  {
   if(ins == 0x90)
    Break(codePtr);
   else if(ins == 0xa0)
    wdr(codePtr);
   else if(ins == 0xc0)
    lpmUnchangeR0(codePtr);
   else if(ins == 0x00)
    ret(codePtr);
   else if(ins == 0x10)
    reti(codePtr);
   else
    sleep(codePtr);
  }
  else
  {
    if(isbclr)
      bclr(codePtr);
    else
      bset(codePtr);
  }
}

int instructionWith0x9__9(uint8_t *codePtr)
{
  uint8_t isIjmp, is0x95, isIcall;
  is0x95 = (codePtr[1] & 0x1) | (codePtr[0] & 0x0);
  isIjmp = *codePtr & 0xf0;
  isIcall = *codePtr & 0xf0;
  if(is0x95)
    if(isIcall == 0x00)
      icall(codePtr);
    else
      eicall(codePtr);
  else
    if(isIjmp == 0x00)
      ijmp(codePtr);
    else
      eijmp(codePtr);

}

int instructionWith1001000(uint8_t *codePtr)
{
	uint8_t low4bit;
	low4bit = *codePtr & 0xf;

  return avr1001000Table [low4bit](codePtr);
}

int instructionWith1001001(uint8_t *codePtr)
{
	uint8_t low4bit;
	low4bit = *codePtr & 0xf;

  return avr1001001Table [low4bit](codePtr);
}

int mulsuORfmulORfmulsORfmulsu(uint8_t *codePtr)
{
	uint8_t lowBit;
	lowBit = ((*codePtr & 0x8) >> 3) | ((*codePtr & 0x80) >> 6);

	if(lowBit == 0x0)
		mulsu(codePtr);
	else if(lowBit == 0x01)
		fmul(codePtr);
	else if(lowBit == 0x2)
		fmuls(codePtr);
  else
    fmulsu(codePtr);
}

int lddyORlddz(uint8_t *codePtr)
{
	uint8_t isLddy;
	isLddy = (*codePtr & 0x8) >> 3;

	if(isLddy)
		lddy(codePtr);
	else
		lddz(codePtr);
}

int stdyORstdz(uint8_t *codePtr)
{
	uint8_t isStdy;
	isStdy = (*codePtr & 0x8) >> 3;

	if(isStdy)
		stdy(codePtr);
	else
		stdz(codePtr);
}

uint32_t getPc(uint8_t *progCounter)
{
	return (progCounter - flash);
}

uint8_t *getCodePtr(uint32_t pc)
{
	return (flash + pc);
}

uint16_t getMcuStackPtr()
{
  uint16_t McuStackPtr;
  McuStackPtr = ((uint16_t)*sph << 8) | *spl;
  return McuStackPtr;
}

uint16_t substractStackPointer(int value)
{
  return *(uint16_t *)spl -= value;
}

void pushWord(uint16_t data)
{
  uint16_t index;
  index = substractStackPointer(2);
  sram[index + 2] = data & 0x00ff;
  sram[index + 1] = (data & 0xff00) >> 8;
}

uint32_t popWord()
{
	uint16_t index;
	uint32_t data;
	index = substractStackPointer(-2);
	data = ((sram[index - 1]) << 8) | sram[index];
  return data;
}

void initSimulator()
{
  *(uint16_t *)spl = 0x8ff;
	initialiseSram();
}

int is2wordInstruction(uint8_t *codePtr)
{
  if((*(uint16_t *)codePtr & 0xf000) == 0x9000)
  {
    if((*(uint16_t *)codePtr & 0x0e00) == 0x400 || (*(uint16_t *)codePtr & 0x0e00) == 0x200)
    {
      if((*(uint16_t *)codePtr & 0xf) == 0x0 || (*(uint16_t *)codePtr & 0xe) == 0xc || (*(uint16_t *)codePtr & 0xe) == 0xe)
        return 1;
    }
  }
  return 0;
}

/**
 * Z:
 *    !R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
int is8bitZero(uint8_t data8bit)
{
	if(data8bit == 0)
		return 1;
	else
		return 0;
}

/**
 * C:
 *    !R15.Rdh7
 */
int is16bitADIWCarry(uint16_t result, uint16_t operand)
{
	result = (~result & operand) >> 15;
	return result;
}

/**
 * Z:
 *    !R15.!R14.!R13.!R12.!R11.!R10.!R9.!R8.!R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
int is16BitZero(uint16_t data16Bit)
{
	if(data16Bit == 0)
		return 1;
	else
		return 0;
}

/**
 * N:
 *    R15
 */
int is16bitNeg(uint16_t result)
{
	result = result >> 15;
	return result;
}

/**
 * V:
 *    R15.!Rdh7
 */
int is16bitADIWOverflow(uint16_t result, uint16_t operand)
{
	result = (~operand & result) >> 15;
	return result;
}

/**
 * S:
 *    N ^ V
 */
int is16bitSigned(uint16_t result, uint16_t operand)
{
	uint8_t n, v, s;
	n = is16bitNeg(result);
	v = is16bitADIWOverflow(result, operand);
	s = n ^ v;
	return s;
}

int handleStatusRegForAddImmWordOperation(uint16_t result, uint16_t operand)
{
	sreg->C = is16bitADIWCarry(result, operand);
	sreg->Z = is16BitZero(result);
	sreg->N = is16bitNeg(result);
	sreg->V = is16bitADIWOverflow(result, operand);
	sreg->S = is16bitSigned(result, operand);
	return 0;
}

/**
 * C:
 *    R15.!Rdh7
 */
int is16bitSBIWCarry(uint16_t result, uint16_t operand)
{
	result = (~operand & result) >> 15;
	return result;
}

/**
 * V:
 *    !R15.Rdh7
 */
int is16bitSBIWOverflow(uint16_t result, uint16_t operand)
{
	result = (~result & operand) >> 15;
	return result;
}

int handleStatusRegForSubImmWordOperation(uint16_t result, uint16_t operand)
{
	sreg->C = is16bitSBIWCarry(result, operand);
	sreg->Z = is16BitZero(result);
	sreg->N = is16bitNeg(result);
	sreg->V = is16bitSBIWOverflow(result, operand);
	sreg->S = is16bitSigned(result, operand);
	return 0;
}

/**
 * C:
 *    Rd7.Rr7+Rr7.!R7+!R7.Rd7
 */
int is8bitAdcAddCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = (operand1 & operand2 | operand2 & (~result) | (~result) & operand1) >> 7;
  return result;
}

/**
 * N:
 *    R7
 */
int is8bitNeg(uint8_t result)
{
  result = result >> 7;
  return result;
}

/**
 * V:
 *    Rd7.Rr7.!R7+!Rd7.!Rr7.R7
 */
int is8bitOverflow(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = (operand1 & operand2 & (~result) | (~operand1) & (~operand2) & result) >> 7;
  return result;
}

/**
 * S:
 *    N^V
 */
int is8bitSigned(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitOverflow(operand1, operand2, result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *    Rd3.Rr3+Rr3.!R3+!R3.Rd3
 */
int is8bitAdcAddHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((operand1 & operand2 | operand2 & (~result) | (~result) & operand1) & 0x8) >> 3;
  return result;
}

int handleStatusRegForAddAdcOperation(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	sreg->C = is8bitAdcAddCarry(operand1, operand2, result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitOverflow(operand1, operand2, result);
	sreg->S = is8bitSigned(operand1, operand2, result);
	sreg->H = is8bitAdcAddHalfCarry(operand1, operand2, result);
	return 0;
}

/**
 * V:
 *    0
 */
int is8bitAndAndiOrOriEorCbrSbrTstOverflow()
{
	uint8_t v;
	sreg->V = 0;
	v = sreg->V;
	return 0;
}

/**
 * S:
 *    N^V
 */
int is8bitAndAndiOrOriEorCbrSbrTstSigned(uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	s = n ^ v;
	return s;
}

int handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	sreg->S = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
}

/**
 * C:
 *    !Rd7.Rr7+Rr7.R7+R7.!Rd7
 */
int is8bitSubSubiSbcSbciCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((~operand1) & operand2 | operand2 & result | result & (~operand1)) >> 7;
  return result;
}

/**
 * H:
 *    !Rd3.Rr3+Rr3.R3+R3.!Rd3
 */
int is8bitSubSubiSbcSbciHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result)
{
  result = ((((~operand1) & operand2) | (operand2 & result) | (result & (~operand1))) & 0x8) >> 3;
  return result;
}

/**
 * V:
 *    Rd7.!Rr7.!R7+!Rd7.Rr7.R7
 */
int is8bitSubSubiSbcSbciOverflow(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	result = ((operand1 & ~operand2 & ~result) | (~operand1 & operand2 & result)) >> 7;
  return result;
}

/**
 * S:
 *    N^V
 */
int is8bitSubSubiSbcSbciSigned(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	s = n ^ v;
	return s;
}

int handleStatusRegForSubSubiSbcSbciOperation(uint8_t operand1, uint8_t operand2, uint8_t result)
{
	sreg->C = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	sreg->S = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	sreg->H = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	return 0;
}

/**
 * C:
 *    1
 */
int is8bitComCarry()
{
	uint8_t c;
	sreg->C = 1;
	c = sreg->C;
	return c;
}

int handleStatusRegForComOperation(uint8_t result)
{
	sreg->C = is8bitComCarry();
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	sreg->S = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
}

/**
 * C:
 *    R7+R6+R5+R4+R3+R2+R1+R0
 */
int is8bitNegCarry(uint8_t data8bit)
{
	if(data8bit == 0x00)
		return 0;
	else
		return 1;
}

/**
 * V:
 *    R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
int is8bitNegOverflow(uint8_t data8bit)
{
	if(data8bit == 0x80)
		return 1;
	else
		return 0;
}

/**
 * S:
 *    N^V
 */
int is8bitNegSigned(uint8_t result)
{
	uint8_t n, v, s;
	n = is8bitNeg(result);
	v = is8bitNegOverflow(result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *    R3+Rd3
 */
int is8bitNegHalfCarry(uint8_t operand1, uint8_t result)
{
	uint8_t h;
	h = ((result | operand1) & 0x8) >> 3;
	return h;
}

int handleStatusRegForNegOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitNegCarry(result);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitNegOverflow(result);
	sreg->S = is8bitNegSigned(result);
	sreg->H = is8bitNegHalfCarry(operand1, result);
}

int handleStatusRegForIncOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitNegOverflow(result);
	sreg->S = is8bitNegSigned(result);
}

/**
 * V:
 *    !R7.R6.R5.R4.R3.R2.R1.R0
 */
int is8bitDecOverflow(uint8_t data8bit)
{
	if(data8bit == 0x7f)
		return 1;
	else
		return 0;
}

int handleStatusRegForDecOperation(uint8_t result)
{
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitDecOverflow(result);
	sreg->S = is8bitNegSigned(result);
}

int handleStatusRegForClrOperation()
{
	sreg->Z = 1;
	sreg->N = 0;
	sreg->V = 0;
	sreg->S = 0;
}

/**
 * C:
 *    R15
 */
int is16bitMulMulsMulsuCarry(uint16_t result)
{
	uint16_t c;

	c = result >> 15;
	return c;
}

int handleStatusRegForMulMulsMulsuOperation(uint16_t result)
{
	sreg->C = is16bitMulMulsMulsuCarry(result);
	sreg->Z = is16BitZero(result);
}

/**
 * C:
 *    Rd7
 */
int is8bitLslRolCarry(uint8_t operand1)
{
	uint8_t c;

	c = operand1 >> 7;
	return c;
}

/**
 * V:
 *    N^C
 */
int is8bitLslRolOverflow(uint8_t operand1, uint8_t result)
{
	uint8_t n, c, v;

	n = is8bitNeg(result);
	c = is8bitLslRolCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *    N^V
 */
int is8bitLslRolSigned(uint8_t operand1, uint8_t result)
{
	uint8_t n, v, s;

	n = is8bitNeg(result);
	v = is8bitLslRolOverflow(operand1, result);
	s = n ^ v;
	return s;
}

/**
 * H:
 *    Rd3
 */
int is8bitLslRolHalfCarry(uint8_t operand1)
{
	uint8_t h;

	h = (operand1 & 0x8) >> 3;
	return h;
}

int handleStatusRegForLslRolOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLslRolCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitLslRolOverflow(operand1, result);
	sreg->S = is8bitLslRolSigned(operand1, result);
	sreg->H = is8bitLslRolHalfCarry(operand1);
}

/**
 * C:
 *    Rd0
 */
int is8bitLsrRorCarry(uint8_t operand1)
{
	uint8_t c;

	c = operand1 & 0x1;
	return c;
}

/**
 * V:
 *    N^C
 */
int is8bitLsrOverflow(uint8_t operand1)
{
	uint8_t n, c, v;

	n = 0;
	c = is8bitLsrRorCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *    N^V
 */
int is8bitLsrSigned(uint8_t operand1)
{
	uint8_t n, v, s;

	n = 0;
	v = is8bitLsrOverflow(operand1);
	s = n ^ v;
	return s;
}

int handleStatusRegForLsrOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLsrRorCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = 0;
	sreg->V = is8bitLsrOverflow(operand1);
	sreg->S = is8bitLsrSigned(operand1);
}

/**
 * V:
 *    N^C
 */
int is8bitRorOverflow(uint8_t operand1, uint8_t result)
{
	uint8_t n, c, v;

	n = is8bitNeg(result);
	c = is8bitLsrRorCarry(operand1);
	v = n ^ c;
	return v;
}

/**
 * S:
 *    N^V
 */
int is8bitRorSigned(uint8_t operand1, uint8_t result)
{
	uint8_t n, v, s;

	n = is8bitNeg(result);
	v = is8bitRorOverflow(operand1, result);
	s = n ^ v;
	return s;
}

int handleStatusRegForRorAsrOperation(uint8_t operand1, uint8_t result)
{
	sreg->C = is8bitLsrRorCarry(operand1);
	sreg->Z = is8bitZero(result);
	sreg->N = is8bitNeg(result);
	sreg->V = is8bitRorOverflow(operand1, result);
	sreg->S = is8bitRorSigned(operand1, result);
}

int handleStatusRegForRorBstOperation(uint8_t operand1)
{
	if(operand1 == 0)
		return 0;
	else
		return 1;
}

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
  uint8_t rd, rr, regVal;

  rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
  regVal = r[rd];
  r[rd] = r[rd] + r[rr];
  handleStatusRegForAddAdcOperation(regVal, r[rr], r[rd]);
  return 2;
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
	uint8_t rd, rr, regVal;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	regVal = r[rd];
	r[rd] = r[rd] + r[rr] + sreg->C;
	handleStatusRegForAddAdcOperation(regVal, r[rr], r[rd]);
  return 2;
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
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 2;
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
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 2;
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
  uint16_t *word, before;

  k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
  rd = ((*codePtr & 0x30) >> 3) + 24;

  word = (uint16_t*)&r[rd];
  before = *word;
  *word += k;

  r[rd] = *word;
  r[rd+1] = (*word & 0xff00) >> 8;
  handleStatusRegForAddImmWordOperation(*word, before);

  return 2;
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
	uint8_t rd, rr, regVal;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
  regVal = r[rd];
	r[rd] = r[rd] - r[rr];
	handleStatusRegForSubSubiSbcSbciOperation(regVal, r[rr], r[rd]);
	return 2;
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
	uint8_t rd, k, regVal;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);
  regVal = r[rd];
	r[rd] = r[rd] - k;
	handleStatusRegForSubSubiSbcSbciOperation(regVal, k, r[rd]);
	return 2;
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
	uint8_t rd, rr, regVal;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	regVal = r[rd];
	r[rd] = r[rd] - r[rr] - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(regVal, r[rr], r[rd]);
	return 2;
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
	uint8_t rd, k, regVal;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);
  regVal = r[rd];
	r[rd] = r[rd] - k - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(regVal, k, r[rd]);
	return 2;
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
	uint16_t word, before;

	k = (*codePtr & 0xf) | ((*codePtr & 0xc0) >> 2);
	rd = ((*codePtr & 0x30) >> 3) + 24;

	word = (((uint16_t)r[rd+1]) << 8) | r[rd];
	before = word;
	word -= k;

  handleStatusRegForSubImmWordOperation(word, before);

	r[rd] = word;
	r[rd+1] = (word & 0xff00) >> 8;
	return 2;
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
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 2;
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
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 2;
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
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 2;
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
	handleStatusRegForComOperation(r[rd]);
	return 2;
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
	uint8_t rd, regVal;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  regVal = r[rd];
	r[rd] = 0x00 - r[rd];
	handleStatusRegForNegOperation(regVal, r[rd]);
	return 2;
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
	handleStatusRegForDecOperation(r[rd]);
	return 2;
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
	handleStatusRegForIncOperation(r[rd]);
	return 2;
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
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 2;
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
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
	return 2;
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
  handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(r[rd]);
  return 2;
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
  handleStatusRegForClrOperation();
  return 2;
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
	return 2;
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
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
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
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
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
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
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
	uint8_t rd, regVal;
	uint16_t temp;

	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
	regVal = r[rd];
	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp;
	handleStatusRegForLslRolOperation(regVal, r[rd]);
	return 2;
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
	uint8_t rd, temp, regVal;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	regVal = r[rd];
	temp = r[rd];
	r[rd] = r[rd] >> 1;
	handleStatusRegForLsrOperation(regVal, r[rd]);
	return 2;
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
	uint8_t rd, regVal;
	uint16_t temp;

	rd = ((codePtr[1] & 0x3) << 7) | (codePtr[0] & 0xff);
	regVal = r[rd];
	temp = ((uint16_t)r[rd] << 1);
	r[rd] = temp + sreg->C;
	handleStatusRegForLslRolOperation(regVal, r[rd]);
	return 2;
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
	handleStatusRegForRorAsrOperation(temp, r[rd]);
	return 2;
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
	handleStatusRegForRorAsrOperation(temp, r[rd]);
	return 2;
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
	return 2;
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
	return 2;
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
	return 2;
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

	sreg->T = (r[rd] & (1 << b)) >> b;

	handleStatusRegForRorBstOperation(r[rd]);
	return 2;
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

  r[rd] = (r[rd] & ~(1 << b)) + (sreg->T << b);
	return 2;
}

/**
 * Instruction:
 * 		SEC None
 *		1001 0100 0000 1000
 */
int sec(uint8_t *codePtr)
{
	sreg->C = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLC None
 *		1001 0100 1000 1000
 */
int clc(uint8_t *codePtr)
{
	sreg->C = 0;
	return 2;
}

/**
 * Instruction:
 * 		SEN None
 *		1001 0100 0010 1000
 */
int sen(uint8_t *codePtr)
{
	sreg->N = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLN None
 *		1001 0100 1010 1000
 */
int cln(uint8_t *codePtr)
{
	sreg->N = 0;
	return 2;
}

/**
 * Instruction:
 * 		SEZ None
 *		1001 0100 0001 1000
 */
int sez(uint8_t *codePtr)
{
	sreg->Z = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLZ None
 *		1001 0100 1001 1000
 */
int clz(uint8_t *codePtr)
{
	sreg->Z = 0;
	return 2;
}

/**
 * Instruction:
 * 		SEI None
 *		1001 0100 0111 1000
 */
int sei(uint8_t *codePtr)
{
	sreg->I = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLI None
 *		1001 0100 1111 1000
 */
int cli(uint8_t *codePtr)
{
	sreg->I = 0;
	return 2;
}

/**
 * Instruction:
 * 		SES None
 *		1001 0100 0100 1000
 */
int ses(uint8_t *codePtr)
{
	sreg->S = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLS None
 *		1001 0100 1100 1000
 */
int cls(uint8_t *codePtr)
{
	sreg->S = 0;
	return 2;
}

/**
 * Instruction:
 * 		SEV None
 *		1001 0100 0011 1000
 */
int sev(uint8_t *codePtr)
{
	sreg->V = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLV None
 *		1001 0100 1011 1000
 */
int clv(uint8_t *codePtr)
{
	sreg->V = 0;
	return 2;
}

/**
 * Instruction:
 * 		SET None
 *		1001 0100 0110 1000
 */
int set(uint8_t *codePtr)
{
	sreg->T = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLT None
 *		1001 0100 1110 1000
 */
int clt(uint8_t *codePtr)
{
	sreg->T = 0;
	return 2;
}

/**
 * Instruction:
 * 		SEH None
 *		1001 0100 0101 1000
 */
int seh(uint8_t *codePtr)
{
	sreg->H = 1;
	return 2;
}

/**
 * Instruction:
 * 		CLH None
 *		1001 0100 1101 1000
 */
int clh(uint8_t *codePtr)
{
	sreg->H = 0;
	return 2;
}

/**
 * Instruction:
 * 		MOV Rd, Rr
 *		0010 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 */
int mov(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

  r[rd] = r[rr];
	return 2;
}

/**
 * Instruction:
 * 		RJMP k
 *		1100 kkkk kkkk kkkk
 * where
 *		-2K <= kkkkkkkkkkkk <= 2K
 */
int rjmp(uint8_t *codePtr)
{
	int k;

	k = ((codePtr[1] & 0xf) << 8) | (codePtr[0] & 0xff);

  if(k & 0x800)
    k |= 0xfffff000;

	return (k+1) * 2;
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 */
int jmp(uint8_t *codePtr)
{
	uint32_t k;
	k = *(uint32_t *)codePtr;

	k = ((k & 0xffff0000) >> 16) | ((k & 0x1f0) << 13) | ((k & 0x1) << 16);

	return getCodePtr(k*2) - codePtr;
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 */
int brbs(uint8_t *codePtr)
{
	uint8_t k, s;
	uint16_t signExt;

	k = ((codePtr[0] & 0xf8) >> 3) | ((codePtr[1] & 0x3) << 5);
  s = (*codePtr & 0x7);

  switch(s)
	{
		case 0b000:
			if(sreg->C == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b001:
			if(sreg->Z == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b010:
			if(sreg->N == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b011:
			if(sreg->V == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b100:
			if(sreg->S == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b101:
			if(sreg->H == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b110:
			if(sreg->T == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		case 0b111:
			if(sreg->I == 1)
        return ((k+1) * 2);
      else
        return 2;
			break;
		default:
			printf("error!");
  }
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 */
int brbc(uint8_t *codePtr)
{
	uint8_t s;
	int16_t k;

	k = ((codePtr[0] & 0xf8) >> 3) | ((codePtr[1] & 0x3) << 5);
  s = (*codePtr & 0x7);
	if((k & 0x40) >> 6)
		k = k | 0xff80;
	else
		k = k;

  switch(s)
	{
		case 0b000:
			if(sreg->C == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b001:
			if(sreg->Z == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b010:
			if(sreg->N == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b011:
			if(sreg->V == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b100:
			if(sreg->S == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b101:
			if(sreg->H == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b110:
			if(sreg->T == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		case 0b111:
			if(sreg->I == 0)
        return ((k+1)*2);
      else
        return 2;
			break;
		default:
			printf("error!");
  }
}

/**
 * Instruction:
 * 		NOP None
 *			0000 0000 0000 0000
 *       0    0    0    0
 */
int nop(uint8_t *codePtr)
{
  return 2;
}

/**
 * Instruction:
 * 		WDR None
 *			1001 0101 1010 1000
 *       9    5    a    8
 */
int wdr(uint8_t *codePtr)
{
  throwSimpleError(WATCHDOG_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		BREAK None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
int Break(uint8_t *codePtr)
{
  throwSimpleError(BREAK_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		SLEEP None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
int sleep(uint8_t *codePtr)
{
  throwSimpleError(SLEEP_EXCEPTION, NULL);
}

/**
 * Instruction:
 * 		CP Rd, Rr
 *			0001 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 */
int cp(uint8_t *codePtr)
{
	uint8_t rd, rr, result;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	result = r[rd] - r[rr];
	handleStatusRegForSubSubiSbcSbciOperation(r[rd], r[rr], result);
	return 2;
}

/**
 * Instruction:
 * 		CPC Rd, Rr
 *			0000 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 */
int cpc(uint8_t *codePtr)
{
	uint8_t rd, rr, result;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);

	result = r[rd] - r[rr] - sreg->C;
	handleStatusRegForSubSubiSbcSbciOperation(r[rd], r[rr], result);
	return 2;
}

/**
 * Instruction:
 * 		CPI Rd, K
 *		0011 KKKK dddd KKKK
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
int cpi(uint8_t *codePtr)
{
	uint8_t rd, k, temp;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	temp = r[rd] - k;
	handleStatusRegForSubSubiSbcSbciOperation(r[rd], k, temp);
	return 2;
}

/**
 * Instruction:
 * 		LDI Rd, K
 *		1110 KKKK dddd KKKK
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
int ldi(uint8_t *codePtr)
{
	uint8_t rd, k;

	rd = ((codePtr[0] & 0xf0) >> 4) + 16;
	k  = ((codePtr[1] & 0xf) << 4) | (codePtr[0] & 0xf);

	r[rd] = k;
	return 2;
}

/**
 * Instruction:
 * 		CPSE Rd, Rr
 *			0001 00rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 */
int cpse(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	rr = ((codePtr[1] & 0x2) << 3) | (codePtr[0] & 0xf);
	codePtr += 2;
	
	if(r[rd] == r[rr])
  {
    if(is2wordInstruction(codePtr))
      return 6;
    else
      return 4;
  }
  else
    return 2;
}

/**
 * Instruction:
 * 		SBRC Rr, b
 *		1111 110r rrrr 0bbb
 * where
 *    0 <= rrrrr <= 31
 *    0 <= bbb <= 7
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 */
int sbrc(uint8_t *codePtr)
{
	uint8_t rr, b, temp;

  rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  b = *codePtr & 0x7;
	temp = r[rr];
  temp = (temp & (1 << b)) >> b;
	codePtr += 2;

	if(temp == 0)
  {
    if(is2wordInstruction(codePtr))
      return 6;
    else
      return 4;
  }
  else
    return 2;
}

/**
 * Instruction:
 * 		SBRS Rr, b
 *		1111 110r rrrr 0bbb
 * where
 *    0 <= rrrrr <= 31
 *    0 <= bbb <= 7
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 */
int sbrs(uint8_t *codePtr)
{
	uint8_t rr, b, temp;

  rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
  b = *codePtr & 0x7;
	temp = r[rr];
  temp = (temp & (1 << b)) >> b;
	codePtr += 2;
	
	if(temp == 1)
  {
    if(is2wordInstruction(codePtr))
      return 6;
    else
      return 4;
  }
  else
    return 2;
}

/**
 * Instruction:
 * 		SBIC A, b
 *		1001 1001 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 */
int sbic(uint8_t *codePtr)
{
	uint8_t A, b;

  A = (*codePtr & 0xf8) >> 3;
  b = *codePtr & 0x7;
  io[A] = (io[A] & (1 << b)) >> b;
	codePtr += 2;
	
	if(io[A] == 0)
  {
    if(is2wordInstruction(codePtr))
      return 6;
    else
      return 4;
  }
  else
    return 2;
}

/**
 * Instruction:
 * 		SBIS A, b
 *		1001 1011 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 */
int sbis(uint8_t *codePtr)
{
	uint8_t A, b;

  A = (*codePtr & 0xf8) >> 3;
  b = *codePtr & 0x7;
  io[A] = (io[A] & (1 << b)) >> b;
	codePtr += 2;

	if(io[A] == 1)
  {
    if(is2wordInstruction(codePtr))
      return 6;
    else
      return 4;
  }
  else
    return 2;
}

/**
 * Instruction:
 * 		IJMP None
 *		1001 0100 0000 1001
 *
 * PC <- Z(15:0)
 */
int ijmp(uint8_t *codePtr)
{
  int pc;
	pc = *zRegPtr;
  return (pc - getPc(codePtr));
}

/**
 * Instruction:
 * 		EIJMP None
 *		1001 0100 0001 1001
 *
 * PC(15:0) <- Z(15:0)
 * PC(21:16) <- EIND
 */
int eijmp(uint8_t *codePtr)
{
  int pc;
	pc = *zRegPtr | ((*(uint32_t *)eind & 0x3f) << 16);
  return (pc - getPc(codePtr));
}

/**
 * Instruction:
 * 		CALL k
 *		1001 010k kkkk 111k
 *    kkkk kkkk kkkk kkkk
 *
 * where
 *    0 <= k <= 64K
 */
int call(uint8_t *codePtr)
{
	uint32_t k;
  uint16_t stackBefore, stackNow, data;

  stackBefore = *(uint16_t *)spl;
	k = *(uint32_t *)codePtr;

	k = ((k & 0xffff0000) >> 16) | ((k & 0x1f0) << 13) | ((k & 0x1) << 16);

	data = (sram[*(uint16_t *)spl] | (sram[*(uint16_t *)spl - 1]) << 8) + 1;
  pushWord(data);
	stackNow = getMcuStackPtr();
	return getCodePtr(k*2) - codePtr;
}

/**
 * Instruction:
 * 		RCALL k
 *		1101 kkkk kkkk kkkk
 *
 * where
 *    -2K <= k <= 2K
 */
int rcall(uint8_t *codePtr)
{
  uint16_t stackBefore, stackNow, data;
	int k;

  stackBefore = *(uint16_t *)spl;
	k = ((codePtr[1] & 0xf) << 8) | (codePtr[0] & 0xff);

  if((k & 0x800) >> 11)
    k |= 0xfffff000;

	//data = (sram[*(uint16_t *)spl] | (sram[*(uint16_t *)spl - 1]) << 8) + 1;
	pushWord((getPc(codePtr) + 1) * 2);
  stackNow = getMcuStackPtr();
  
	return (k+1)*2;
}

/**
 * Instruction:
 * 		ICALL None
 *		1001 0101 0000 1001
 */
int icall(uint8_t *codePtr)
{
	uint16_t stackBefore, stackNow, data;
  int pc;

	stackBefore = *(uint16_t *)spl;
  pc = *zRegPtr;

	data = (sram[*(uint16_t *)spl] | (sram[*(uint16_t *)spl - 1]) << 8) + 1;
	pushWord(data);
  stackNow = getMcuStackPtr();

	return pc;
}

/**
 * Instruction:
 * 		EICALL None
 *		1001 0101 0001 1001
 */
int eicall(uint8_t *codePtr)
{
  int pc;
	uint16_t stackBefore, stackNow, data;

	stackBefore = *(uint16_t *)spl;
  pc = *zRegPtr | ((*(uint32_t *)eind & 0x3f) << 16);

  data = (sram[*(uint16_t *)spl] | (sram[*(uint16_t *)spl - 1]) << 8) + 1;
	pushWord(data);
  stackNow = getMcuStackPtr();

	return pc;
}

/**
 * Instruction:
 * 		RET None
 *		1001 0101 0000 1000
 */
int ret(uint8_t *codePtr)
{
  int pc;
	
	popWord();
  pc = sram[*(uint16_t *)spl] | (sram[*(uint16_t *)spl - 1]) << 8;
	
	return (pc * 2);
}

/**
 * Instruction:
 * 		RETI None
 *		1001 0101 0001 1000
 */
int reti(uint8_t *codePtr)
{
  int pc;
	
	popWord();	
  pc = sram[*(uint16_t *)spl];

  sreg->I = 1;

	return (pc * 2);
}

/**
 * Instruction:
 * 		CBI A, b
 *		1001 1000 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 */
int cbi(uint8_t *codePtr)
{
	uint8_t A, b;

  A = (*codePtr & 0xf8) >> 3;
  b = *codePtr & 0x7;
  io[A] = io[A] & ~(1 << b);

	return 2;
}

/**
 * Instruction:
 * 		SBI A, b
 *		1001 1010 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 */
int sbi(uint8_t *codePtr)
{
	uint8_t A, b;

  A = (*codePtr & 0xf8) >> 3;
  b = *codePtr & 0x7;
  io[A] = (io[A] | (1 << b));

	return 2;
}

/**
 * Instruction:
 * 		MOVW Rd + 1:Rd, Rr + 1:Rr
 *		0000 0001 dddd rrrr
 * where
 * 		dddd is {
 *			0000 => 0,
 *			0001 => 2,
 *			0010 => 4,
 *			0011 => 6,
 *			0100 => 8,
 *			0101 => 10,
 *			0110 => 12,
 *			0111 => 14,
 *			1000 => 16,
 *			1001 => 18,
 *			1010 => 20,
 *			1011 => 22,
 *			1100 => 24,
 *			1101 => 26,
 *			1110 => 28,
 *			1111 => 30,
 *		}
 *
 * 		rrrr is {
 *			0000 => 0,
 *			0001 => 2,
 *			0010 => 4,
 *			0011 => 6,
 *			0100 => 8,
 *			0101 => 10,
 *			0110 => 12,
 *			0111 => 14,
 *			1000 => 16,
 *			1001 => 18,
 *			1010 => 20,
 *			1011 => 22,
 *			1100 => 24,
 *			1101 => 26,
 *			1110 => 28,
 *			1111 => 30,
 *		}
 */
int movw(uint8_t *codePtr)
{
	uint8_t rd, rr;

	rd = ((*codePtr & 0xf0) >> 4) * 2;
	rr = (*codePtr & 0xf) * 2;

	r[rd] = r[rr];
  r[rd+1] = r[rr+1];

	return 2;
}

/**
 * Instruction:
 * 		IN Rd, A
 *		1011 0AAd dddd AAAA
 * where
 *		0 <= ddddd <= 31
 *    0 <= AAAAAA <= 63
 */
int in(uint8_t *codePtr)
{
	uint8_t rd, A;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	A = ((codePtr[1] & 0x6) << 3) | (codePtr[0] & 0xf);

	r[rd] = io[A];

	return 2;
}

/**
 * Instruction:
 * 		OUT A, Rr
 *		1011 1AAr rrrr AAAA
 * where
 *		0 <= rrrrr <= 31
 *    0 <= AAAAAA <= 63
 */
int out(uint8_t *codePtr)
{
	uint8_t rr, A;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	A = ((codePtr[1] & 0x6) << 3) | (codePtr[0] & 0xf);

	io[A] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, X
 *		1001 000d dddd 1100
 * where
 *		0 <= ddddd <= 31
 */
int ldxUnchanged(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*xRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, X+
 *		1001 000d dddd 1101
 * where
 *		0 <= ddddd <= 31
 */
int ldxPostInc(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*xRegPtr];
  *xRegPtr = *xRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, -X
 *		1001 000d dddd 1110
 * where
 *		0 <= ddddd <= 31
 */
int ldxPreDec(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*xRegPtr = *xRegPtr - 1;
	r[rd] = sram[*xRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, Y
 *		1000 000d dddd 1000
 * where
 *		0 <= ddddd <= 31
 */
int ldyUnchanged(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*yRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, Y+
 *		1001 000d dddd 1001
 * where
 *		0 <= ddddd <= 31
 */
int ldyPostInc(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*yRegPtr];
  *yRegPtr = *yRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, -Y
 *		1001 000d dddd 1010
 * where
 *		0 <= ddddd <= 31
 */
int ldyPreDec(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*yRegPtr = *yRegPtr - 1;
	r[rd] = sram[*yRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LDD Rd, Y+q
 *		10q0 qq0d dddd 1qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 */
int lddy(uint8_t *codePtr)
{
	uint8_t rd, q;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	q = (codePtr[1] & 0x20) | ((codePtr[1] & 0xc) << 1) | (codePtr[0] & 0x7);

	r[rd] = sram[*yRegPtr + q];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, Z
 *		1000 000d dddd 0000
 * where
 *		0 <= ddddd <= 31
 */
int ldzUnchanged(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, Z+
 *		1001 000d dddd 0001
 * where
 *		0 <= ddddd <= 31
 */
int ldzPostInc(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = sram[*zRegPtr];
  *zRegPtr = *zRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		LD Rd, -Z
 *		1001 000d dddd 0010
 * where
 *		0 <= ddddd <= 31
 */
int ldzPreDec(uint8_t *codePtr)
{
	uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*zRegPtr = *zRegPtr - 1;
	r[rd] = sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LDD Rd, Z+q
 *		10q0 qq0d dddd 0qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 */
int lddz(uint8_t *codePtr)
{
	uint8_t rd, q;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	q = (codePtr[1] & 0x20) | ((codePtr[1] & 0xc) << 1) | (codePtr[0] & 0x7);

	r[rd] = sram[*zRegPtr + q];

	return 2;
}

/**
 * Instruction:
 * 		ST X, Rr
 *		1001 001r rrrr 1100
 * where
 *		0 <= ddddd <= 31
 */
int stxUnchanged(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*xRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		ST X+, Rr
 *		1001 001r rrrr 1101
 * where
 *		0 <= ddddd <= 31
 */
int stxPostInc(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*xRegPtr] = r[rr];
	*xRegPtr = *xRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		ST -X, Rr
 *		1001 001r rrrr 1110
 * where
 *		0 <= ddddd <= 31
 */
int stxPreDec(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*xRegPtr = *xRegPtr - 1;
	sram[*xRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		ST Y, Rr
 *		1000 001r rrrr 1000
 * where
 *		0 <= ddddd <= 31
 */
int styUnchanged(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*yRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		ST Y+, Rr
 *		1001 001r rrrr 1001
 * where
 *		0 <= ddddd <= 31
 */
int styPostInc(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*yRegPtr] = r[rr];
	*yRegPtr = *yRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		ST -Y, Rr
 *		1001 001r rrrr 1010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st -Y, 11
 *		1001 0010 1011 1010
 *      9 	   2      b     a
 */
int styPreDec(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*yRegPtr = *yRegPtr - 1;
	sram[*yRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		STD Y+q, Rr
 *		10q0 qq1r rrrr 1qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 */
int stdy(uint8_t *codePtr)
{
	uint8_t rr, q;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	q = (codePtr[1] & 0x20) | ((codePtr[1] & 0xc) << 1) | (codePtr[0] & 0x7);

	sram[*yRegPtr + q] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		ST Z, Rr
 *		1000 001r rrrr 0000
 * where
 *		0 <= ddddd <= 31
 */
int stzUnchanged(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		ST Z+, Rr
 *		1001 001r rrrr 0001
 * where
 *		0 <= ddddd <= 31
 */
int stzPostInc(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = r[rr];
	*zRegPtr = *zRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		ST -Z, Rr
 *		1001 001r rrrr 0010
 * where
 *		0 <= ddddd <= 31
 */
int stzPreDec(uint8_t *codePtr)
{
	uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	*zRegPtr = *zRegPtr - 1;
	sram[*zRegPtr] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		STD Z+q, Rr
 *		10q0 qq1r rrrr 0qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 */
int stdz(uint8_t *codePtr)
{
	uint8_t rr, q;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	q = (codePtr[1] & 0x20) | ((codePtr[1] & 0xc) << 1) | (codePtr[0] & 0x7);

	sram[*zRegPtr + q] = r[rr];

	return 2;
}

/**
 * Instruction:
 * 		LPM None
 *		1001 0101 1100 1000
 */
int lpmUnchangeR0(uint8_t *codePtr)
{
	r[0] = flashMemory[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LPM Rd, Z
 *		1001 000d dddd 0100
 * where
 *		0 <= ddddd <= 31
 */
int lpmUnchange(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);
	r[rd] = flashMemory[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LPM Rd, Z+
 *		1001 000d dddd 0101
 * where
 *		0 <= ddddd <= 31
 */
int lpmPostInc(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	r[rd] = flashMemory[*zRegPtr];
	*zRegPtr = *zRegPtr + 1;

	return 2;
}

/**
 * Instruction:
 * 		LAT Z, Rd
 *		1001 000r rrrr 0111
 * where
 *		0 <= ddddd <= 31
 */
int lat(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = r[rd] ^ sram[*zRegPtr];
  r[rd] = sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LAS Z, Rd
 *		1001 001r rrrr 0101
 * where
 *		0 <= ddddd <= 31
 */
int las(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = r[rd] | sram[*zRegPtr];
  r[rd] = sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		LAC Z, Rd
 *		1001 001r rrrr 0101
 * where
 *		0 <= ddddd <= 31
 */
int lac(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = (0xff - r[rd]) & sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		XCH Z, Rd
 *		1001 001d dddd 0100
 * where
 *		0 <= ddddd <= 31
 */
int xch(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

	sram[*zRegPtr] = r[rd];
  r[rd] = sram[*zRegPtr];

	return 2;
}

/**
 * Instruction:
 * 		PUSH Rr
 *		1001 001r rrrr 1111
 * where
 *		0 <= rrrrr <= 31
 */
int push(uint8_t *codePtr)
{
  uint8_t rr;

	rr = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

  sram[*(uint16_t *)spl] = r[rr];
  substractStackPointer(1);

  return 2;
}

/**
 * Instruction:
 * 		POP Rd
 *		1001 000d dddd 1111
 * where
 *		0 <= ddddd <= 31
 */
int pop(uint8_t *codePtr)
{
  uint8_t rd;

	rd = ((codePtr[1] & 0x1) << 4) | ((codePtr[0] & 0xf0) >> 4);

  r[rd] = sram[*(uint16_t *)spl];
  substractStackPointer(-1);

  return 2;
}

/**
 * Instruction:
 * 		FMUL Rd, Rr
 *		0000 0011 0ddd 1rrr
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
int fmul(uint8_t *codePtr)
{
	uint8_t rd, rr;
	uint16_t result;

	rd = ((*codePtr & 0x70) >> 4) + 16;
	rr = (*codePtr & 0x7) + 16;

	result = (r[rd] * r[rr]) << 1;
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
}

/**
 * Instruction:
 * 		FMULS Rd, Rr
 *		0000 0011 1ddd 0rrr
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
int fmuls(uint8_t *codePtr)
{
	uint8_t rd, rr;
	int16_t result;

	rd = ((*codePtr & 0x70) >> 4) + 16;
	rr = (*codePtr & 0x7) + 16;

	result = ((int8_t)r[rd] * (int8_t)r[rr]) << 1;
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
}

/**
 * Instruction:
 * 		FMULSU Rd, Rr
 *		0000 0011 1ddd 1rrr
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
int fmulsu(uint8_t *codePtr)
{
	uint8_t rd, rr;
	int16_t result;

	rd = ((*codePtr & 0x70) >> 4) + 16;
	rr = (*codePtr & 0x7) + 16;

	result = ((int8_t)r[rd] * (int8_t)r[rr]) << 1;
	r[0] = result;
	r[1] = (result & 0xff00) >> 8;
	handleStatusRegForMulMulsMulsuOperation(result);
	return 2;
}
