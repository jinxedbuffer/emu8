CC = g++
CFLAGS = -Wall -Wextra -std=c++17
INC = -I/usr/include -Isrc/include
LIBS = -lSDL2
SRC = src/main.cpp src/cpu.cpp src/gfx.cpp
OBJ = main.o cpu.o gfx.o

all: emu8 clean

emu8: $(OBJ)
	$(CC) $(CFLAGS) $(INC) $(OBJ) $(LIBS) -o $@

main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INC) $(LIBS) -c $< -o $@

cpu.o: src/cpu.cpp
	$(CC) $(CFLAGS) $(INC) $(LIBS) -c $< -o $@

gfx.o: src/gfx.cpp
	$(CC) $(CFLAGS) $(INC) $(LIBS) -c $< -o $@

clean:
	rm -f $(OBJ)