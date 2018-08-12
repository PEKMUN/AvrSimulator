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
 *		!R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
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
 *		!R15.Rdh7
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
 *		!R15.!R14.!R13.!R12.!R11.!R10.!R9.!R8.!R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
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
 *		R15 
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
 *		R15.!Rdh7
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
 *		N ^ V
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
 *		R15.!Rdh7
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
 *		!R15.Rdh7
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
 *    Rd7.Rr7 + Rr7.!R7 + !R7.Rd7
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
 *    Rd7.Rr7.!R7 + !Rd7.!Rr7.R7
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
 *		N ^ V
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
 *    Rd3.Rr3 + Rr3.!R3 + !R3.Rd3
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

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x77_operand2_is_0x71_result_is_0xf9(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x77;
	operand2 = 0x71;
	result = 0xf9;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x70_operand2_is_0x9e_result_is_0x13(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x70;
	operand2 = 0x9e;
	result = 0x13;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x00_operand2_is_0xff_result_is_0xff(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xff;
	result = 0xff;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0xfc_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xfc;
	operand2 = 0x11;
	result = 0x07;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x8a_operand2_is_0x11_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8a;
	operand2 = 0x11;
	result = 0x98;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x22(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x22;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitAdcAddHalfCarry_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;
  
	h = is8bitAdcAddHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForAddAdcOperation_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t result, operand1, operand2;
	
	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;
	
	handleStatusRegForAddAdcOperation(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(1, sreg->H);
}

/**
 * V:
 *		0
 */
void test_is8bitAndAndiOrOriEorCbrSbrTstOverflow_given_v_is_0(void)
{
	uint8_t v;
	sreg->V = 0;
	
	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitAndAndiOrOriEorCbrSbrTstOverflow_given_v_is_1(void)
{
	uint8_t v;
	sreg->V = 1;
	
	v = is8bitAndAndiOrOriEorCbrSbrTstOverflow();
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitAndAndiOrOriEorCbrSbrTstSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result;
	
	result = 0x07;
	
	s = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitAndAndiOrOriEorCbrSbrTstSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result;
	
	result = 0x97;
	
	s = is8bitAndAndiOrOriEorCbrSbrTstSigned(result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation_given_result_is_0xab(void)
{
	uint8_t result;

	result = 0xab;
	
	handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(result);
	
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * C: 
 *    !Rd7.Rr7 + Rr7.R7 + R7.!Rd7
 */
void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x09(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitSubSubiSbcSbciCarry_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t c;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;
  
	c = is8bitSubSubiSbcSbciCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, c);
}

/**
 * H: 
 *		!Rd3.Rr3 + Rr3.R3 + R3.!Rd3
 */
void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x00_operand2_is_0x01_result_is_0x01(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0x01;
	result = 0x01;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x77_operand2_is_0x71_result_is_0xf9(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x77;
	operand2 = 0x71;
	result = 0xf9;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x70_operand2_is_0x9e_result_is_0x13(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x70;
	operand2 = 0x9e;
	result = 0x13;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x00_operand2_is_0xff_result_is_0xff(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xff;
	result = 0xff;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0xfc_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xfc;
	operand2 = 0x11;
	result = 0x07;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x8a_operand2_is_0x11_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8a;
	operand2 = 0x11;
	result = 0x98;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x22(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x22;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitSubSubiSbcSbciHalfCarry_given_operand1_is_0xff_operand2_is_0xab_result_is_0x98(void)
{
	uint8_t h;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xff;
	operand2 = 0xab;
	result = 0x98;
  
	h = is8bitSubSubiSbcSbciHalfCarry(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

/**
 * V: 
 *    Rd7.!Rr7.!R7 + !Rd7.Rr7.R7
 */
void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x06_operand2_is_0x71_result_is_0x77(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x06;
	operand2 = 0x71;
	result = 0x77;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x7f_operand2_is_0x7a_result_is_0xf9(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x7a;
	result = 0xf9;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x7c_operand2_is_0x9e_result_is_0x1a(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x7c;
	operand2 = 0x9e;
	result = 0x1a;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x00_operand2_is_0xf1_result_is_0xf1(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0xf1;
	result = 0xf1;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0xf6_operand2_is_0x11_result_is_0x07(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x11;
	result = 0x07;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x86_operand2_is_0x11_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x86;
	operand2 = 0x11;
	result = 0x97;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0x8f_operand2_is_0x9c_result_is_0x2b(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0x8f;
	operand2 = 0x9c;
	result = 0x2b;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitSubSubiSbcSbciOverflow_given_operand1_is_0xf6_operand2_is_0xa1_result_is_0x97(void)
{
	uint8_t v;
	uint8_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0xa1;
	result = 0x97;
  
	v = is8bitSubSubiSbcSbciOverflow(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitSubSubiSbcSbciSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0x16;
	operand2 = 0x11;
	result = 0x07;
	
	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_0_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0x9f;
	operand2 = 0x4c;
	result = 0x5b;
	
	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_1_v_is_0(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0xf6;
	operand2 = 0x01;
	result = 0x97;
	
	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitSubSubiSbcSbciSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result, operand1, operand2;
	
	operand1 = 0x7f;
	operand2 = 0x8a;
	result = 0xf9;
	
	s = is8bitSubSubiSbcSbciSigned(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_handleStatusRegForSubSubiSbcSbciOperation_given_operand1_is_0xaf_operand2_is_0x6b_result_is_0x11(void)
{
	uint8_t result, operand1, operand2;
	
	operand1 = 0xaf;
	operand2 = 0x6b;
	result = 0x11;
	
	handleStatusRegForSubSubiSbcSbciOperation(operand1, operand2, result);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * C:
 *		1
 */
void test_is8bitComCarry_given_c_is_0(void)
{
	uint8_t c;
	//sreg->C = 0;
	
	c = is8bitComCarry();
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitComCarry_given_c_is_1(void)
{
	uint8_t c;
	sreg->C = 1;
	
	c = is8bitComCarry();
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_handleStatusRegForComOperation_given_operand1_is_0x00_operand2_is_0x00_result_is_0x00(void)
{
	uint8_t result, operand1, operand2;
	
	operand1 = 0x00;
	operand2 = 0x00;
	result = 0x00;
	
	handleStatusRegForComOperation(result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(1, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *		R7 + R6 + R5 + R4 + R3 + R2 + R1 + R0
 */
void test_is8bitNegCarry_given_data8bit_is_0b00010100(void)
{
  uint8_t data8bit, c;
	data8bit = 0x14;
	
	c = is8bitNegCarry(data8bit);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_is8bitNegCarry_given_data8bit_is_0b00000000(void)
{
	uint8_t data8bit, c;
	data8bit = 0x0;
	
	c = is8bitNegCarry(data8bit);
	
	TEST_ASSERT_EQUAL(0, c);
}

/**
 * V:
 *		R7.!R6.!R5.!R4.!R3.!R2.!R1.!R0
 */
void test_is8bitNegOverflow_given_data8bit_is_0b10000000(void)
{
  uint8_t data8bit, v;
	data8bit = 0x80;
	
	v = is8bitNegOverflow(data8bit);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitNegOverflow_given_data8bit_is_0b10011111(void)
{
	uint8_t data8bit, v;
	data8bit = 0x9f;
	
	v = is8bitNegOverflow(data8bit);
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitNegSigned_given_n_is_0_v_is_0(void)
{
	uint8_t s;
	uint16_t result;
	
	result = 0x07;
	
	s = is8bitNegSigned(result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitNegSigned_given_n_is_1_v_is_1(void)
{
	uint8_t s;
	uint16_t result;
	
	result = 0x80;
	
	s = is8bitNegSigned(result);
	
	TEST_ASSERT_EQUAL(0, s);
}

/**
 * H:
 *		R3 + Rd3
 */
void test_is8bitNegHalfCarry_given_operand1_is_0xa3_result_is_0x80(void)
{
	uint8_t h;
	uint16_t result, operand1;
	
	operand1 = 0xa3;
	result = 0x80;
	
	h = is8bitNegHalfCarry(operand1, result);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xa3_result_is_0x88(void)
{
	uint8_t h;
	uint16_t result, operand1;
	
	operand1 = 0xa3;
	result = 0x88;
	
	h = is8bitNegHalfCarry(operand1, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xaf_result_is_0x81(void)
{
	uint8_t h;
	uint16_t result, operand1;
	
	operand1 = 0xaf;
	result = 0x81;
	
	h = is8bitNegHalfCarry(operand1, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_is8bitNegHalfCarry_given_operand1_is_0xaf_result_is_0x8a(void)
{
	uint8_t h;
	uint16_t result, operand1;
	
	operand1 = 0xaf;
	result = 0x8a;
	
	h = is8bitNegHalfCarry(operand1, result);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForNegOperation_given_operand1_is_0x81_result_is_0x94(void)
{
	uint8_t result, operand1;
	
	operand1 = 0x81;
	result = 0x94;
	
	handleStatusRegForNegOperation(operand1, result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

void test_handleStatusRegForIncOperation_given_operand1_is_0x81_result_is_0x94(void)
{
	uint8_t result;
	
	result = 0x94;
	
	handleStatusRegForIncOperation(result);
	
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(1, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * V:
 *		!R7.R6.R5.R4.R3.R2.R1.R0
 */
void test_is8bitDecOverflow_given_data8bit_is_0b01111111(void)
{
	uint8_t data8bit, v;
	data8bit = 0x7f;
	
	v = is8bitDecOverflow(data8bit);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitDecOverflow_given_data8bit_is_0b10011100(void)
{
	uint8_t data8bit, v;
	data8bit = 0x9c;
	
	v = is8bitDecOverflow(data8bit);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_handleStatusRegForClrOperation(void)
{
	handleStatusRegForClrOperation();
	
	TEST_ASSERT_EQUAL(1, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(0, sreg->V);
	TEST_ASSERT_EQUAL(0, sreg->S);
}

/**
 * C:
 *		R15
 */
void test_is16bitMulMulsMulsuCarry_given_result_is_0x6f73(void)
{
	uint16_t result, c;
	result = 0x6f73;
	
	c = is16bitMulMulsMulsuCarry(result);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is16bitMulMulsMulsuCarry_given_result_is_0xff73(void)
{
	uint16_t result, c;
	result = 0xff73;
	
	c = is16bitMulMulsMulsuCarry(result);
	
	TEST_ASSERT_EQUAL(1, c);
}

void test_handleStatusRegForMulMulsMulsuOperation_given_reult_is_0x1234(void)
{
	uint16_t result;
	result = 0x1234;
	
	handleStatusRegForMulMulsMulsuOperation(result);
	
	TEST_ASSERT_EQUAL(0, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
}

/**
 * C:
 *		Rd7
 */
void test_is8bitLslRolCarry_given_operand1_is_0x6f(void)
{
	uint8_t operand1, c;
	operand1 = 0x6f;
	
	c = is8bitLslRolCarry(operand1);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitLslRolCarry_given_operand1_is_0xf6(void)
{
	uint8_t operand1, c;
	operand1 = 0xf6;
	
	c = is8bitLslRolCarry(operand1);
	
	TEST_ASSERT_EQUAL(1, c);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitLslRolOverflow_given_operand1_is_0x6f_result_is_0x47(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x6f;
	result = 0x47;
	
	v = is8bitLslRolOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0x6f_result_is_0x89(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x6f;
	result = 0x89;
	
	v = is8bitLslRolOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0x92_result_is_0x47(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x92;
	result = 0x47;
	
	v = is8bitLslRolOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitLslRolOverflow_given_operand1_is_0xff_result_is_0x96(void)
{
	uint8_t operand1, result, v;
	operand1 = 0xff;
	result = 0x96;
	
	v = is8bitLslRolOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitLslRolSigned_given_operand1_is_0x6f_result_is_0x47(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x6f;
	result = 0x47;
	
	s = is8bitLslRolSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0x6f_result_is_0x89(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x6f;
	result = 0x89;
	
	s = is8bitLslRolSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0x92_result_is_0x47(void)
{
	uint8_t operand1, result, s;
	operand1 = 0x92;
	result = 0x47;
	
	s = is8bitLslRolSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_is8bitLslRolSigned_given_operand1_is_0xff_result_is_0x96(void)
{
	uint8_t operand1, result, s;
	operand1 = 0xff;
	result = 0x96;
	
	s = is8bitLslRolSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(1, s);
}

/**
 * H:
 *		Rd3
 */
void test_is8bitLslRolHalfCarry_given_operand1_is_0x34(void)
{
	uint8_t operand1, h;
	operand1 = 0x34;
	
	h = is8bitLslRolHalfCarry(operand1);
	
	TEST_ASSERT_EQUAL(0, h);
}

void test_is8bitLslRolHalfCarry_given_operand1_is_0xff(void)
{
	uint8_t operand1, h;
	operand1 = 0xff;
	
	h = is8bitLslRolHalfCarry(operand1);
	
	TEST_ASSERT_EQUAL(1, h);
}

void test_handleStatusRegForLslRolOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;
	
	handleStatusRegForLslRolOperation(operand1, result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
	TEST_ASSERT_EQUAL(0, sreg->H);
}

/**
 * C:
 *		Rd0
 */
void test_is8bitLsrRorCarry_given_operand1_is_0x68(void)
{
	uint8_t operand1, c;
	operand1 = 0x68;
	
	c = is8bitLsrRorCarry(operand1);
	
	TEST_ASSERT_EQUAL(0, c);
}

void test_is8bitLsrRorCarry_given_operand1_is_0xf9(void)
{
	uint8_t operand1, c;
	operand1 = 0xf9;
	
	c = is8bitLsrRorCarry(operand1);
	
	TEST_ASSERT_EQUAL(1, c);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitLsrOverflow_given_operand1_is_0x68(void)
{
	uint8_t operand1, v;
	operand1 = 0x68;
	
	v = is8bitLsrOverflow(operand1);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitLsrOverflow_given_operand1_is_0x6f_operand1_is_0x89(void)
{
	uint8_t operand1, v;
	operand1 = 0x89;
	
	v = is8bitLsrOverflow(operand1);
	
	TEST_ASSERT_EQUAL(1, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitLsrSigned_given_operand1_is_0x68(void)
{
	uint8_t operand1, s;
	operand1 = 0x68;
	
	s = is8bitLsrSigned(operand1);
	
	TEST_ASSERT_EQUAL(0, s);
}

void test_is8bitLsrSigned_given_operand1_is_0x6f_operand1_is_0x89(void)
{
	uint8_t operand1, s;
	operand1 = 0x89;
	
	s = is8bitLsrSigned(operand1);
	
	TEST_ASSERT_EQUAL(1, s);
}

void test_handleStatusRegForLsrOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;
	
	handleStatusRegForLsrOperation(operand1, result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

/**
 * V:
 *		N ^ C
 */
void test_is8bitRorOverflow_given_operand1_is_0x67_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x04;
	
	v = is8bitRorOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x67_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x94;
	
	v = is8bitRorOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x89_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x89;
	result = 0x04;
	
	v = is8bitRorOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorOverflow_given_operand1_is_0x89_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x89;
	result = 0x94;
	
	v = is8bitRorOverflow(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

/**
 * S:
 *		N ^ V
 */
void test_is8bitRorSigned_given_operand1_is_0x68_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x68;
	result = 0x04;
	
	v = is8bitRorSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorSigned_given_operand1_is_0x67_result_is_0x04(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x67;
	result = 0x04;
	
	v = is8bitRorSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_is8bitRorSigned_given_operand1_is_0x88_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x88;
	result = 0x94;
	
	v = is8bitRorSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(0, v);
}

void test_is8bitRorSigned_given_operand1_is_0x87_result_is_0x94(void)
{
	uint8_t operand1, result, v;
	operand1 = 0x87;
	result = 0x94;
	
	v = is8bitRorSigned(operand1, result);
	
	TEST_ASSERT_EQUAL(1, v);
}

void test_handleStatusRegForRorAsrOperation_given_operand1_is_0x87_result_is_0x43(void)
{
	uint8_t operand1, result;
	operand1 = 0x87;
	result = 0x43;
	
	handleStatusRegForLsrOperation(operand1, result);
	
	TEST_ASSERT_EQUAL(1, sreg->C);
	TEST_ASSERT_EQUAL(0, sreg->Z);
	TEST_ASSERT_EQUAL(0, sreg->N);
	TEST_ASSERT_EQUAL(1, sreg->V);
	TEST_ASSERT_EQUAL(1, sreg->S);
}

void test_handleStatusRegForRorBstOperation_given_operand1_is_0b010(void)
{
	uint8_t operand1, t;
	operand1 = 0b010;
	
	t = handleStatusRegForRorBstOperation(operand1);
	
	TEST_ASSERT_EQUAL(1, t);
}