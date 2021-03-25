gol: game_of_life.o core.o
	gcc -o gol game_of_life.o core.o -lSDL2_gfx -lSDL2 

game_of_life.o: game_of_life.c core.h
	gcc -c game_of_life.c -O2

core.o: core.c core.h
	gcc -c core.c -O2
