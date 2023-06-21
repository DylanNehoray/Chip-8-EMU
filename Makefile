all:
	g++ -I src/include -L src/lib -o Chip8 sourcecode/main.cpp sourcecode/chip8.cpp -lmingw32 -lSDL2main -lSDL2