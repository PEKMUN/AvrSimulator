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
	flash = codeMemory;
	
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
  uint8_t pc;
  uint8_t codeMemory[] = {
		0xef, 0x12,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
	flash = codeMemory;
	
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
 * Simulate bset 7
 * 		sss = 3 = b'011'
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

/**
 * Instruction:
 * 		RJMP k
 *			1100 kkkk kkkk kkkk
 * where
 *			-2K <= kkkkkkkkkkkk <= 2K
 *
 * Simulate rjmp -2
 *			1100 1111 1111 1110
 *			 c   	 f  	   f  	  e
 */
void test_AvrOperatorTable_given_rjmp_0xffe(void)
{
  uint32_t k;
	uint8_t codeMemory[] = {
		0xfe, 0xcf,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(-2, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 4
 *			1001 0100 0000 1100
 *				9				4			0			c
 *			0000 0000 0000 0100
 *			 	0 	 		  0   	  0  	 	 4
 */
void test_AvrOperatorTable_given_jmp_4(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0x0c, 0x94, 0x04, 0x00, 
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(8, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 1
 *			1001 0100 0000 1100
 *				9				4			0			c
 *			0000 0000 0000 0001
 *			 	0 	 		  0   	  0  	 	 1
 */
void test_AvrOperatorTable_given_jmp_1(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
	  // start: 	
		0x03, 0x95,														// inc r16	
		// here: 	
		0xfe, 0xcf,															// rjmp start
		0x12, 0x2c,															// mov r1, r2
		0x0c, 0x94, 0x01, 0x00,				// jmp here
	};
	uint8_t *progCounter = &codeMemory[6];
	flash = codeMemory;
  
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(-4, k);
}

/**
 * Instruction:
 * 		JMP k
 *			1001 010k kkkk 110k
 *			kkkk kkkk kkkk kkkk
 * where
 *			0 <= kkkkkkkkkkkk <= 4M
 *
 * Simulate jmp 4 * 1024 * 1024 - 2
 *			1001 0101 1111 1101
 *				9			4			f		 d
 *			1111 1111 1111 1110
 *			 	f 	 	 f   	f  	 e
 */
void test_AvrOperatorTable_given_jmp_4x1024x1024minus2(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
	  // start: 	
		0x03, 0x95,														// inc r16	
		// here: 	
		0xfe, 0xcf,															// rjmp start
		0x12, 0x2c,															// mov r1, r2
		0xfd, 0x95, 0xfe, 0xff,				// jmp 4 * 1024 *1024 -2
	};
	uint8_t *progCounter = &codeMemory[6];
	flash = codeMemory;
  
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32((0x3ffffe*2)-0x6, k);
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 4, 61
 *			1111 0001 1110 1100
 *			 f 	  1    e  	c
 */
void test_AvrOperatorTable_given_brbs_4_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xec, 0xf1,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->S = 1;
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(124, k);
}

/**
 * Instruction:
 * 		BRBS s, k
 *			1111 00kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 5, 61
 *			1111 0001 1110 1101
 *			 f 	  1    e  	d
 */
void test_AvrOperatorTable_given_brbs_5_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xed, 0xf1,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->H = 0;
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(2, k);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 5, 61
 *			1111 0101 1110 1101
 *			 f 	  5    e  	d
 */
void test_AvrOperatorTable_given_brbc_5_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xed, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->H = 0;
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(124, k);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbs 4, 61
 *			1111 0101 1110 1100
 *			 f 	  5    e  	c
 */
void test_AvrOperatorTable_given_brbc_4_61(void)
{
	uint32_t k;
	uint8_t codeMemory[] = {
		0xec, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->S = 1;
	k = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(2, k);
}

/**
 * Instruction:
 * 		WDR None
 *			1001 0101 1010 1000
 *       9    5    a    8
 */
void xtest_AvrOperatorTable_given_wdr(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0xa8, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;

	Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect WATCHDOG_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(WATCHDOG_EXCEPTION, e->errorCode);
    freeError(e);
  }
}

/**
 * Instruction:
 * 		BREAK None
 *			1001 0101 1001 1000
 *       9    5    9    8
 */
void xtest_AvrOperatorTable_given_break(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0x98, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect BREAK_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(BREAK_EXCEPTION, e->errorCode);
    freeError(e);
  }
}

/**
 * Instruction:
 * 		SLEEP None
 *			1001 0101 1000 1000
 *       9    5    8    8
 */
void xtest_AvrOperatorTable_given_sleep(void)
{
  CEXCEPTION_T e;
	uint8_t codeMemory[] = {
		0x88, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
  Try {
    simulateOneInstruction(progCounter);
    TEST_FAIL_MESSAGE("Expect SLEEP_EXCEPTION. But no exception thrown.");
  } Catch(e) {
    printf(e->errorMsg);
    TEST_ASSERT_EQUAL(SLEEP_EXCEPTION, e->errorCode);
    freeError(e);
  }
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
 *
 * Simulate cpi R18, 211
 *			0011 1101 0010 0011
 *			 3 	  d    2  	3
 */
void test_AvrOperatorTable_given_cpi_r18_211(void)
{
  uint8_t codeMemory[] = {
		0x23, 0x3d,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
	r[18] = 0x9c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
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
 *
 * Simulate ldi R31, 211
 *			1110 1101 1111 0011
 *			 e 	  d    f  	3
 */
void test_AvrOperatorTable_given_cpi_r31_211(void)
{
  uint8_t codeMemory[] = {
		0xf3, 0xed,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(211, r[31]);
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
 *
 * Simulate sbrc R7, 1
 *		1111 1100 0111 0001
 *     f 	  c    7  	1
 */
void test_AvrOperatorTable_given_sbrc_r7_1(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0x71, 0xfc,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  r[7] = 0x12;

	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
 *
 * Simulate sbrc R7, 2
 *		1111 1100 0111 0010
 *     f 	  c    7  	2
 */
void test_AvrOperatorTable_given_sbrc_r7_2(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x72, 0xfc,      //sbrc R7, 2
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[7] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(4, relAddr);
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
 *
 * Simulate sbrc R7, 3
 *		1111 1100 0111 0011
 *     f 	  c    7  	3
 */
void test_AvrOperatorTable_given_sbrc_r7_3(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x73, 0xfc,                   //sbrc R7, 3
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[7] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, relAddr);
}

/**
 * Instruction:
 * 		SBRS Rr, b
 *		1111 111r rrrr 0bbb
 * where
 *    0 <= rrrrr <= 31
 *    0 <= bbb <= 7
 *
 * PC <- PC + 1, condition false - no skip
 * PC <- PC + 2, Skip a one word instruction
 * PC <- PC + 3, Skip a two word instruction
 *
 * Simulate sbrs R7, 1
 *		1111 1110 0111 0001
 *     f 	  e    7  	1
 */
void test_AvrOperatorTable_given_sbrs_r7_1(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0x71, 0xfe,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[7] = 0x4c;

	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
 *
 * Simulate sbrs R7, 2
 *		1111 1110 0111 0010
 *     f 	  e    7  	2
 */
void test_AvrOperatorTable_given_sbrs_r7_2(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x72, 0xfe,      //sbrs R7, 2
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[7] = 0x4c;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(4, relAddr);
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
 *
 * Simulate sbrs R7, 3
 *		1111 1110 0111 0011
 *     f 	  e    7  	3
 */
void test_AvrOperatorTable_given_sbrs_r7_3(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x73, 0xfe,                   //sbrs R7, 3
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[7] = 0x4c;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, relAddr);
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
 *
 * Simulate sbic 12, 1
 *		1001 1001 0110 0001
 *     9 	  9    6  	1
 */
void test_AvrOperatorTable_given_sbic_12_1(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0x61, 0x99,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  io[12] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
 *
 * Simulate sbic 12, 2
 *		1001 1001 0110 0010
 *     9 	  9    6  	2
 */
void test_AvrOperatorTable_given_sbic_12_2(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x62, 0x99,      //sbic 12, 2
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[12] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(4, relAddr);
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
 *
 * Simulate sbic 12, 3
 *		1001 1001 0110 0011
 *     9 	  9    6  	3
 */
void test_AvrOperatorTable_given_sbic_12_3(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x63, 0x99,                   //sbic 12, 3
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[12] = 0x12;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, relAddr);
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
 *
 * Simulate sbis 30, 1
 *		1001 1011 1111 0001
 *     9 	  b    f  	1
 */
void test_AvrOperatorTable_given_sbis_30_1(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0xf1, 0x9b,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  io[30] = 0x4c;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(2, relAddr);
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
 *
 * Simulate sbis 30, 2
 *		1001 1011 1111 0010
 *     9 	  b    f  	2
 */
void test_AvrOperatorTable_given_sbis_30_2(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0xf2, 0x9b,      //sbis 30, 2
    0x0f, 0x01,      //add r16, r17
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  io[30] = 0x4c;
  
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL(4, relAddr);
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
 *
 * Simulate sbis 30, 3
 *		1001 1011 1111 0011
 *     9 	  b    f  	3
 */
void test_AvrOperatorTable_given_sbis_30_3(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0xf3, 0x9b,                   //sbis 30, 3
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  io[30] = 0x4c;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(6, relAddr);
}

/**
 * Instruction:
 * 		IJMP None
 *		1001 0100 0000 1001
 *
 * PC <- Z(15:0)
 *
 * Simulate ijmp
 *    1001 0100 0000 1001
 *     9    4    0    9
 */
void test_AvrOperatorTable_given_ijmp(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0xe0, 0xe4,                   //ldi r30, 0x40
    0xf5, 0xe7,                   //ldi r31, 0x75
    0x09, 0x94,                   //ijmp
	};
	uint8_t *progCounter = &codeMemory[4];
  flash = codeMemory;
  r[30] = 0x40;
  r[31] = 0x75;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX32((0x7540 - 4), relAddr);
}

/**
 * Instruction:
 * 		EIJMP None
 *		1001 0100 0001 1001
 *
 * PC(15:0) <- Z(15:0)
 * PC(21:16) <- EIND
 *
 * Simulate eijmp
 *    1001 0100 0001 1001
 *     9    4    1    9
 */
void test_AvrOperatorTable_given_eijmp(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
    0x05, 0xe0,               //ldi r16, 0x05
		0xe0, 0xe0,               //ldi r30, 0x00
    0xf0, 0xe1,               //ldi r31, 0x10
    0x19, 0x94,               //eijmp
	};
	uint8_t *progCounter = &codeMemory[6];
  flash = codeMemory;
  *eind = 0x05;
  r[30] = 0x00;
  r[31] = 0x10;
  
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX32((0x51000 - 6), relAddr);
}

/**
 * Instruction:
 * 		CALL k
 *		1001 010k kkkk 111k
 *    kkkk kkkk kkkk kkkk
 *
 * where
 *    0 <= k <= 64K
 *
 * Simulate call 4000
 *    1001 0100 0000 1110
 *     9    4    0    e
 *    0000 1111 1010 0000
 *     0    f    a    0
 */
void test_AvrOperatorTable_given_call(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    0x05, 0xe0,              		 //ldi r16, 0x05
    0x0e, 0x94, 0xa0, 0x0f,      // call 4000
	};
	uint8_t *progCounter = &codeMemory[2];
  flash = codeMemory;

	relAddr = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL(6, *(uint16_t *)(spl));
	TEST_ASSERT_EQUAL_HEX16(4000*2 - 2, relAddr);
}

/**
 * Instruction:
 * 		RCALL k
 *		1101 kkkk kkkk kkkk
 *
 * where
 *    -2K <= k <= 2K
 *
 * Simulate rcall 846
 *    1101 0011 0100 1110
 *     d    3    4    e
 */
void test_AvrOperatorTable_given_rcall_846(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    0x4e, 0xd3,              		 //rcall 846
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	relAddr = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL(2, *(uint16_t *)spl);
	TEST_ASSERT_EQUAL_HEX16((846 + 1) * 2, relAddr);
}

/**
 * Instruction:
 * 		ICALL None
 *		1001 0101 0000 1001
 *
 * Simulate icall None
 *    1001 0101 0000 1001
 *     9    5    0    9
 */
void test_AvrOperatorTable_given_icall(void)
{
  uint32_t pc;
  uint8_t codeMemory[] = {
    0x09, 0x95,              		 //icall None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0x4837;
	pc = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL(2, *(uint16_t *)spl);
  TEST_ASSERT_EQUAL_HEX16(0x4837, pc);
}

/**
 * Instruction:
 * 		EICALL None
 *		1001 0101 0001 1001
 *
 * Simulate eicall None
 *    1001 0101 0001 1001
 *     9    5    1    9
 */
void test_AvrOperatorTable_given_eicall(void)
{
  uint32_t pc;
  uint8_t codeMemory[] = {
    0x19, 0x95,              		 //eicall None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0x4837;
  *eind = 0x32;
	pc = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX32(3, *(uint16_t *)spl);
  TEST_ASSERT_EQUAL_HEX32(0x324837, pc);
}

/**
 * Instruction:
 * 		RET None
 *		1001 0101 0000 1000
 *
 * Simulate ret None
 *    1001 0101 0000 1000
 *     9    5    0    8
 */
void test_AvrOperatorTable_given_ret(void)
{
  int pc;
  uint8_t codeMemory[] = {
    0x08, 0x95,              		 //ret None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0x48;
  *sph = 0x32;
  sram[0x3248] = 0x24;
	pc = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX16(-2, *(uint16_t *)spl);
  TEST_ASSERT_EQUAL_HEX16(0x25, pc);
}

/**
 * Instruction:
 * 		RETI None
 *		1001 0101 0001 1000
 *
 * Simulate reti None
 *    1001 0101 0001 1000
 *     9    5    1    8
 */
void test_AvrOperatorTable_given_reti(void)
{
  int pc;
  uint8_t codeMemory[] = {
    0x18, 0x95,              		 //reti None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0x21;
  *sph = 0x98;
  sram[0x9821] = 0x6b;
	pc = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX16(-2, *(uint16_t *)spl);
  TEST_ASSERT_EQUAL_HEX16(0x6b, pc);
}

/**
 * Instruction:
 * 		CBI A, b
 *		1001 1000 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 *
 * Simulate cbi 18, 5
 *		1001 1000 1001 0101
 *     9 	   8      9     5
 */
void test_AvrOperatorTable_given_cbi_18_5(void)
{
  uint8_t codeMemory[] = {
		0x95, 0x98,								//cbi 18, 5
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  io[18] = 0x6b;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x4b, io[18]);
}

/**
 * Instruction:
 * 		SBI A, b
 *		1001 1010 AAAA Abbb
 * where
 *    0 <= AAAAA <= 31
 *    0 <= bbb <= 7
 *
 * Simulate sbi 18, 7
 *		1001 1010 1001 0111
 *     9 	   a      9     7
 */
void test_AvrOperatorTable_given_sbi_18_7(void)
{
  uint8_t codeMemory[] = {
		0x97, 0x9a,								//sbi 18, 7
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  io[18] = 0x6b;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xeb, io[18]);
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
 * 
 * Simulate movw r16, r0
 *		0000 0001 1000 0000
 *		  0    	 1    	8    	 0
 */
void test_AvrOperatorTable_given_movw_r16_r0(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x80, 0x01,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
	
	r[0] = 0x10;
	r[1] = 0x6b;

	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x10, r[16]);
	TEST_ASSERT_EQUAL_HEX16(0x6b, r[17]);
}

/**
 * Instruction:
 * 		IN Rd, A
 *		1011 0AAd dddd AAAA
 * where
 *		0 <= ddddd <= 31
 *    0 <= AAAAAA <= 63
 *
 * Simulate in R18, 52
 *		1011 0111 0010 0100
 *      b 	   7      2     4
 */
void test_AvrOperatorTable_given_in_18_52(void)
{
  uint8_t codeMemory[] = {
		0x24, 0xb7,								//in R18, 52
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  io[52] = 0x6b;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0x6b, r[18]);
}

/**
 * Instruction:
 * 		OUT A, Rr
 *		1011 1AAr rrrr AAAA
 * where
 *		0 <= rrrrr <= 31
 *    0 <= AAAAAA <= 63
 *
 * Simulate out 48, R31
 *		1011 1111 1111 0000
 *      b 	   f      f     0
 */
void test_AvrOperatorTable_given_out_48_31(void)
{
  uint8_t codeMemory[] = {
		0xf0, 0xbf,								//out 31, R48
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[31] = 0xbb;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL(0xbb, io[48]);
}

/**
 * Instruction:
 * 		LD Rd, X
 *		1001 000d dddd 1100
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R20, X
 *		1001 0001 0100 1100
 *      9 	   1      4     c
 */
void test_AvrOperatorTable_given_ld_20_X(void)
{
  uint8_t codeMemory[] = {
		0x4c, 0x91,								//ld R20, X
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[20] = 0;
  *xRegPtr = 0x100;
  sram[0x100] = 0x93;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x93, r[20]);
}

/**
 * Instruction:
 * 		LD Rd, X+
 *		1001 000d dddd 1101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R20, X+
 *		1001 0001 0100 1101
 *      9 	   1      4     d
 */
void test_AvrOperatorTable_given_ld_20_Xplus(void)
{
  uint8_t codeMemory[] = {
		0x4d, 0x91,								//ld R20, X+
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[20] = 0;
  *xRegPtr = 0x120;
  sram[0x120] = 0x65;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x65, r[20]);
  TEST_ASSERT_EQUAL_HEX16(0x121, *xRegPtr);
}

/**
 * Instruction:
 * 		LD Rd, -X
 *		1001 000d dddd 1110
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R20, -X
 *		1001 0001 0100 1110
 *      9 	   1      4     e
 */
void test_AvrOperatorTable_given_ld_20_minusX(void)
{
  uint8_t codeMemory[] = {
		0x4e, 0x91,								//ld R20, -X
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[20] = 0;
  *xRegPtr = 0x112;
  sram[0x111] = 0x32;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x32, r[20]);
}

/**
 * Instruction:
 * 		LD Rd, Y
 *		1000 000d dddd 1000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R31, Y
 *		1000 0001 1111 1000
 *      8 	   1      f     8
 */
void test_AvrOperatorTable_given_ld_31_Y(void)
{
  uint8_t codeMemory[] = {
		0xf8, 0x81,								//ld R31, Y
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[31] = 0;
  *yRegPtr = 0x191;
  sram[0x191] = 0x88;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x88, r[31]);
}

/**
 * Instruction:
 * 		LD Rd, Y+
 *		1001 000d dddd 1001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R31, Y+
 *		1001 0001 1111 1001
 *      9 	   1      f     9
 */
void test_AvrOperatorTable_given_ld_31_Yplus(void)
{
  uint8_t codeMemory[] = {
		0xf9, 0x91,								//ld R31, Y+
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[31] = 0;
  *yRegPtr = 0x143;
  sram[0x143] = 0x98;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x98, r[31]);
  TEST_ASSERT_EQUAL_HEX16(0x144, *yRegPtr);
}

/**
 * Instruction:
 * 		LD Rd, -Y
 *		1001 000d dddd 1010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R31, -Y
 *		1001 0001 1111 1010
 *      9 	   1      f     a
 */
void test_AvrOperatorTable_given_ld_31_minusY(void)
{
  uint8_t codeMemory[] = {
		0xfa, 0x91,								//ld R31, -Y
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[31] = 0;
  *yRegPtr = 0x154;
  sram[0x153] = 0x76;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x76, r[31]);
}

/**
 * Instruction:
 * 		LD Rd, Z
 *		1000 000d dddd 0000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R13, Z
 *		1000 0000 1101 0000
 *      8 	   0      d     0
 */
void test_AvrOperatorTable_given_ld_13_Z(void)
{
  uint8_t codeMemory[] = {
		0xd0, 0x80,								//ld R13, Z
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[13] = 0;
  *zRegPtr = 0x111;
  sram[0x111] = 0x98;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x98, r[13]);
}

/**
 * Instruction:
 * 		LD Rd, Z+
 *		1001 000d dddd 0001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R13, Z+
 *		1001 0000 1101 0001
 *      9 	   0      d     1
 */
void test_AvrOperatorTable_given_ld_13_Zplus(void)
{
  uint8_t codeMemory[] = {
		0xd1, 0x90,								//ld R31, Z+
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[13] = 0;
  *zRegPtr = 0x143;
  sram[0x143] = 0x55;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x55, r[13]);
  TEST_ASSERT_EQUAL_HEX16(0x144, *zRegPtr);
}

/**
 * Instruction:
 * 		LD Rd, -Z
 *		1001 000d dddd 0010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate ld R13, -Z
 *		1001 0000 1101 0010
 *      9 	   0      d     2
 */
void test_AvrOperatorTable_given_ld_13_minusZ(void)
{
  uint8_t codeMemory[] = {
		0xd2, 0x90,								//ld R13, -Z
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[13] = 0;
  *zRegPtr = 0x154;
  sram[0x153] = 0x66;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x66, r[13]);
}

/**
 * Instruction:
 * 		ST X, Rr
 *		1001 001r rrrr 1100
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st X, R21
 *		1001 0011 0101 1100
 *      9 	   3      5     c
 */
void test_AvrOperatorTable_given_st_X_21(void)
{
  uint8_t codeMemory[] = {
		0x5c, 0x93,								//st X, R21
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[21] = 0x11;
  *xRegPtr = 0x8aa;
  sram[0x8aa] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x11, sram[0x8aa]);
}

/**
 * Instruction:
 * 		ST X+, Rr
 *		1001 001r rrrr 1101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st X+, R21
 *		1001 0011 0101 1101
 *      9 	   3      5     d
 */
void test_AvrOperatorTable_given_st_Xplus_21(void)
{
  uint8_t codeMemory[] = {
		0x5d, 0x93,								//ld X+, R21
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[21] = 0x98;
	*xRegPtr = 0x543;
  sram[0x543] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x543]);
  TEST_ASSERT_EQUAL_HEX16(0x544, *xRegPtr);
}

/**
 * Instruction:
 * 		ST -X, Rr
 *		1001 001r rrrr 1110
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st -X, R21
 *		1001 0011 0101 1110
 *      9 	   3      5     e
 */
void test_AvrOperatorTable_given_st_minusX_21(void)
{
  uint8_t codeMemory[] = {
		0x5e, 0x93,								//ld -X, R21
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[21] = 0x67;
  *xRegPtr = 0x743;
  sram[0x743] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x67, sram[0x742]);
}

/**
 * Instruction:
 * 		ST Y, Rr
 *		1000 001r rrrr 1000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st Y, R11
 *		1000 0010 1011 1000
 *      8 	   2      b     8
 */
void test_AvrOperatorTable_given_st_Y_11(void)
{
  uint8_t codeMemory[] = {
		0xb8, 0x82,								//st Y, R11
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[11] = 0x8;
  *yRegPtr = 0x655;
  sram[0x655] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x8, sram[0x655]);
}

/**
 * Instruction:
 * 		ST Y+, Rr
 *		1001 001r rrrr 1001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st Y+, R11
 *		1001 0010 1011 1001
 *      9 	   2      b     9
 */
void test_AvrOperatorTable_given_st_Yplus_11(void)
{
  uint8_t codeMemory[] = {
		0xb9, 0x92,								//ld Y+, R11
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[11] = 0x98;
	*yRegPtr = 0x159;
  sram[0x159] = 0;
	
	simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x159]);
	TEST_ASSERT_EQUAL_HEX16(0x15a, *yRegPtr);
}

/**
 * Instruction:
 * 		ST -Y, Rr
 *		1001 001r rrrr 1010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st -Y, R11
 *		1001 0010 1011 1010
 *      9 	   2      b     a
 */
void test_AvrOperatorTable_given_st_minusY_11(void)
{
  uint8_t codeMemory[] = {
		0xba, 0x92,								//ld -X, R11
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[11] = 0x67;
	*yRegPtr = 0x321;
  sram[0x321] = 0;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x67, sram[0x320]);
}

/**
 * Instruction:
 * 		ST Z, Rr
 *		1000 001r rrrr 0000
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st Z, R8
 *		1000 0010 1000 0000
 *      8 	   2      8     0
 */
void test_AvrOperatorTable_given_st_Z_8(void)
{
  uint8_t codeMemory[] = {
		0x80, 0x82,								//st Z, R8
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[8] = 0x27;
  *zRegPtr = 0x345;
  sram[0x345] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x27, sram[0x345]);
}

/**
 * Instruction:
 * 		ST Z+, Rr
 *		1001 001r rrrr 0001
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st Z+, R8
 *		1001 0010 1000 0001
 *      9 	   2      8     1
 */
void test_AvrOperatorTable_given_st_Zplus_8(void)
{
  uint8_t codeMemory[] = {
		0x81, 0x92,								//ld Z+, R8
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[8] = 0x98;
	*zRegPtr = 0x863;
  sram[0x863] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x863]);
  TEST_ASSERT_EQUAL_HEX16(0x864, *zRegPtr);
}

/**
 * Instruction:
 * 		ST -Z, Rr
 *		1001 001r rrrr 0010
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate st -Z, R8
 *		1001 0010 1000 0010
 *      9 	   2      8     2
 */
void test_AvrOperatorTable_given_st_minusZ_8(void)
{
  uint8_t codeMemory[] = {
		0x82, 0x92,								//ld -Z, R8
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[8] = 0x98;
  *zRegPtr = 0x59a;
  sram[0x59a] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x599]);
}

/**
 * Instruction:
 * 		LPM None
 *		1001 0101 1100 1000
 *
 * Simulate lpm None
 *		1001 0101 1100 1000
 *      9 	   5      c     8
 */
void test_AvrOperatorTable_given_lpm(void)
{
  uint8_t codeMemory[] = {
		0xc8, 0x95,								//lpm None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0x4000;
  flashMemory[0x4000] = 0x43;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x43, r[0]);
}

/**
 * Instruction:
 * 		LPM Rd, Z
 *		1001 000d dddd 0100
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lpm R9, Z
 *		1001 0000 1001 0100
 *      9 	   0      9     4
 */
void test_AvrOperatorTable_given_lpm_9_Z(void)
{
  uint8_t codeMemory[] = {
		0x94, 0x90,								//lpm R9, Z
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0x8000;
  flashMemory[0x8000] = 0x3b;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x3b, r[9]);
}

/**
 * Instruction:
 * 		LPM Rd, Z+
 *		1001 000d dddd 0101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lpm R15, Z+
 *		1001 0000 1111 0101
 *      9 	   0      f     5
 */
void test_AvrOperatorTable_given_lpm_15_Zplus(void)
{
  uint8_t codeMemory[] = {
		0xf5, 0x90,								//lpm R15, Z+
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0xef32;
  flashMemory[0xef32] = 0x4a;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x4a, r[15]);
  TEST_ASSERT_EQUAL_HEX32(0xef33, *zRegPtr);
}

/**
 * Instruction:
 * 		LAT Z, Rd
 *		1001 001r rrrr 0111
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lat Z, R29
 *		1001 0011 1101 0111
 *      9 	   3      d     7
 */
void test_AvrOperatorTable_given_lat_Z_29(void)
{
  uint8_t codeMemory[] = {
		0xd7, 0x93,								//lat Z, R29
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[29] = 0x77;
	*zRegPtr = 0x163;
  sram[0x163] = 0x3c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x4b, r[29]);
}

/**
 * Instruction:
 * 		LAS Z, Rd
 *		1001 001r rrrr 0101
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate las Z, R23
 *		1001 0011 0111 0101
 *      9 	   3      7     5
 */
void test_AvrOperatorTable_given_las_Z_23(void)
{
  uint8_t codeMemory[] = {
		0x75, 0x93,								//las Z, R23
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[23] = 0x22;
	*zRegPtr = 0x584;
  sram[0x584] = 0x3c;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x3e, r[23]);
}

/**
 * Instruction:
 * 		LAC Z, Rd
 *		1001 001r rrrr 0110
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate lac Z, R13
 *		1001 0010 1101 0110
 *      9 	   2      d     6
 */
void test_AvrOperatorTable_given_lac_Z_13(void)
{
  uint8_t codeMemory[] = {
		0xd6, 0x92,								//lac Z, R13
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[13] = 0x65;
	*zRegPtr = 0x345;
  sram[0x345] = 0x11;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x10, sram[0x345]);
}

/**
 * Instruction:
 * 		XCH Z, Rd
 *		1001 001d dddd 0100
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate xch Z, R12
 *		1001 0010 1100 0100
 *      9 	   2      c     4
 */
void test_AvrOperatorTable_given_xch_Z_12(void)
{
  uint8_t codeMemory[] = {
		0xc4, 0x92,								//xch Z, R12
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[12] = 0x22;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x22, r[12]);
}

/**
 * Instruction:
 * 		PUSH Rr
 *		1001 001r rrrr 1111
 * where
 *		0 <= rrrrr <= 31
 *
 * Simulate push R12
 *		1001 0010 1100 1111
 *      9 	   2      c      f
 */
void test_AvrOperatorTable_given_push_12(void)
{
  uint8_t codeMemory[] = {
		0xcf, 0x92,								//push R12
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[12] = 0x7d;
  *spl = 0x21;
  *sph = 0x12;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x7d, sram[0x1221]);
}

/**
 * Instruction:
 * 		POP Rd
 *		1001 000d dddd 1111
 * where
 *		0 <= ddddd <= 31
 *
 * Simulate pop R27
 *		1001 0001 1011 1111
 *      9 	   1      b      f
 */
void test_AvrOperatorTable_given_pop_27(void)
{
  uint8_t codeMemory[] = {
		0xbf, 0x91,								//pop R27
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0x3d;
  *sph = 0x11;
  sram[0x113d] = 0x58;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x58, r[27]);
}