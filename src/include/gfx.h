#ifndef CHIP8_GFX_H
#define CHIP8_GFX_H

#include<cstdint>

#define SCALE_FACTOR 8
#define EMU_SCREEN_WIDTH DISPLAY_WIDTH * SCALE_FACTOR
#define EMU_SCREEN_HEIGHT DISPLAY_HEIGHT * SCALE_FACTOR

typedef struct {
    bool qf; // quit flag
    bool init(); // initialize SDL
    void eventhandler(uint8_t key[]); // checks for keypresses
    void draw(uint8_t* disp); // draws on screen
    void destroy(); // quits the emulator
} gfx;

#endif