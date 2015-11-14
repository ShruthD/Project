/*  Graphical sudoku solver
    Copyright (C) 2015 Shruti Dogra

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/* Sudoku Puzzle Solver, puzzle solution logic.*/

/* Technique flags. */
#define		SNL(a,b)	((a)=((a)&~(0xF))|(b))
#define		GNL(a)		((a)&(0xF))
#define		SHL(a,b)	((a)=((a)&~(0xF0))|((b)<<4))
#define		GHL(a)		(((a)&(0xF0))>>4)


/*Cell Coordinates.*/
typedef struct {
	int	row;
	int	column;
} CELL;

/* Solution history object.*/
typedef struct {
	int	head;
	int	list [81];
	CELL	cellCoords;
} HISTORY;


HISTORY *openHistory ();
#define	closeHistory(d)	free((void*)d)
int	pushHistory ( HISTORY *hist, int row, int col );
CELL	*popHistory ( HISTORY *hist );


/*Sudoku puzzle object.*/
typedef struct {
	int	difficulty;
	int	technique;
	char	grid [9][9];
	short	masks [9][9];
	short	rowMasks [9];
	short	columnMasks [9];
	short	blockMasks [3][3];
	HISTORY	*history;
} PUZZLE;

/* Puzzle solution function. */
int	solvePuzzle ( PUZZLE *, HISTORY * );
PUZZLE	*openPuzzleSolver ( char * );
void	closePuzzle ( PUZZLE * );
