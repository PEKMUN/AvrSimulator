#include <stdio.h>
#include "unity.h"
#include "Simulator.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

void setUp(void){}

void tearDown(void){}

/**
 * Instruction:
 * 		NOP None
 *			0000 0000 0000 0000
 *       0    0    0    0
 */
void test_AvrOperatorTable_given_nop(void)
{
	uint8_t pc;
	uint8_t codeMemory[] = {
		0x00, 0x00,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	pc = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, pc);
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
 * 
 * Simulate muls R21, R20
 *		    dddd = b'0101' ==> 21
 *		    rrrr = b'0100' ==> 20
 *		0000 0010 0101 0100
 *		  0      2      5     4
 */
void test_AvrOperatorTable_given_muls_r21_r20(void)
{
	uint8_t codeMemory[] = {
		0x54, 0x02,
	};
	uint8_t *progCounter = codeMemory;
	
	r[21] = -32;
	r[20] = -10;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x1, r[1]);
	TEST_ASSERT_EQUAL(0x40, r[0]);
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
 * 
 * Simulate mulsu R23, R20
 *		    ddd = b'111' ==> 21
 *		    rrr = b'100' ==> 20
 *		0000 0010 0111 0100
 *		  0      2      7     4
 */
void test_AvrOperatorTable_given_mulsu_r23_r20(void)
{
	uint8_t codeMemory[] = {
		0x74, 0x02,
	};
	uint8_t *progCounter = codeMemory;
	
	r[23] = -32;
	r[20] = 10;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xfe, r[1]);
	TEST_ASSERT_EQUAL(0xc0, r[0]);
}

/**
 * Instruction:
 * 		CPC Rd, Rr
 *			0000 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * Simulate cpc R17, R23
 *			0000 0111 0001 0111
 *			 0 	  7    1  	7
 */
void test_AvrOperatorTable_given_cpc_r17_r23(void)
{
  uint8_t codeMemory[] = {
		0x17, 0x07,
	};
	uint8_t *progCounter = codeMemory;
	
	r[17] = 0x9c;
	r[23] = 0x52;
	sreg->C = 0;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		SBC Rd,Rr
 *		0000 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate sbc R9,R16
 * 		ddddd = 9 = b'01001'
 * 		rrrrr = 16 = b'10000'
 *		0000 1010 1001 0000
 *		  0     a     9    0
 */
void test_AvrOperatorTable_given_sbc_r9_r16(void)
{
  uint8_t codeMemory[] = {
		0x90, 0x0a,
	};
	uint8_t *progCounter = codeMemory;
	
	sreg->C = 1;
	
	r[9]  = 0x9c;
	r[16] = 0x52;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x49, r[9]);
	TEST_ASSERT_EQUAL(0x52, r[16]);
}

/**
 * Instruction:
 * 		ADD Rd,Rr
 *		0000 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate add R4,R7
 * 		ddddd = 4 = b'00100'
 * 		rrrrr = 7 = b'00111'
 *		0000 1100 0100 0111
 *		  0    c    4    7
 */
void test_AvrOperatorTable_given_add_r4_r7(void)
{
    uint8_t codeMemory[] = {
		0x47, 0x0c,
	};
	uint8_t *progCounter = codeMemory;
	
	r[7] = 23;
	r[4] = 5;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(23, r[7]);
	TEST_ASSERT_EQUAL(28, r[4]);
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
 *
 * Simulate cpse r14, r15
 *			0001 0010 1110 1111
 *			 1 	  2    e  	f
 */
void test_AvrOperatorTable_given_cpse_r14_r15(void)
{
  uint8_t pc;
  uint8_t codeMemory[] = {
		0xef, 0x12,
	};
	uint8_t *progCounter = codeMemory;
	
  r[14] = 0x12;
  r[15] = 0x10;
  
	pc = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, pc);
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
 *
 * Simulate cpse r16, r17
 *			0001 0011 0000 0001
 *			 1 	  3    0  	1
 */
void test_AvrOperatorTable_given_cpse_r16_r17(void)
{
  uint8_t pc;
  uint8_t codeMemory[] = {
    //start:
		0x01, 0x13,      //cpse r16, r17
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
	
  r[16] = 0x12;
  r[17] = 0x12;
  
	pc = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(4, pc);
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
 *
 * Simulate cpse r18, r19
 *			0001 0011 0010 0011
 *			 1 	  3    2  	3
 */
void test_AvrOperatorTable_given_cpse_r18_r19(void)
{
  uint8_t pc;
  uint8_t codeMemory[] = {
    //start:
		0x23, 0x13,                   //cpse r16, r17
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
	
  r[18] = 0x12;
  r[19] = 0x12;
  
	pc = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, pc);
}

/**
 * Instruction:
 * 		CP Rd, Rr
 *			0001 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * Simulate cp R4, R21
 *			0001 0110 0100 0101
 *			 1 	  6    4  	5
 */
void test_AvrOperatorTable_given_cp_r4_r21(void)
{
  uint8_t codeMemory[] = {
		0x45, 0x16,
	};
	uint8_t *progCounter = codeMemory;
	
	r[4]  = 0x9c;
	r[21] = 0x52;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		SUB Rd,Rr
 *		0001 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate sub R15,R21
 * 		ddddd = 15 = b'01111'
 * 		rrrrr = 21 = b'10101'
 *		0001 1010 1111 0101
 *		  1     a     f     5
 */
void test_AvrOperatorTable_given_sub_r15_r21(void)
{
    uint8_t codeMemory[] = {
		0xf5, 0x1a,
	};
	uint8_t *progCounter = codeMemory;
	
	r[15] = 23;
	r[21] = 5;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(18, r[15]);
	TEST_ASSERT_EQUAL(5, r[21]);
}

/**
 * Instruction:
 * 		ADC Rd,Rr
 *		0001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate add R5,R9
 * 		ddddd = 5 = b'00101'
 * 		rrrrr = 9 = b'01001'
 *		0001 1100 0101 1001
 *		  0    c    5    9
 */
void test_AvrOperatorTable_given_adc_r5_r9(void)
{
    uint8_t codeMemory[] = {
		0x59, 0x1c,
	};
	uint8_t *progCounter = codeMemory;
	
	sreg->C = 1;
	
	r[5] = 0x7d;
	r[9] = 0x1c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x9a, r[5]);
	TEST_ASSERT_EQUAL(0x1c, r[9]);
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate and R2,R3
 * 		ddddd = 2 = b'00010'
 * 		rrrrr = 3 = b'00011'
 *		0010 0000 0010 0011
 *		  2    0    2    3
 */
void test_AvrOperatorTable_given_and_r2_r3(void)
{
	uint8_t codeMemory[] = {
		0x23, 0x20,
	};
	uint8_t *progCounter = codeMemory;
	
	r[2] = 0x5;
	r[3] = 0xf;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x5, r[2]);
	TEST_ASSERT_EQUAL(0xf, r[3]);
}

/**
 * Instruction:
 * 		EOR Rd,Rr
 *		0010 01rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate eor R16,R10
 * 		ddddd = 16 = b'10000'
 * 		rrrrr = 10 = b'01010'
 *		0010 0101 0000 1010
 *		  2     5     0     a
 */
void test_AvrOperatorTable_given_eor_r16_r10(void)
{
    uint8_t codeMemory[] = {
		0x0a, 0x25,
	};
	uint8_t *progCounter = codeMemory;
	
	r[16] = 0x47;
	r[10] = 0xe3;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xa4, r[16]);
	TEST_ASSERT_EQUAL(0xe3, r[10]);
}

/**
 * Instruction:
 * 		OR Rd,Rr
 *		0010 10rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate or R2,R8
 * 		ddddd = 2 = b'00010'
 * 		rrrrr = 8 = b'01000'
 *		0010 1000 0010 1000
 *		  2     8     2     8
 */
void test_AvrOperatorTable_given_or_r2_r8(void)
{
    uint8_t codeMemory[] = {
		0x28, 0x28,
	};
	uint8_t *progCounter = codeMemory;
	
	r[2] = 0xe;
	r[8] = 0x18;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x1e, r[2]);
	TEST_ASSERT_EQUAL(0x18, r[8]);
}