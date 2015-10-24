all: sudoku 

sudoku:sudoku.o countbits.o

sudoku.o: sudoku.h countbits.h

countbits.o: countbits.h


