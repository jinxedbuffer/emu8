#include<SDL2/SDL.h>
#include "beeper.h"

void audio_callback(void* _beeper, uint8_t* _stream, int _length) {
    Beeper* beeper = (Beeper*) _beeper;
    int16_t* stream = (int16_t*) _stream;
    int length = _length / 2;
    beeper->generateSamples(stream, length);
}

Beeper::Beeper() {
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = SAMPLING_RATE;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    SDL_OpenAudio(&desiredSpec, &obtainedSpec);
    SDL_PauseAudio(0);
}

Beeper::~Beeper() {
    SDL_CloseAudio();
}

void Beeper::beep(double freq, int duration) {
    Beep beep;
    beep.freq = freq;
    beep.samplesLeft = duration * SAMPLING_RATE / 1000;
    SDL_LockAudio();
    beeps.push(beep);
    SDL_UnlockAudio();
}

void Beeper::generateSamples(int16_t* stream, int length) {
    int i = 0;
    while(i < length) {
        if(beeps.empty()) {
            while(i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        Beep& beep = beeps.front();
        int samplesToDo = std::min(i + beep.samplesLeft, length);
        beep.samplesLeft -= samplesToDo - i;
        while(i < samplesToDo) {
            stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / SAMPLING_RATE);
            i++;
            v += beep.freq;
        }
        if(beep.samplesLeft == 0) {
            beeps.pop();
        }
    }
}