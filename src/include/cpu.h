#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include<cstdint>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define KEYPAD_SIZE 16
#define ROM_START 0x200
#define CLOCK_SPEED 540
#define DEBUG_MODE 0

class CPU {
    public:
        uint16_t  opcode; // 16-bit opcode (0x000 - 0xFFFF)
        uint8_t memory[MEMORY_SIZE]; // 4KB memory (0x000 - 0xFFF)
        uint8_t V[REGISTER_COUNT]; // 16 general purpose registers (GPRs) 
        uint16_t I; // index register
        uint16_t pc; // program counter
        uint16_t stack[STACK_SIZE]; // stack
        uint16_t sp; // stack pointer
        uint8_t dt; // delay timer
        uint8_t st; // sound timer
        uint8_t disp[DISPLAY_WIDTH*DISPLAY_HEIGHT]; // 64x32 pixel monochrome display
        uint8_t key[KEYPAD_SIZE]; // hex-based keypad (0 - F)
        bool df; // draw flag
        void init(); // initialize CPU
        bool load_rom(const char* filename); // loads ROM to CPU
        void execute(); // executes one opcode at a time
};

#endif