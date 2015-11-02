all: sudoku 

sudoku: LDLIBS += -lcurses

sudoku: main.o sudoku.o display.o countbits.o

main.o: sudoku.h display.h

display.o: display.h countbits.h

sudoku.o: sudoku.h countbits.h

countbits.o: countbits.h


