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
/*void test_AvrOperatorTable_given_adiw_r27_r26_3(void)
{
	uint8_t *codePtr;
    uint8_t codeMemory[] = {
		0x1d, 0x96,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	adiw(&codePtr);
	TEST_ASSERT_EQUAL(codeMemory, codePtr);
}*/

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
	
	Simulate(codeMemory);
	
	TEST_ASSERT_EQUAL(28, r[4]);
	TEST_ASSERT_EQUAL(23, r[7]);
}