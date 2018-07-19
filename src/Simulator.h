#ifndef _SIMULATOR_H
#define _SIMULATOR_H

typedef int (*AvrOperator)(uint8_t *codePtr);

int add(uint8_t *codePtr);
int adc(uint8_t *codePtr);
int and(uint8_t *codePtr);
int andi(uint8_t *codePtr);
int adiw(uint8_t *codePtr);

//Export Variable(s)
extern AvrOperator AvrOperatorTable[256];

#endif // _SIMULATOR_H
