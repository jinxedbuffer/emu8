#include<iostream>
#include<SDL2/SDL.h>
#include "cpu.h"
#include "gfx.h"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Scancode controlkeymap[KEYPAD_SIZE] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A, 
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};

bool gfx::init() {
    window = NULL;
    renderer = NULL;
    qf = false;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) { 
        std::cout << "Error: Cannot initialize SDL!\n";
        return false;
    }
    window = SDL_CreateWindow("emu8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, EMU_SCREEN_WIDTH, EMU_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        std::cout << "Error: Window cannot be created!\n";
        return false;
    } else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    return true;
}

void gfx::eventhandler(uint8_t key[]) {
    SDL_Event e;
    if(SDL_PollEvent(&e)) {
        const uint8_t* state = SDL_GetKeyboardState(NULL);
        if(e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) {
            qf = true;
        } else {
            for(int i = 0; i < KEYPAD_SIZE; i++) {
                key[i] = state[controlkeymap[i]];
            }
        }
    }
}

void gfx::draw(uint8_t* disp) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int y = 0; y < DISPLAY_HEIGHT; y++) {
        for(int x = 0; x < DISPLAY_WIDTH; x++) {
            if(disp[x + DISPLAY_WIDTH*y] != 0) {
                SDL_Rect r;
                r.x = x * SCALE_FACTOR;
                r.y = y * SCALE_FACTOR;
                r.w = SCALE_FACTOR;
                r.h = SCALE_FACTOR;
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void gfx::destroy() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}