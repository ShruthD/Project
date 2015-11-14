Graphical sudoku solver

Shruti Dogra - 111403075

Description :
This is a sudoku solver which takes a few clues from the user and then solves the given sudoku. Even while the user is giving inputs it makes sure that no wrong input is given. Eg: If 8 is given as input the program first checks for other 8's in the row, column and block in which the given value is given as input. If there is any other 8 present, the 8 is not accepted as input.
For solving the puzzle, the program first scans the grid using cross-hatching on the blocks. If that doesn’t turn up anything it scans the grid again this time cross-hatching the rows and columns. If that doesn’t turn up anything it scans the grid using the missing digit technique. Again if that doesn’t turn up anything it repeats the scan using virtual cross-hatching. If that fails the next strategy is tuples analysis. If all those strategies fail, the program resorts to guessing.
