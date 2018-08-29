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
 * Simulate brbc 5, 61
 *			1111 0101 1110 1101
 *			 f 	  5    e  	d
 */
void test_AvrOperatorTable_given_brbc_5_61(void)
{
	int relAddr;
	uint8_t codeMemory[] = {
		0xed, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->H = 0;
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(124, relAddr);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbc 4, 61
 *			1111 0101 1110 1100
 *			 f 	  5    e  	c
 */
void test_AvrOperatorTable_given_brbc_4_61(void)
{
	int relAddr;
	uint8_t codeMemory[] = {
		0xec, 0xf5,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->S = 1;
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(2, relAddr);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbc 1, -0x4
 *			1111 0111 1110 0001
 *			 f 	  7      e    	1
 */
void test_AvrOperatorTable_given_brbc_1_minus0x4(void)
{
	int relAddr;
	uint8_t codeMemory[] = {
		0xe1, 0xf7,
	};
	uint8_t *progCounter = codeMemory;
	flash = codeMemory;
  
  sreg->Z = 0;
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(-6, relAddr);
}

/**
 * Instruction:
 * 		BRBC s, k
 *			1111 01kk kkkk ksss
 * where
 *			-64 <= kkkkkkk <= +63
 *      0 <= sss <= 7
 *
 * Simulate brbc 1, -0x7
 *			1111 0111 1100 1001
 *			 f 	  7    c    9
 */
void test_AvrOperatorTable_given_brbc_1_minus0x7(void)
{
	int relAddr;
	uint8_t codeMemory[] = {
		[22] = 0xc9, 0xf7,               // BRNE PC-0x07
	};
	uint8_t *progCounter = &codeMemory[22];
	flash = codeMemory;
  
  sreg->Z = 0;
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_INT32(-12, relAddr);
}

/**
 * Instruction:
 * 		WDR None
 *			1001 0101 1010 1000
 *       9    5    a    8
 */
void test_AvrOperatorTable_given_wdr(void)
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
void test_AvrOperatorTable_given_break(void)
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
void test_AvrOperatorTable_given_sleep(void)
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
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
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
 * Simulate cpi R28, 0x60
 *			0011 0110 1100 0000
 *			 3 	  6    c  	0
 */
void test_AvrOperatorTable_given_cpi_r28_0x60(void)
{
  uint8_t codeMemory[] = {
		0xc0, 0x36,
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
	r[28] = 0x9b;
	
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
 *		1111 111r rrrr 0bbb
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
 *		1111 111r rrrr 0bbb
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
 * Simulate sbrs R18, 5
 *		1111 1111 0010 0101
 *     f 	  f    2  	5
 */
void test_AvrOperatorTable_given_sbrs_r18_5(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    //start:
		0x25, 0xff,                   //sbrs R18, 5
    0x0e, 0x94, 0x78, 0x05,       //call 1400
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;
	
  r[18] = 0x24;
  
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

	*spl = 0xff;
  *sph = 0x8;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fd, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x0, sram[0x8fe]);
	TEST_ASSERT_EQUAL_HEX16(0x03, sram[0x8ff]);
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
 * Simulate rcall PC + 3
 *    1101 0000 0000 0010
 *     d    0    0    2
 */
void test_AvrOperatorTable_given_rcall_PC_plus_3(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    0x47, 0xe0,              		 //ldi r20, 0x7
    0x02, 0xd0,              		 //rcall PC + 3
    0x11, 0xe0,                  //ldi r17, 0x1
    0x27, 0xe0,                  //ldi r18, 0x7
    0x31, 0xe0,                  //ldi r19, 0x1
	};
	uint8_t *progCounter = &codeMemory[2];
  flash = codeMemory;

	*spl = 0xff;
  *sph = 0x8;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fd, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x00, sram[0x8fe]);
	TEST_ASSERT_EQUAL_HEX16(0x02, sram[0x8ff]);
	TEST_ASSERT_EQUAL_HEX16((2 + 1) * 2, relAddr);
}

/**
 * Instruction:
 * 		RCALL k
 *		1101 kkkk kkkk kkkk
 *
 * where
 *    -2K <= k <= 2K
 *
 * Simulate rcall PC - 4
 *    1101 1111 1111 1011
 *			 d			f			f			 b
 */
void test_AvrOperatorTable_given_rcall_PC_minus_4(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0xfb, 0xeb,              		 //ldi r31, 0xbb
		0x02, 0xd0,              		 //rcall PC + 4
		0xbb, 0x27,              		 //clr r27
		0xab, 0xeb,              		 //ldi r26, 0xbb
		0xb4, 0xe0,              		 //ldi r27, 0x4
		0x1d, 0x90,              		 //ld r1, X+
    0xfb, 0xdf,              		 //rcall PC - 4
		0x1c, 0x90,              		 //rcall r1, X
	};
	uint8_t *progCounter = &codeMemory[12];
  flash = codeMemory;

	*spl = 0xff;
  *sph = 0x8;
	//sram[0x8fe] = 0x00;
	//sram[0x8ff] = 0x21;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fd, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x00, sram[0x8fe]);
	TEST_ASSERT_EQUAL_HEX16(0x7, sram[0x8ff]);
	TEST_ASSERT_EQUAL_HEX16((-5 + 1) * 2, relAddr);
}

/**
 * Instruction:
 * 		RCALL k
 *		1101 kkkk kkkk kkkk
 *
 * where
 *    -2K <= k <= 2K
 *
 * Simulate rcall PC - 0x20
 *    1101 1111 1101 1111
 *			 d			f			d			 f
 */
void test_AvrOperatorTable_given_rcall_PC_minus_0x20(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
		0x0e, 0xc0,              		 //RJMP PC+0x000F
		0xe1, 0x2f,              		 //MOV R30,R17
		0xf2, 0x2f,              		 //MOV R31,R18
		0xf0, 0x2e,              		 //MOV R15,R16
		0xd0, 0x80,              		 //LDD R13,Z+0
		0xe2, 0x90,              		 //LD R14,-Z
    0xde, 0x14,              		 //CP R13,R14
		0x10, 0xf0,              		 //BRCS PC+0x03
		0xd0, 0x82,              		 //STD Z+0,R13
		0xe1, 0x82,              		 //STD Z+1,R14
		0xfa, 0x94,              		 //DEC R15
		0xc1, 0xf7,              		 //BRNE PC-0x07
		0x0a, 0x95,              		 //DEC R16
		0x99, 0xf7,              		 //BRNE PC-0x0C
    0x08, 0x95,              		 //RET
		0x0f, 0xef,              		 //SER R16
		0x0d, 0xbf,              		 //OUT 0x3D,R16
		0x08, 0xe0,              		 //LDI R16,0x08
		0x0e, 0xbf,              		 //OUT 0x3E,R16
		0xff, 0x27,              		 //CLR R31
		0xe3, 0xe8,              		 //LDI R30,0x83
		0xc4, 0xe6,              		 //LDI R28,0x64
    0xd2, 0xe0,              		 //LDI R29,0x02
		0xc8, 0x95,              		 //LPM
		0x0a, 0x92,              		 //ST -Y,R0
		0x31, 0x97,              		 //SBIW R30,0x01
		0xc0, 0x36,              		 //CPI R28,0x60
		0xd9, 0xf7,              		 //BRNE PC-0x04
		0xd2, 0x30,              		 //CPI R29,0x02
		0xc9, 0xf7,              		 //BRNE PC-0x06
    0x13, 0xe6,              		 //LDI R17,0x63
		0x22, 0xe0,              		 //LDI R18,0x02
		0x03, 0xe0,              		 //LDI R16,0x03
		0xdf, 0xdf,              		 //RCALL PC-0x0020
    0x98, 0x95,              		 //BREAK
		0xff, 0xcf,              		 //RJMP PC-0x0000
	};
	uint8_t *progCounter = &codeMemory[66];
  flash = codeMemory;

	*spl = 0xff;
  *sph = 0x8;
	sram[0x8fe] = 0x00;
	sram[0x8ff] = 0x22;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fd, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x00, sram[0x8fe]);
	TEST_ASSERT_EQUAL_HEX16(0x22, sram[0x8ff]);
	TEST_ASSERT_EQUAL_HEX16((-33 + 1) * 2, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    0x09, 0x95,              		 //icall None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0xfd;
  *sph = 0x8;
	*zRegPtr = 0x260;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fb, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x00, sram[0x8fc]);
	TEST_ASSERT_EQUAL_HEX16(0x01, sram[0x8fd]);
  TEST_ASSERT_EQUAL_HEX16(0x260, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    0x19, 0x95,              		 //eicall None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *zRegPtr = 0x4837;
  *eind = 0x32;
	*spl = 0xfd;
  *sph = 0x8;
	relAddr = simulateOneInstruction(progCounter);

	TEST_ASSERT_EQUAL_HEX16(0x8fb, getMcuStackPtr());
	TEST_ASSERT_EQUAL_HEX16(0x00, sram[0x8fc]);
	TEST_ASSERT_EQUAL_HEX16(0x01, sram[0x8fd]);
  TEST_ASSERT_EQUAL_HEX32(0x324837, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    0x08, 0x95,              		 //ret None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0xfd;
  *sph = 0x8;
	sram[0x8fe] = 0x1;
	sram[0x8ff] = 0x22;
	relAddr = simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x1, sram[0x8fe]);
	TEST_ASSERT_EQUAL_HEX16(0x22, sram[0x8ff]);
  TEST_ASSERT_EQUAL_HEX16(0x8ff, getMcuStackPtr());
  TEST_ASSERT_EQUAL_HEX16(0x244, relAddr);
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
void test_AvrOperatorTable_given_ret_stack_pointer_is_0x08fb(void)
{
  int relAddr;
  uint8_t codeMemory[] = {
    0x08, 0x95,              		 //ret None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0xfb;
  *sph = 0x8;
	sram[0x8fc] = 0x0;
	sram[0x8fd] = 0x26;
	relAddr = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX16(0x8fd, getMcuStackPtr());
  TEST_ASSERT_EQUAL_HEX16(0x4c, relAddr);
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
  int relAddr;
  uint8_t codeMemory[] = {
    0x18, 0x95,              		 //reti None
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  *spl = 0xfd;
  *sph = 0x8;
	sram[0x8fe] = 0x0;
  sram[0x8ff] = 0x6b;
	sreg->I = 0;
	relAddr = simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX16(0x8ff, getMcuStackPtr());
  TEST_ASSERT_EQUAL_HEX16(0xd6, relAddr);
	TEST_ASSERT_EQUAL_HEX16(1, sreg->I);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x93, sram[0x100]);
	TEST_ASSERT_EQUAL_HEX16(0x00, r[26]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[27]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x65, sram[0x120]);
	TEST_ASSERT_EQUAL_HEX8(0x65, r[20]);
  TEST_ASSERT_EQUAL_HEX16(0x21, r[26]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[27]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x32, sram[0x112-1]);
	TEST_ASSERT_EQUAL_HEX16(0x11, r[26]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[27]);
	TEST_ASSERT_EQUAL_HEX16(0x32, r[20]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x88, sram[0x191]);
	TEST_ASSERT_EQUAL_HEX16(0x91, r[28]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[29]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x98, sram[0x143]);
	TEST_ASSERT_EQUAL_HEX8(0x98, r[31]);
  TEST_ASSERT_EQUAL_HEX16(0x44, r[28]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[29]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x76, sram[0x154-1]);
	TEST_ASSERT_EQUAL_HEX16(0x53, r[28]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[29]);
	TEST_ASSERT_EQUAL_HEX8(0x76, r[31]);
}

/**
 * Instruction:
 * 		LDD Rd, Y+q
 *		10q0 qq0d dddd 1qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 *
 * Simulate ldd R17, Y+18
 *		1000 1001 0001 1010
 *      8 	   9      1     a
 */
void test_AvrOperatorTable_given_ldd_17_Y_plus_18(void)
{
  uint8_t codeMemory[] = {
		0x1a, 0x89,								//ldd R17, Y+18
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[17] = 0;
  *yRegPtr = 0x191;
  sram[0x1a3] = 0x88;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x88, sram[0x191 + 18]);
	TEST_ASSERT_EQUAL_HEX16(0x91, r[28]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[29]);
	TEST_ASSERT_EQUAL_HEX16(0x88, r[17]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x98, sram[0x111]);
	TEST_ASSERT_EQUAL_HEX16(0x11, r[30]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[31]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x55, sram[0x143]);
	TEST_ASSERT_EQUAL_HEX8(0x55, r[13]);
  TEST_ASSERT_EQUAL_HEX16(0x44, r[30]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[31]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x66, sram[0x154-1]);
	TEST_ASSERT_EQUAL_HEX16(0x53, r[30]);
	TEST_ASSERT_EQUAL_HEX16(0x1, r[31]);
	TEST_ASSERT_EQUAL_HEX8(0x66, r[13]);
}

/**
 * Instruction:
 * 		LDD Rd, Z+q
 *		10q0 qq0d dddd 0qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 *
 * Simulate ldd R17, Z+18
 *		1000 1001 0001 0010
 *      8 	   9      1     2
 */
void test_AvrOperatorTable_given_ldd_17_Z_plus_18(void)
{
  uint8_t codeMemory[] = {
		0x12, 0x89,								//ldd R17, Z+18
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[17] = 0;
  *zRegPtr = 0x254;
  sram[0x254 + 18] = 0x72;
	
	simulateOneInstruction(progCounter);
	
  TEST_ASSERT_EQUAL_HEX16(0x72, sram[0x254 + 18]);
  TEST_ASSERT_EQUAL_HEX16(0x54, r[30]);
  TEST_ASSERT_EQUAL_HEX16(0x2, r[31]);
	TEST_ASSERT_EQUAL_HEX16(0x72, r[17]);
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
	
  TEST_ASSERT_EQUAL_HEX16(0xaa, r[26]);
  TEST_ASSERT_EQUAL_HEX16(0x8, r[27]);
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
  TEST_ASSERT_EQUAL_HEX16(0x44, r[26]);
  TEST_ASSERT_EQUAL_HEX16(0x5, r[27]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x42, r[26]);
  TEST_ASSERT_EQUAL_HEX16(0x7, r[27]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x55, r[28]);
  TEST_ASSERT_EQUAL_HEX16(0x6, r[29]);
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
		0xb9, 0x92,								//st Y+, R11
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[11] = 0x98;
	*yRegPtr = 0x159;
  sram[0x159] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x5a, r[28]);
  TEST_ASSERT_EQUAL_HEX16(0x1, r[29]);
  TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x159]);
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
		0xba, 0x92,								//st -Y, R11
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

	r[11] = 0x67;
	*yRegPtr = 0x321;
  sram[0x321] = 0;
  
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x20, r[28]);
  TEST_ASSERT_EQUAL_HEX16(0x3, r[29]);
	TEST_ASSERT_EQUAL_HEX8(0x67, sram[0x320]);
}

/**
 * Instruction:
 * 		STD Y+q, Rr
 *		10q0 qq1r rrrr 1qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 *
 * Simulate std Y+48, R12
 *		1010 1010 1100 1000
 *      a 	   a      c     8
 */
void test_AvrOperatorTable_given_std_Y_plus_48_12(void)
{
  uint8_t codeMemory[] = {
		0xc8, 0xaa,								//std Y+48, R12
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[12] = 0xaa;
  *yRegPtr = 0x655;
  sram[0x685] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x55, r[28]);
  TEST_ASSERT_EQUAL_HEX16(0x6, r[29]);
	TEST_ASSERT_EQUAL_HEX16(0xaa, sram[0x685]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x45, r[30]);
  TEST_ASSERT_EQUAL_HEX16(0x3, r[31]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x64, r[30]);
  TEST_ASSERT_EQUAL_HEX16(0x8, r[31]);
	TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x863]);
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
	
	TEST_ASSERT_EQUAL_HEX16(0x99, r[30]);
  TEST_ASSERT_EQUAL_HEX16(0x5, r[31]);
	TEST_ASSERT_EQUAL_HEX8(0x98, sram[0x599]);
}

/**
 * Instruction:
 * 		STD Z+q, Rr
 *		10q0 qq1r rrrr 0qqq
 * where
 *		0 <= ddddd <= 31
 *		0 <= qqqqqq <= 63
 *
 * Simulate std Z+63, R24
 *		1010 1111 1000 0111
 *      a 	   f      8     7
 */
void test_AvrOperatorTable_given_std_Z_plus_63_24(void)
{
  uint8_t codeMemory[] = {
		0x87, 0xaf,								//std Z+63, R24
	};
	uint8_t *progCounter = codeMemory;
  flash = codeMemory;

  r[24] = 0x6f;
  *zRegPtr = 0x518;
  sram[0x557] = 0;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX16(0x18, r[30]);
  TEST_ASSERT_EQUAL_HEX16(0x5, r[31]);
	TEST_ASSERT_EQUAL_HEX16(0x6f, sram[0x557]);
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
  *sph = 0x2;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x7d, sram[0x221]);
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
  *sph = 0x1;
  sram[0x13d] = 0x58;
	
	simulateOneInstruction(progCounter);
	
	TEST_ASSERT_EQUAL_HEX8(0x58, r[27]);
}