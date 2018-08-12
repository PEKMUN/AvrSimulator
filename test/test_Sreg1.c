#include <stdio.h>
#include "unity.h"
#include "Simulator.h"
#include "Error.h"
#include "Exception.h"
#include "CException.h"

void setUp(void){}

void tearDown(void){}

/**
 * Z:
 *   !R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
void test_is8bitZero_given_data8bit_is_0b00010100(void)
{
	uint8_t data8bit, z;
	data8bit = 0x14;
	
	z = is8bitZero(data8bit);
	
	TEST_ASSERT_EQUAL(0, z);
}

void test_is8bitZero_given_data8bit_is_0b00000000(void)
{
	uint8_t data8bit, z;
	data8bit = 0x0;
	
	z = is8bitZero(data8bit);
	
	TEST_ASSERT_EQUAL(1, z);
}

/**
 * C:
 *    !R15.Rdh7
 */
void test_is16bitADIWCarry_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0x46f6;
	
	c = is16bitADIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0xff4d_operand_is_0xc6f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0xc6f6;
	
	c = is16bitADIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0x46f6;
	
	c = is16bitADIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitADIWCarry_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0xf6f6;
	
	c = is16bitADIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(1, c);
}

/**
 * Z:
 *   !R15.!R14.!R13.!R12.!R11.!R10.!R9.!R8.!R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
void test_is16bitZero_given_data8bit_is_0x6f94(void)
{
	uint16_t data16Bit, z;
	data16Bit = 0x6f94;
	
	z = is16BitZero(data16Bit);
	
	TEST_ASSERT_EQUAL(0, z);
}

void test_is16bitZero_given_data8bit_is_0x0000(void)
{
	uint16_t data16Bit, z;
	data16Bit = 0x0000;
	
	z = is16BitZero(data16Bit);
	
	TEST_ASSERT_EQUAL(1, z);
}

/**
 * N:
 *    R15
 */
void test_is16bitNeg_given_result_is_0x4f4d(void)
{
	uint8_t n;
	uint16_t result;
	
	result = 0x4f4d;
	
	n = is16bitNeg(result);
	
	TEST_ASSERT_EQUAL(0, n);
}

void test_is16bitNeg_given_result_is_0xff4d(void)
{
	uint8_t n;
	uint16_t result;
	
	result = 0xff4d;
	
	n = is16bitNeg(result);
	
	TEST_ASSERT_EQUAL(1, n);
}

/**
 * V:
 *    R15.!Rdh7
 */
void test_is16bitADIWOverflow_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0xf6f6;
	
	v = is16bitADIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0x46f6;
	
	v = is16bitADIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0xf6f6;
	
	v = is16bitADIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitADIWOverflow_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0x46f6;
	
	v = is16bitADIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(1, v);
}

/**
 * S:
 *    N^V
 */
void test_is16bitSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0xf6f6;
	
	s = is16bitSigned(result, operand);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is16bitSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0x46f6;
	
	s = is16bitSigned(result, operand);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_handleStatusRegForAddImmWordOperation_given_result_is_0x54d6_operand_is_0xdd82(void)
{
	uint16_t result, operand;
	
	result = 0x54d6;
	operand = 0xdd82;
	
	handleStatusRegForAddImmWordOperation(result, operand);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *    R15.!Rdh7
 */
void test_is16bitSBIWCarry_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0xf6f6;
	
	c = is16bitSBIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0x46f6;
	
	c = is16bitSBIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0xf6f6;
	
	c = is16bitSBIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitSBIWCarry_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t c;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0x46f6;
	
	c = is16bitSBIWCarry(result, operand);
	
	TEST_ASSERT_EQUAL(1, c);
} 

/**
 * V:
 *    !R15.Rdh7
 */
void test_is16bitSBIWOverflow_given_result_is_0xff4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0x46f6;
	
	v = is16bitSBIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0xff4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0xff4d;
	operand = 0xf6f6;
	
	v = is16bitSBIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0x4f4d_operand_is_0x46f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0x46f6;
	
	v = is16bitSBIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is16bitSBIWOverflow_given_result_is_0x4f4d_operand_is_0xf6f6(void)
{
	uint8_t v;
	uint16_t result, operand;
	
	result = 0x4f4d;
	operand = 0xf6f6;
	
	v = is16bitSBIWOverflow(result, operand);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_handleStatusRegForSubImmWordOperation_given_result_is_0x83f5_operand_is_0x98a2(void)
{
	uint16_t result, operand;
	
	result = 0x83f5;
	operand = 0x98a2;
	
	handleStatusRegForAddImmWordOperation(result, operand);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * C: 
 *    Rd7.Rr7+Rr7.!R7+!R7.Rd7
 */
void test_is8bitAdcAddCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x09(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitAdcAddCarry_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;
  
	c = is8bitAdcAddCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

/**
 * N: 
 *    R7
 */
void test_is8bitNeg_given_result_is_0x17(void)
{
	uint8_t n;
	uint8_t result;
	
	result = 0x17;
  
	n = is8bitNeg(result);
	
	TEST_ASSERT_EQUAL(0, n);
}

void test_is8bitNeg_given_result_is_0xa7(void)
{
	uint8_t n;
	uint8_t result;
	
	result = 0xa7;
  
	n = is8bitNeg(result);
	
	TEST_ASSERT_EQUAL(1, n);
}

/**
 * V: 
 *    Rd7.Rr7.!R7+!Rd7.!Rr7.R7
 */
void test_is8bitOverflow_given_operand1_is_0x06_operand2_is_0x71_result_is_0x77(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x06;
	operand2 = 0x71;
	result = 0x77;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitOverflow_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitOverflow_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitOverflow_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;
  
	v = is8bitOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *    N^V
 */
void test_is8bitSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;
	
	s = is8bitSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitSigned_given_n_is_0_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;
	
	s = is8bitSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;
	
	s = is8bitSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;
	
	s = is8bitSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

/**
 * H: 
 *    Rd3.Rr3+Rr3.!R3+!R3.Rd3
 */
void test_is8bitAdcAddHalfCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x01(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}