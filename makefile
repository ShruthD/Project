project: main.o sudoku.o display.o countbits.o
	cc main.o sudoku.o display.o countbits.o -o project -lcurses

main.o:  main.c sudoku.h display.h
	cc -c main.c

display.o: display.h countbits.h
	cc -c display.c

sudoku.o: sudoku.h countbits.h
	cc -c sudoku.c
	
countbits.o: countbits.h
	cc -c countbits.c
