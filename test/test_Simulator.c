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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[2] = 0b0101;
	r[3] = 0b1111;
	
	and(codeMemory);
	
	TEST_ASSERT_EQUAL(0b0101, r[2]);
	TEST_ASSERT_EQUAL(0b1111, r[3]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[4]  = 0b1110101;
	r[31] = 0b1111000;
	
	and(codeMemory);
	
	TEST_ASSERT_EQUAL(0b1110000, r[4]);
	TEST_ASSERT_EQUAL(0b1111000, r[31]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[20] = 0b11110000;
	
	andi(codeMemory);
	
	TEST_ASSERT_EQUAL(0b00100000, r[20]);
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
 *		0000 1100 0101 1001
 *		  0    c    5    9
 */
void test_AvrOperatorTable_given_adc_r5_r9(void)
{
    uint8_t codeMemory[] = {
		0x59, 0x0c,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	sreg->C = 1;
	
	r[5] = 0x7d;
	r[9] = 0x1c;
	
	adc(codeMemory);
	
	TEST_ASSERT_EQUAL(0x9a, r[5]);
	TEST_ASSERT_EQUAL(0x1c, r[9]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[15] = 23;
	r[21] = 5;
	
	sub(codeMemory);
	
	TEST_ASSERT_EQUAL(18, r[15]);
	TEST_ASSERT_EQUAL(5, r[21]);
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
 * 
 * Simulate subi R25, 201
 * 		KKKKKKKK = 201 = b'11001001'
 *		    dddd = b'1001' ==> 25
 *		0111 1100 1001 1001
 *		  7     c     9     9
 */
void test_AvrOperatorTable_given_subi_r25_201(void)
{
	uint8_t *codePtr;
	uint8_t codeMemory[] = {
		0x99, 0x7c,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[25] = 0b11110000;
	
	subi(codeMemory);
	
	TEST_ASSERT_EQUAL(0b00100111, r[25]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	sreg->C = 1;
	
	r[9]  = 0x9c;
	r[16] = 0x52;
	
	sbc(codeMemory);
	
	TEST_ASSERT_EQUAL(0x49, r[9]);
	TEST_ASSERT_EQUAL(0x52, r[16]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	sreg->C = 1;
	
	r[29] = 0b11111111;
	
	sbci(codeMemory);
	
	TEST_ASSERT_EQUAL(0b01100011, r[29]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[28] = 0x28;
	r[29] = 0x10;

	sbiw(codeMemory);
	
	TEST_ASSERT_EQUAL(0xf2, r[28]);
	TEST_ASSERT_EQUAL(0xf, r[29]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[2] = 0b01110;
	r[8] = 0b11000;
	
	or(codeMemory);
	
	TEST_ASSERT_EQUAL(0b11110, r[2]);
	TEST_ASSERT_EQUAL(0b11000, r[8]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);

	r[23] = 0b01100011;
	
	ori(codeMemory);
	
	TEST_ASSERT_EQUAL(0b01101111, r[23]);
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
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	r[16] = 0b01000111;
	r[10] = 0b11100011;
	
	eor(codeMemory);
	
	TEST_ASSERT_EQUAL(0b10100100, r[16]);
	TEST_ASSERT_EQUAL(0b11100011, r[10]);
}

/**
 * Instruction:
 * 		COM Rd
 *		1001 010d dddd 0000
 * where
 *		0 <= ddddd <= 31
 */
void test_itIsCom_given_bitZero_for_16BitOpcode_is_1_expect_1(void)
{
	uint8_t retVal;
	uint8_t codeMemory[] = {
		0xf0, 0x95,
	};
	uint8_t *progCounter = codeMemory;
	AvrOperatorTable [*(progCounter + 1)](progCounter);
	
	retVal = itIsCom(codeMemory);
	
	TEST_ASSERT_EQUAL(1, retVal);
}