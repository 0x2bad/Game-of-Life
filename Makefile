gol: game_of_life.o core.o
	gcc -o gol game_of_life.o core.o -lSDL2_gfx -lSDL2 

game_of_life.o: game_of_life.c core.h
	gcc -g -c game_of_life.c -O0

core.o: core.c core.h
	gcc -g -c core.c -O0
