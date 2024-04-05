# EMU8
A CHIP-8 emulator (or interpreter)  written in C++ with SDL.

# Screenshots
![](/airplane.gif)

# Run
To run, type `./emu8 [game filename]`.<br>
Or you can run `make` to compile it from source code (make sure you have `libsdl2-dev` installed).

# Keypad
```
   ACTUAL CHIP8          EMU8
      KEYPAD            KEYPAD
    -----------       -----------
    | 1 2 3 C |       | 1 2 3 4 |
    | 4 5 6 D |       | Q W E R | 
    | 7 8 9 E |       | A S D F |
    | A 0 B F |       | Z X C V |
    -----------       -----------

```

# TODO
- [ ] Add sound
- [ ] GUI Interface 

# References
<ul>
    <li>[Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)</li>
    <li>[CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)</li>
    <li>[How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)</li>
</ul>