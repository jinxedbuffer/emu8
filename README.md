# EMU8
A CHIP-8 emulator (or interpreter)  written in C++ with SDL.

# Screenshots
![](/space_invaders.gif)

# Run
To run, type `./emu8 [game filename]`.<br>
Or you can run `make` to compile it from source code (make sure you have `libsdl2-dev` installed).

# Keypad
```
   ACTUAL CHIP8          EMU8
      KEYPAD            KEYPAD
    +---------+       +---------+
    | 1 2 3 C |       | 1 2 3 4 |
    | 4 5 6 D |       | Q W E R | 
    | 7 8 9 E |       | A S D F |
    | A 0 B F |       | Z X C V |
    +---------+       +---------+

```

# Customizatons

- To change the clock speed of CPU according to your need, just modify `CLOCK_SPEED` in `src/include/cpu.h`. By default, it has been set to 540hz.
- To change the frequency and volume of the beep sound, go to `src/include/beeper.h` and modify `BEEP_FREQUENCY` and `AMPLITUDE` respectively.
- To change the screen size of window, go to `src/include/gfx.h` and modify `SCALE_FACTOR` according to your need.

# TODO
- [x] Add sound
- [x] Performance improvements
- [ ] GUI Interface

# References
<ul>
    <li><a href="http://devernay.free.fr/hacks/chip8/C8TECH10.HTM">Cowgod's Chip-8 Technical Reference v1.0</a></li>
    <li><a href="https://en.wikipedia.org/wiki/CHIP-8">CHIP-8 Wikipedia</a></li>
    <li><a href="https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/">How to write an emulator (CHIP-8 interpreter)</a></li>
</ul>