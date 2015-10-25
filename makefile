all: sudoku 

sudoku:sudoku.o countbits.o

sudoku.o: countbits.h

countbits.o: countbits.h


