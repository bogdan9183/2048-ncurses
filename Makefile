all : 2048

2048 : 2048.o Game.o functions.o
	gcc -Wall $^ -o 2048 -lcurses
2048.o : 2048.c 
	gcc -Wall -c -g 2048.c
Game.o : Game.c
	gcc -Wall -c -g Game.c
functions.o : functions.c
	gcc -Wall -c -g functions.c 
run : 2048
	./2048
clean : 
	rm *.o 2048
