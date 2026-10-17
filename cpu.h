#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>

 //unsigned char can be used instead of uint8_t
 //unsigned short can be used instead of uint16_t

extern uint16_t opCode;
extern uint16_t pc; //program counter
extern uint16_t I;
extern uint16_t stack[16]; //16 levels to the stack
extern uint16_t sp; //stack pointer to point to the stack position
extern uint8_t memory[4096];  // 4k memory
extern uint8_t V[16]; // has 16 registers (v0-vf)
extern uint8_t dT; //delay timer
extern uint8_t sT; //sound timer
extern uint8_t graphics[64 * 32]; //64 by 32 pixel display.
extern uint8_t key[16]; //hex based keypad (0x0-0xF) 16 keys

void initCpu(); //intialize the cpu
void emulateCycle(); //emulates 1 cycle of the cpu running
void loadGame(const char *filename); //loads the file into memory