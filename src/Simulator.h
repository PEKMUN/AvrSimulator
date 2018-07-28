#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#define SRAM_SIZE 0x900

typedef int (*AvrOperator)(uint8_t *codePtr);
typedef struct SregRegister SregRegister;
struct SregRegister{
	uint8_t C: 1;
	uint8_t Z: 1;
	uint8_t N: 1;
	uint8_t V: 1;
	uint8_t S: 1;
	uint8_t H: 1;
	uint8_t T: 1;
	uint8_t I: 1;
};

int add(uint8_t *codePtr);
int adc(uint8_t *codePtr);
int and(uint8_t *codePtr);
int andi(uint8_t *codePtr);
int adiw(uint8_t *codePtr);
int sub(uint8_t *codePtr);
int subi(uint8_t *codePtr);
int sbc(uint8_t *codePtr);
int sbci(uint8_t *codePtr);
int sbiw(uint8_t *codePtr);
int or(uint8_t *codePtr);
int ori(uint8_t *codePtr);
int eor(uint8_t *codePtr);
int itIsCom(uint8_t *codePtr);
//int negOrCom(uint8_t *codePtr);
//int com(uint8_t *codePtr);
//int neg(uint8_t *codePtr);

//Export Variable(s)
extern AvrOperator AvrOperatorTable[256];
extern uint8_t sram[SRAM_SIZE];

//AVR Register
extern uint8_t *r;
extern SregRegister *sreg;
extern uint8_t *sph;
extern uint8_t *spl;

#endif // _SIMULATOR_H
