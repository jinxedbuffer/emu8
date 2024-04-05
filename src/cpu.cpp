#include<iostream>
#include<fstream>
#include<cstdlib>
#include "cpu.h"

const uint8_t fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

void CPU::init() {
    std::cout << "Initializing CPU...\n";
    opcode = 0;
    for(int i = 0x000; i < 0x50; i++) {
        memory[i] = fontset[i];
    }
    for(int i = 0x050; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    for(int i = 0; i < REGISTER_COUNT; i++) {
        V[i] = 0;
    }
    I = 0;
    pc = ROM_START;
    for(int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
    sp = -1;
    dt = 0;
    st = 0;
    for(int i = 0; i < DISPLAY_WIDTH*DISPLAY_HEIGHT; i++) {
        disp[i] = 0;
    }
    for(int i = 0; i < KEYPAD_SIZE; i++) {
        key[i] = 0;
    }
    df = false;
}

bool CPU::load_rom(const char* filename) {
    FILE* rom = fopen(filename, "rb");
    std::cout << "Loading " << filename << "... ";
    if(!rom) {
        std::cout << "Failed! Cannot open file.\n";
        return false;
    } else {
        std::cout << "Successful.\n";
    }
    fseek(rom, 0, SEEK_END);
    long filesize = ftell(rom);
    rewind(rom);
    std::cout << "Filesize: " << filesize << "B\n";
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*filesize);
    if(!buffer) {
        std::cout << "Failed! Cannot allocate memory.\n";
        return false;
    } else {
        std::cout << "Successfully allocated memory...\n";
    }
    size_t read_bytes = fread(buffer, 1, filesize, rom);
    if(read_bytes!=(size_t)filesize) {
        std::cout << "Failed! Reading error.\n";
        return false;
    } else {
        std::cout << "Successfully read file...\n";
    }
    fclose(rom);
    if(MEMORY_SIZE - ROM_START >= filesize) {
        for(int i = 0; i < filesize; i++) {
            memory[ROM_START + i] = buffer[i];
        }
        std::cout << "Loaded buffer into memory...\n";
    } else {
        std::cout << "File is too big for memory! Max size allowed: 3.5KB.\n";
    }
    free(buffer);
    return true;
}

void CPU::execute() {
    opcode = (memory[pc] << 8) | memory[pc+1];
    uint8_t n1 = (opcode & 0xF000) >> 12;
    uint8_t n2 = (opcode & 0x0F00) >> 8;
    uint8_t n3 = (opcode & 0x00F0) >> 4;
    uint8_t n4 = opcode & 0x000F;
    df = false;

    if(n1 == 0x0) {
        if(n3 == 0xE) {
            if(n4 == 0x0) { // 0x00E0
                for(int i = 0; i < DISPLAY_WIDTH*DISPLAY_HEIGHT; i++) {
                    disp[i] = 0x0;
                }
                df = true;
                pc += 2;
            } else if(n4 == 0xE) { // 0x00EE
                pc = stack[sp];
                sp--;
                pc += 2;
            }
        } else { // 0x0nnn (ignore)
            pc += 2;
        }
    } else if(n1 == 0x1) { // 0x1nnn
        pc = (opcode & 0x0FFF);
    } else if(n1 == 0x2) { // 0x2nnn
        sp++;
        stack[sp] = pc;
        pc = opcode & 0xFFF;
    } else if(n1 == 0x3) { // 0x3xkk
        if(V[n2] == (opcode & 0x00FF)) {
            pc += 4;
        } else {
            pc += 2;
        }
    } else if(n1 == 0x4) { // 0x4xkk
        if(V[n2] != (opcode & 0x00FF)) {
            pc += 4;
        } else {
            pc += 2;
        }
    } else if(n1 == 0x5) { // 0x5xy0
        if(V[n2] == V[n3]) {
            pc += 4;
        } else {
            pc += 2;
        }
    } else if(n1 == 0x6) { // 0x6xkk
        V[n2] = opcode & 0x00FF;
        pc += 2;
    } else if(n1 == 0x7) { // 0x7xkk
        V[n2] += opcode & 0x00FF;
        pc += 2;
    } else if(n1 == 0x8) {
        if(n4 == 0x0) { // 0x8xy0
            V[n2] = V[n3];
            pc += 2;
        } else if(n4 == 0x1) { // 0x8xy1
            V[n2] |= V[n3];
            pc += 2;
        } else if(n4 == 0x2) { // 0x8xy2
            V[n2] &= V[n3];
            pc += 2;
        } else if(n4 == 0x3) { // 0x8xy3
            V[n2] ^= V[n3];
            pc += 2;
        } else if(n4 == 0x4) { // 0x8xy4
            if(V[n2] + V[n3] > 255) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[n2] += V[n3];
            pc += 2;
        } else if(n4 == 0x5) { // 0x8xy5
            if(V[n2] > V[n3]) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[n2] -= V[n3];
            pc += 2;
        } else if(n4 == 0x6) { // 0x8xy6
            if((V[n2] & 0x1) == 1) {
                V[0xF] = 1;
            } else{
                V[0xF] = 0;
            }
            V[n2] >>= 1;
            pc += 2;
        } else if(n4 == 0x7) { // 0x8xy7
            if(V[n3] > V[n2]) {
                V[0xF] = 1;
            } else {
                V[0xF] = 0;
            }
            V[n2] = V[n3] - V[n2];
            pc += 2;
        } else if(n4 == 0xE) { // 0x8xyE
            if(V[n2] >> 7 == 1) {
                V[0xF] = 1;
            } else{
                V[0xF] = 0;
            }
            V[n2] <<= 1;
            pc += 2;
        }
    } else if(n1 == 0x9 && n4 == 0x0) { // 0x9xy0
        if(V[n2] != V[n3]) {
            pc += 4;
        } else {
            pc += 2;
        }
    } else if(n1 == 0xA) { // 0xAnnn
        I = opcode & 0x0FFF;
        pc += 2;
    } else if(n1 == 0xB) { // 0xBnnn
        pc = V[0x0] + (opcode & 0x0FFF);
    } else if(n1 == 0xC) { // 0xCxkk
        V[n2] = (rand() % 0xFF) & (opcode & 0x00FF);
        pc += 2;
    } else if(n1 == 0xD) { // 0xDxyn
        uint16_t x = V[n2];
        uint16_t y = V[n3];
        uint16_t height = n4;
        uint16_t sprite_line;
        V[0xF] = 0;
        for (int y_pos = 0; y_pos < height; y_pos++) {
            sprite_line = memory[I + y_pos];
            for(int x_pos = 0; x_pos < 8; x_pos++) {
                if((sprite_line & (0x80 >> x_pos)) != 0) {
                    if(disp[(x + x_pos + ((y + y_pos) * 64))] == 1) {
                        V[0xF] = 1;                                    
                    }
                    disp[x + x_pos + ((y + y_pos) * 64)] ^= 1;
                }
            }
        } 
        df = true;
        pc += 2;
    } else if(n1 == 0xE) {
        if(n3 == 0x9 && n4 == 0xE) { // 0xEx9E
            if(key[V[n2]] != 0) {
                pc += 4;
            } else {
                pc += 2;
            }
        } else if(n3 == 0xA && n4 == 0x1) { // 0xExA1
            if(key[V[n2]] == 0) {
                pc += 4;
            } else {
                pc += 2;
            }
        }
    } else if(n1 == 0xF) {
        if(n3 == 0x0) {
            if(n4 == 0x7) { // 0xFx07
                V[n2] = dt;
                pc += 2;
            } else if(n4 == 0xA) { // 0xFx0A
                bool keyPress = false;
                for(int i = 0; i < 16; i++) {
                    if(key[i] != 0) {
                        V[n2] = i;
                        keyPress = true;
                    }
                }
                if(!keyPress)
                    return;
                pc += 2;
            }
        } else if(n3 == 0x1) {
            if(n4 == 0x5) { // 0xFx15
                dt = V[n2];
                pc += 2;
            } else if(n4 == 0x8) { // 0xFx18
                st = V[n2];
                pc += 2;
            } else if(n4 == 0xE) { // 0xFx1E
                I += V[n2];
            }
        } else if(n3 == 0x2 && n4 == 0x9) { // 0xFx29
            I = V[n2] * 0x5;
            pc += 2;
        } else if(n3 == 0x3 && n4 == 0x3) { // 0xFx33
            memory[I] = V[n2] / 100;
            memory[I+1] = (V[n2] / 10) % 10;
            memory[I+2] = (V[n2] % 100) % 10;
            pc += 2;
        } else if(n3 == 0x5 && n4 == 0x5) { // 0xFx55
            for(int i = 0; i <= n2; i++) {
                memory[I + i] = V[i];
            }
            pc += 2;
        } else if(n3 == 0x6 && n4 == 0x5) { // 0xFx65
            for(int i = 0; i <= n2; i++) {
                V[i] = memory[I + i];
            }
            pc += 2;
        }
    } else {
        printf("Unknown opcode: 0x%X\n", opcode);
    }

    if(dt > 0) {
        dt--;
    }
    if(st > 0) {
        if(st == 1) {
            std::cout << "Beep!\n";
        }
        st--;
    }
}
