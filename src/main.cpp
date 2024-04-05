#include<iostream>
#include<unistd.h>
#include "cpu.h"
#include "gfx.h"

int main(int argc, char* argv[]) {
    if(argc < 2 || argc > 2) {
        std::cout << "Please enter in correct format!\nUsage: ./emu8 [game filename]\n";
        return -1;
    }
    CPU cpu;
    cpu.init();
    if(cpu.load_rom(argv[1])) {
        gfx screen;
        if(screen.init()) {
            while(!screen.qf) {
                cpu.execute();
                screen.eventhandler(cpu.key);
                if(cpu.df) {
                    screen.draw(cpu.disp);
                }
                usleep(16666);
            }
        screen.destroy();
        }
    }
    return 0;
}