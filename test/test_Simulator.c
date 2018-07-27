#include <stdio.h>
#include "unity.h"
#include "Simulator.h"

void setUp(void){}

void tearDown(void){}

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
  int k;
	uint8_t *codePtr;
  uint8_t codeMemory[] = {
		0x1d, 0x96,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[26] = 24;
	r[27] = 10;
		
	adiw(codeMemory);
	
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
 * Simulate adiw R31,30, 63
 * 		KKKKKK = 2 = 0x2 = b'000010'
 *		    dd = b'11' ==> 30
 *		1001 0110 0011 0010
 *		  9    6    3    2
 */
void test_AvrOperatorTable_given_adiw_r31_r30_2(void)
{
  int k;
	uint8_t *codePtr;
  uint8_t codeMemory[] = {
		0x32, 0x96,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[30] = 0xff;
	r[31] = 0x10;
		
	adiw(codeMemory);
	
  TEST_ASSERT_EQUAL(0x01, r[30]);
	TEST_ASSERT_EQUAL(0x11, r[31]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[7] = 23;
	r[4] = 5;
	
	add(codeMemory);
	
	TEST_ASSERT_EQUAL(23, r[7]);
	TEST_ASSERT_EQUAL(28, r[4]);
}