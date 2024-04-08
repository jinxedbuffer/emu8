#ifndef CHIP8_BEEPER_H
#define CHIP8_BEEPER_H

#define BEEP_FREQUENCY 3000
#define AMPLITUDE 2000
#define SAMPLING_RATE 11205

#include <cstdint>
#include <queue>

class Beep {
    public:
        double freq;
        int samplesLeft;
};

class Beeper {
    private:
        double v;
        std::queue<Beep> beeps;
    public:
        Beeper();
        ~Beeper();
        void beep(double freq, int duration);
        void generateSamples(int16_t* stream, int length);
};

#endif