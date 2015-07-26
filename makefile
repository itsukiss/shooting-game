all:
	gcc -c game2.c -g
	gcc game2.o -o game2 -l ncurses
	rm game2.o
