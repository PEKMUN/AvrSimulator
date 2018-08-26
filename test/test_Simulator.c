#include <stdio.h>
#include "unity.h"
#include "Simulator.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

void setUp(void){}

void tearDown(void){}

void test_is2wordInstruction_given_codePtr_is_0(void)
{
	uint8_t code;
  uint8_t codeMemory[] = {
		0xef, 0x12,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
	code = 0x14;
	
	code = is2wordInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, code);
}

void test_pushWord_given_spl_0x4d_sph_0x5(void)
{
  *spl = 0x4d;
  *sph = 0x5;
  
  pushWord(0x1234);
  
  TEST_ASSERT_EQUAL(0x12, sram[getMcuStackPtr()+1]);
  TEST_ASSERT_EQUAL(0x34, sram[getMcuStackPtr()+2]);
}

void test_popWord_given_data_0x1234(void)
{
	uint32_t x;
	*spl = 0xfd;
  *sph = 0x8;
  pushWord(0x1234);
  
	x = popWord();
	
  TEST_ASSERT_EQUAL_HEX32(0x1234, x);
}

/**
 * Instruction:
 * 		NOP None
 *			0000 0000 0000 0000
 *       0    0    0    0
 */
void test_AvrOperatorTable_given_nop(void)
{
	int relAddr;
	uint8_t codeMemory[] = {
		0x00, 0x00,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
 * 
 * Simulate fmul R18, R20
 *		    ddd = b'010' ==> 18
 *		    rrr = b'100' ==> 20
 *		0000 0011 0010 1100
 *		   0      3      2     	c
 *
 *										0000 1101								13
 *								x	0000 1001					x		9
 *	--------------------------					------
 *	0000 0000 0111 0101							117
 * --------------------------					------
 */
void test_AvrOperatorTable_given_fmul_r18_r20(void)
{
	uint8_t codeMemory[] = {
		0x2c, 0x03,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[18] = 0xd;
	r[20] = 0x9;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x0, r[1]);
	TEST_ASSERT_EQUAL(0xea, r[0]);
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
 * 
 * Simulate fmuls R18, R20
 *		    ddd = b'010' ==> 18
 *		    rrr = b'100' ==> 20
 *		0000 0011 1010 0100
 *		   0      3      a     	4
 *
 *							0000 1101              13
 *					x	1111   0111         x	(-	9)
 *	--------------------------    ---------
 *    0000 0000 0111 0101						-117
 * --------------------------     ---------
 */
void test_AvrOperatorTable_given_fmuls_r18_r20(void)
{
	uint8_t codeMemory[] = {
		0xa4, 0x03,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[18] = 0xd;
	r[20] = 0xf7;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xff, r[1]);
	TEST_ASSERT_EQUAL(0x16, r[0]);
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
 * 
 * Simulate fmuls R20, R20
 *		    ddd = b'100' ==> 20
 *		    rrr = b'100' ==> 20
 *		0000 0011 1100 0100
 *		   0      3      c     	4
 *
 *									0000 1001								 9
 *								x	0000 1001					    x	 9
 *	--------------------------					 ------
 *	      0000 0000 0101 0001							  81
 * --------------------------					   ------
 */
void test_AvrOperatorTable_given_fmuls_r20_r20(void)
{
	uint8_t codeMemory[] = {
		0xc4, 0x03,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[20] = 0x9;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x0, r[1]);
	TEST_ASSERT_EQUAL(0xa2, r[0]);
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
 * 
 * Simulate fmuls R16, R20
 *		    ddd = b'000' ==> 16
 *		    rrr = b'100' ==> 20
 *		0000 0011 1000 0100
 *		   0      3      8     	4
 *
 *									1110 1011							  (-21)
 *								x	1111 0111					    x	 (-9)
 *	--------------------------					 --------
 *	      0000 0000 1011 1101							   189
 * --------------------------					   --------
 */
void test_AvrOperatorTable_given_fmuls_r16_r20(void)
{
	uint8_t codeMemory[] = {
		0x84, 0x03,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  r[16] = 0xeb;
	r[20] = 0xf7;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x1, r[1]);
	TEST_ASSERT_EQUAL(0x7a, r[0]);
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
 * 
 * Simulate fmulsu R19, R23
 *		    ddd = b'011' ==> 19
 *		    rrr = b'111' ==> 23
 *		0000 0011 1011 1111
 *		   0      3      b     	f
 *
 *									1110 1011							  (-21)
 *								x	1111 0111					    x	  9
 *	--------------------------					 --------
 *	      0000 0000 1011 1101							  -189
 * --------------------------					   --------
 */
void test_AvrOperatorTable_given_fmulsu_r19_r23(void)
{
	uint8_t codeMemory[] = {
		0xbf, 0x03,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  r[19] = 0xeb;
	r[23] = 0x9;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xfe, r[1]);
	TEST_ASSERT_EQUAL(0x86, r[0]);
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
	r[17] = 0x9c;
	r[23] = 0x52;
	sreg->C = 0;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
  int relAddr;
  uint8_t codeMemory[] = {
		0xef, 0x12,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  r[14] = 0x12;
  r[15] = 0x10;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x01, 0x13,      //cpse r16, r17
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  r[16] = 0x12;
  r[17] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(4, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x23, 0x13,                   //cpse r16, r17
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  r[18] = 0x12;
  r[19] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, relAddr);
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
	flash = codeMemory;
	
	r[4]  = 0x63;
	r[21] = 0x52;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * Instruction:
 * 		CP Rd, Rr
 *			0001 01rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *      0 <= rrrrr <= 31
 *
 * Simulate cp R13, R14
 *			0001 0100 1101 1110
 *			  1			 4			d				e
 */
void test_AvrOperatorTable_given_cp_r13_r14(void)
{
  uint8_t codeMemory[] = {
		0xde, 0x14,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[13] = 0x30;
	r[14] = 0x4c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
	r[2] = 0xe;
	r[8] = 0x18;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x1e, r[2]);
	TEST_ASSERT_EQUAL(0x18, r[8]);
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
 * 
 * Simulate adiw R27,26, 13
 * 		KKKKKK = 13 = 0xd = b'001101'
 *		    dd = b'01' ==> 26
 *		1001 0110 0001 1101
 *		  9    6    1    d
 */
void test_AvrOperatorTable_given_adiw_r27_r26_13(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x1d, 0x96,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[26] = 24;
	r[27] = 10;

	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(37, r[26]);
	TEST_ASSERT_EQUAL(10, r[27]);
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
 * 
 * Simulate adiw R31,30, 2
 * 		KKKKKK = 2 = 0x2 = b'000010'
 *		    dd = b'11' ==> 30
 *		1001 0110 0011 0010
 *		  9    6    3    2
 */
void test_AvrOperatorTable_given_adiw_r31_r30_2(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x32, 0x96,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[30] = 0xff;
	r[31] = 0x10;

	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x01, r[30]);
	TEST_ASSERT_EQUAL(0x11, r[31]);
}

/**
 * Instruction:
 * 		AND Rd,Rr
 *		0010 00rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate and R4,R31
 * 		ddddd = 4  = b'00100'
 * 		rrrrr = 31 = b'11111'
 *		0010 0010 0100 1111
 *		  2    2    4    f
 */
void test_AvrOperatorTable_given_and_r4_r31(void)
{
	uint8_t codeMemory[] = {
		0x4f, 0x22,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[4]  = 0x75;
	r[31] = 0x78;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x70, r[4]);
	TEST_ASSERT_EQUAL(0x78, r[31]);
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
 * 
 * Simulate andi R20, 42
 * 		KKKKKKKK = 42 = b'00101010'
 *		    dddd = b'0100' ==> 20
 *		0111 0010 0100 1010
 *		  7    2    4    a
 */
void test_AvrOperatorTable_given_andi_r20_42(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x4a, 0x72,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[20] = 0xf0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x20, r[20]);
}

/**
 * Instruction:
 * 		SUBI Rd, K
 *		0101 KKKK dddd KKKK
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
 * 
 * Simulate subi R25, 201
 * 		KKKKKKKK = 201 = b'11001001'
 *		    dddd = b'1001' ==> 25
 *		0101 1100 1001 1001
 *		  5     c     9     9
 */
void test_AvrOperatorTable_given_subi_r25_201(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x99, 0x5c,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[25] = 0xf0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x27, r[25]);
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
 * 
 * Simulate sbci R29, 155
 * 		KKKKKKKK = 155 = b'10011011'
 *		    dddd = b'1101' ==> 29
 *		0100 1001 1101 1011
 *		  4     9     d     b
 */
void test_AvrOperatorTable_given_sbci_r29_155(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xdb, 0x49,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	sreg->C = 1;
	
	r[29] = 0xff;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x63, r[29]);
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
 * 
 * Simulate sbiw R29,28, 54
 * 		KKKKKK = 54 = 0x36 = b'110110'
 *		    dd = b'10' ==> 28
 *		1001 0111 1110 0110
 *		  9     7     e     6
 */
void test_AvrOperatorTable_given_sbiw_r29_r28_54(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xe6, 0x97,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[28] = 0x28;
	r[29] = 0x10;

	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xf2, r[28]);
	TEST_ASSERT_EQUAL(0xf, r[29]);
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
 * 
 * Simulate ori R23, 78
 * 		KKKKKKKK = 78 = b'01001110'
 *		    dddd = b'0111' ==> 23
 *		0110 0100 0111 1110
 *		  6     4     7     e
 */
void test_AvrOperatorTable_given_ori_r23_78(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x7e, 0x64,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[23] = 0x63;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x6f, r[23]);
}

/**
 * Instruction:
 * 		COM Rd
 *		1001 010d dddd 0000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate com R9
 * 		ddddd = 9 = b'01001'
 *		1001 0100 1001 0000
 *		  9     4      9      0
 */
void test_AvrOperatorTable_given_com_r9(void)
{
    uint8_t codeMemory[] = {
		0x90, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[9] = 0x66;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x99, r[9]);
}

/**
 * Instruction:
 * 		NEG Rd
 *		1001 010d dddd 0001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate neg R9
 * 		ddddd = 9 = b'01001'
 *		1001 0100 1001 0001
 *		  9     4      9      1
 */
void test_AvrOperatorTable_given_neg_r9(void)
{
    uint8_t codeMemory[] = {
		0x91, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[9] = 0x1;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xff, r[9]);
}

/**
 * Instruction:
 * 		DEC Rd
 *		1001 010d dddd 1010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate dec R16
 * 		ddddd = 16 = b'10000'
 *		1001 0101 0000 1010
 *		  9     5     0      a
 */
void test_AvrOperatorTable_given_dec_r16(void)
{
    uint8_t codeMemory[] = {
		0x0a, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[16] = 0x9b;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x9a, r[16]);
}

/**
 * Instruction:
 * 		INC Rd
 *		1001 010d dddd 0011
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate inc R16
 * 		ddddd = 16 = b'10000'
 *		1001 0101 0000 0011
 *		  9     5      0      3
 */
void test_AvrOperatorTable_given_inc_r16(void)
{
    uint8_t codeMemory[] = {
		0x03, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[16] = 0x9b;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x9c, r[16]);
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
 * 
 * Simulate sbr R31, 240
 * 		KKKKKKKK = 240 = b'11110000'
 *		    dddd = b'1111' ==> 31
 *		0110 1111 1111 0000
 *		  6     f      f     0
 */
void test_AvrOperatorTable_given_sbr_r31_240(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0xf0, 0x6f,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[31] = 0xe6;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xf6, r[31]);
}

/**
 * Instruction:
 * 		TST Rd
 *		0010 00dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate tst R18
 * 		dddddddddd = 18 = b'0000010010'
 *		0010 0000 0001 0010
 *		  2     0     1      2
 */
void test_AvrOperatorTable_given_tst_r18(void)
{
	uint8_t codeMemory[] = {
		0x12, 0x20,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[18] = 0x3b;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x3b, r[18]);
}

/**
 * Instruction:
 * 		CLR Rd
 *		0010 01dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate clr R3
 * 		dddddddddd = 3 = b'0000000011'
 *		0010 0100 0000 0011
 *		  2      4     0     3
 */
void test_AvrOperatorTable_given_clr_r3(void)
{
	uint8_t codeMemory[] = {
		0x03, 0x24,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[3] = 0x7c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x7c, r[3]);
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
 * 
 * Simulate ser R22
 *		    dddd = b'0110' ==> 22
 *		1110 1111 0110 1111
 *		  e     f      6     f
 */
void test_AvrOperatorTable_given_ser_r22(void)
{
	uint8_t codeMemory[] = {
		0x6f, 0xef,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[22] = 0x87;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xff, r[22]);
}

/**
 * Instruction:
 * 		MUL Rd,Rr
 *		1001 11rd dddd rrrr
 * where
 *		0 <= ddddd <= 31
 *		0 <= rrrrr <= 31
 *
 * Simulate mul R4,R5
 * 		ddddd = 4 = b'00100'
 * 		rrrrr = 5 = b'00101'
 *		1001 1100 0100 0101
 *		  9      c     4     5
 */
void test_AvrOperatorTable_given_mul_r4_r5(void)
{
    uint8_t codeMemory[] = {
		0x45, 0x9c,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[4] = 0x34;
	r[5] = 0x6d;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x16, r[1]);
	TEST_ASSERT_EQUAL(0x24, r[0]);
}

/**
 * Instruction:
 * 		LSL Rd
 *		0000 11dd dddd dddd
 * where
 *		0 <= dddddddddd <= 31
 *
 * Simulate lsl R0
 * 		dddddddddd = 0 = b'0000000000'
 *		0000 1100 0000 0000
 *		  0      c     0      0
 */
void test_AvrOperatorTable_given_lsl_r0(void)
{
	uint8_t codeMemory[] = {
		0x00, 0x0c,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[0] = 0x97;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x2e, r[0]);
}

/**
 * Instruction:
 * 		LSR Rd
 *		1001 010d dddd 0110
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lsr R1
 * 		ddddd = 1 = b'00001'
 *		1001 0100 0001 0110
 *		  9      4     1      6
 */
void test_AvrOperatorTable_given_lsr_r1(void)
{
	uint8_t codeMemory[] = {
		0x16, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[1] = 0x91;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x48, r[1]);
	TEST_ASSERT_EQUAL(1, sreg->C);
}

/**
 * Instruction:
 * 		ROR Rd
 *		1001 010d dddd 0111
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R3
 * 		ddddd = 3 = b'00011'
 *		1001 0100 0011 0111
 *		  9      4     3      7
 */
void test_AvrOperatorTable_given_ror_r3(void)
{
	uint8_t codeMemory[] = {
		0x37, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[3] = 0x90;
	
	sreg->C = 1;
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xc8, r[3]);
}

/**
 * Instruction:
 * 		ASR Rd
 *		1001 010d dddd 0101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R3
 * 		ddddd = 4 = b'00100'
 *		1001 0100 0100 0101
 *		  9      4     4      5
 */
void test_AvrOperatorTable_given_asr_r4(void)
{
	uint8_t codeMemory[] = {
		0x45, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[4] = 0x90;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xc8 ,r[4]);
}

/**
 * Instruction:
 * 		SWAP Rd
 *		1001 010d dddd 0010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ror R7
 * 		ddddd = 7 = b'00111'
 *		1001 0100 0111 0010
 *		  9      4     7      2
 */
void test_AvrOperatorTable_given_swap_r7(void)
{
	uint8_t codeMemory[] = {
		0x72, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[7] = 0x94;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x49, r[7]);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 0
 * 		sss = 0 = b'000'
 *		1001 0100 0000 1000
 *		  9      4      0      8
 */
void test_AvrOperatorTable_given_bset_0(void)
{
	uint8_t codeMemory[] = {
		0x08, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 1
 * 		sss = 1 = b'001'
 *		1001 0100 0001 1000
 *		  9      4      1      8
 */
void test_AvrOperatorTable_given_bset_1(void)
{
	uint8_t codeMemory[] = {
		0x18, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->Z);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 2
 * 		sss = 2 = b'010'
 *		1001 0100 0010 1000
 *		  9      4      2      8
 */
void test_AvrOperatorTable_given_bset_2(void)
{
	uint8_t codeMemory[] = {
		0x28, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->N);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 3
 * 		sss = 3 = b'011'
 *		1001 0100 0011 1000
 *		  9      4     3      8
 */
void test_AvrOperatorTable_given_bset_3(void)
{
	uint8_t codeMemory[] = {
		0x38, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->V);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 4
 * 		sss = 4 = b'100'
 *		1001 0100 0100 1000
 *		  9      4      4      8
 */
void test_AvrOperatorTable_given_bset_4(void)
{
	uint8_t codeMemory[] = {
		0x48, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 5
 * 		sss = 5 = b'101'
 *		1001 0100 0101 1000
 *		  9      4      5      8
 */
void test_AvrOperatorTable_given_bset_5(void)
{
	uint8_t codeMemory[] = {
		0x58, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 6
 * 		sss = 6 = b'110'
 *		1001 0100 0110 1000
 *		  9      4      6      8
 */
void test_AvrOperatorTable_given_bset_6(void)
{
	uint8_t codeMemory[] = {
		0x68, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->T);
}

/**
 * Instruction:
 * 		BSET s
 *		1001 0100 0sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bset 7
 * 		sss = 7 = b'011'
 *		1001 0100 0111 1000
 *		  9      4     7      8
 */
void test_AvrOperatorTable_given_bset_7(void)
{
	uint8_t codeMemory[] = {
		0x78, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->I);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 0
 * 		sss = 0 = b'000'
 *		1001 0100 1000 1000
 *		  9      4      8      8
 */
void test_AvrOperatorTable_given_bclr_0(void)
{
	uint8_t codeMemory[] = {
		0x88, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 1
 * 		sss = 1 = b'001'
 *		1001 0100 1001 1000
 *		  9      4      9      8
 */
void test_AvrOperatorTable_given_bclr_1(void)
{
	uint8_t codeMemory[] = {
		0x98, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->Z);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 2
 * 		sss = 2 = b'010'
 *		1001 0100 1010 1000
 *		  9      4      a      8
 */
void test_AvrOperatorTable_given_bclr_2(void)
{
	uint8_t codeMemory[] = {
		0xa8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->N);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 3
 * 		sss = 3 = b'011'
 *		1001 0100 1011 1000
 *		  9      4     b      8
 */
void test_AvrOperatorTable_given_bclr_3(void)
{
	uint8_t codeMemory[] = {
		0xb8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->V);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 4
 * 		sss = 4 = b'100'
 *		1001 0100 1100 1000
 *		  9      4      c      8
 */
void test_AvrOperatorTable_given_bclr_4(void)
{
	uint8_t codeMemory[] = {
		0xc8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 5
 * 		sss = 5 = b'101'
 *		1001 0100 1101 1000
 *		  9      4      d      8
 */
void test_AvrOperatorTable_given_bclr_5(void)
{
	uint8_t codeMemory[] = {
		0xd8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 6
 * 		sss = 6 = b'110'
 *		1001 0100 1110 1000
 *		  9      4      e      8
 */
void test_AvrOperatorTable_given_bclr_6(void)
{
	uint8_t codeMemory[] = {
		0xe8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->T);
}

/**
 * Instruction:
 * 		BCLR s
 *		1001 0100 1sss 1000
 * where
 *		0 <= sss <= 7
 *
 * Simulate bclr 7
 * 		sss = 7 = b'111'
 *		1001 0100 1111 1000
 *		  9      4     f      8
 */
void test_AvrOperatorTable_given_bclr_7(void)
{
	uint8_t codeMemory[] = {
		0xf8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->I);
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bst R6, 3
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'011'
 *		1111 1010 0110 0011
 *		  f      a     6      3
 */
void test_AvrOperatorTable_given_bst_r6_3(void)
{
	uint8_t codeMemory[] = {
		0x63, 0xfa,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[6] = 0x94;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->T);
}

/**
 * Instruction:
 * 		BST Rd, b
 *		1111 101d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bst R6, 7
 * 		ddddd = 6 = b'00110'
 *		   bbb = 7 = b'111'
 *		1111 1010 0110 0111
 *		 f    a    6    7
 */
void test_AvrOperatorTable_given_bst_r6_7(void)
{
	uint8_t codeMemory[] = {
		0x67, 0xfa,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[6] = 0x94;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->T);
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bld R6, 3
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'011'
 *		1111 1000 0110 0011
 *		  f      8     6      3
 */
void test_AvrOperatorTable_given_bld_r6_3(void)
{
	uint8_t codeMemory[] = {
		0x63, 0xf8,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[6] = 0x94;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x9c, r[6]);
}

/**
 * Instruction:
 * 		BLD Rd, b
 *		1111 100d dddd 0bbb
 * where
 *		0 <= ddddd <= 31
 *		0 <= bbb <= 7
 *
 * Simulate bld R6, 7
 * 		ddddd = 6 = b'00110'
 *		   bbb = 3 = b'111'
 *		1111 1000 0110 0111
 *		  f      8     6      7
 */
void test_AvrOperatorTable_given_bld_r6_7(void)
{
	uint8_t codeMemory[] = {
		0x67, 0xf8,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[6] = 0x94;
	
	sreg->T = 0;
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x14, r[6]);
}

/**
 * Instruction:
 * 		SEC None
 *		1001 0100 0000 1000
 *
 * Simulate sec
 *		1001 0100 0000 1000
 *		  9      4     0      8
 */
void test_AvrOperatorTable_given_sec(void)
{
	uint8_t codeMemory[] = {
		0x08, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
}

/**
 * Instruction:
 * 		CLC None
 *		1001 0100 1000 1000
 *
 * Simulate clc
 *		1001 0100 1000 1000
 *		  9      4     8      8
 */
void test_AvrOperatorTable_given_clc(void)
{
	uint8_t codeMemory[] = {
		0x88, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
}

/**
 * Instruction:
 * 		SEN None
 *		1001 0100 0010 1000
 *
 * Simulate sen
 *		1001 0100 0010 1000
 *		  9      4     2      8
 */
void test_AvrOperatorTable_given_sen(void)
{
	uint8_t codeMemory[] = {
		0x28, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->N);
}

/**
 * Instruction:
 * 		CLN None
 *		1001 0100 1010 1000
 *
 * Simulate cln
 *		1001 0100 1010 1000
 *		  9      4     a      8
 */
void test_AvrOperatorTable_given_cln(void)
{
	uint8_t codeMemory[] = {
		0xa8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->N);
}

/**
 * Instruction:
 * 		SEZ None
 *		1001 0100 0001 1000
 *
 * Simulate sez
 *		1001 0100 0001 1000
 *		  9      4     1      8
 */
void test_AvrOperatorTable_given_sez(void)
{
	uint8_t codeMemory[] = {
		0x18, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->Z);
}

/**
 * Instruction:
 * 		CLZ None
 *		1001 0100 1001 1000
 *
 * Simulate clz
 *		1001 0100 1001 1000
 *		  9      4     9      8
 */
void test_AvrOperatorTable_given_clz(void)
{
	uint8_t codeMemory[] = {
		0x98, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->Z);
}

/**
 * Instruction:
 * 		SEI None
 *			1001 0100 0111 1000
 *
 * Simulate sei
 *			1001 0100 0111 1000
 *			  9      4     7      8
 */
void test_AvrOperatorTable_given_sei(void)
{
	uint8_t codeMemory[] = {
		0x78, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->I);
}

/**
 * Instruction:
 * 		CLI None
 *			1001 0100 1111 1000
 *
 * Simulate cli
 *			1001 0100 1111 1000
 *			  9      4     f      8
 */
void test_AvrOperatorTable_given_cli(void)
{
	uint8_t codeMemory[] = {
		0xf8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->I);
}

/**
 * Instruction:
 * 		SES None
 *			1001 0100 0100 1000
 *
 * Simulate ses
 *			1001 0100 0100 1000
 *			  9      4     4      8
 */
void test_AvrOperatorTable_given_ses(void)
{
	uint8_t codeMemory[] = {
		0x48, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * Instruction:
 * 		CLS None
 *			1001 0100 1100 1000
 *
 * Simulate cls
 *			1001 0100 1100 1000
 *		 	 9      4     c      8
 */
void test_AvrOperatorTable_given_cls(void)
{
	uint8_t codeMemory[] = {
		0xc8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * Instruction:
 * 		SEV None
 *			1001 0100 0011 1000
 *
 * Simulate sev
 *			1001 0100 0011 1000
 *			  9      4     3      8
 */
void test_AvrOperatorTable_given_sev(void)
{
	uint8_t codeMemory[] = {
		0x38, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->V);
}

/**
 * Instruction:
 * 		CLV None
 *			1001 0100 1011 1000
 *
 * Simulate clv
 *			1001 0100 1011 1000
 *			  9      4     b      8
 */
void test_AvrOperatorTable_given_clv(void)
{
	uint8_t codeMemory[] = {
		0xb8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->V);
}

/**
 * Instruction:
 * 		SET None
 *			1001 0100 0110 1000
 *
 * Simulate set
 *			1001 0100 0110 1000
 *			  9      4     6      8
 */
void test_AvrOperatorTable_given_set(void)
{
	uint8_t codeMemory[] = {
		0x68, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->T);
}

/**
 * Instruction:
 * 		CLT None
 *			1001 0100 1110 1000
 *
 * Simulate clt
 *			1001 0100 1110 1000
 *			  9      4     e      8
 */
void test_AvrOperatorTable_given_clt(void)
{
	uint8_t codeMemory[] = {
		0xe8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->T);
}

/**
 * Instruction:
 * 		SEH None
 *			1001 0100 0101 1000
 *
 * Simulate seh
 *		1001 0100 0101 1000
 *		 9    4    5   8
 */
void test_AvrOperatorTable_given_seh(void)
{
	uint8_t codeMemory[] = {
		0x58, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * Instruction:
 * 		CLH None
 *			1001 0100 1101 1000
 *
 * Simulate clh
 *		1001 0100 1101 1000
 *		  9   4    d    8
 */
void test_AvrOperatorTable_given_clh(void)
{
	uint8_t codeMemory[] = {
		0xd8, 0x94,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * Instruction:
 * 		MOV Rd, Rr
 *			0010 11rd dddd rrrr
 * where
 *			0 <= ddddd <= 31
 *			0 <= rrrrr <= 31
 *
 * Simulate mov
 *			0010 1110 1101 0001
 *			  2       e      d      1
 */
void test_AvrOperatorTable_given_mov_r13_r17(void)
{
	uint8_t codeMemory[] = {
		0xd1, 0x2e,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[17] = 0x54;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x54, r[13]);
}

