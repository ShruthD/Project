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
/*Sudoku Puzzle Solver.*/
#include        <stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"display.h"
#include	"sudoku.h"
static char * tupleNames [] = { "pair", "triplet", "quadruplet", "quintuplet", "sextuplet", "septuplet", "octuplet" };
static char tupleNameBuffer [100];
void displaySolution ( DISPLAY *display, PUZZLE *puzzle ) {
	int row, column, delay;
	delay = display->delay;
	display->delay = 0;
	for ( row = 0; row < 9; ++row ) {
		for ( column = 0; column < 9; ++column ) {
			if ( display->board [row][column] == 0 )
				displayEntry ( display, row, column, puzzle->grid [row][column] );
		}
  	}
   	display->delay = delay;
}
char *printTuple ( int tuple ) {
	char *tupleName = tupleNameBuffer;
	if ( tuple > 1 && tuple < 9 ) 
	   	tupleName = tupleNames [tuple-2];
	else 
	   	sprintf ( tupleNameBuffer, "%d", tuple );
	return tupleName;
}
int main ( int argc, char **argv ) {
  	int status = 1;
   	DISPLAY	*display;
   	int delay = 1000;
   	int analysis = 0;
   	if (display = (openDisplay(delay))) {
		PUZZLE	*puzzle;
		int	technique = 0;
		int	difficulty = 0;
		char	clues[82];
		int	row;
		int	i = 0;
		for ( row = 0; row < 9; ++row ) {
			int	column;
			for ( column = 0; column < 9; ++column ) {
				clues [i++] = '0' + display->board [row] [column];
			}
		}
		clues [i] = '\0';
		if(puzzle = (openPuzzleSolver(clues))) {
			HISTORY	*hist = NULL;
			if ( analysis ) {
				display->flags = 0;
				display->delay = 0;
			} else hist = openHistory ();
			status = solvePuzzle ( puzzle, hist );
			if ( status == 1 ) {
				difficulty = puzzle->difficulty;
				technique = puzzle->technique;
			}
			if ( hist ) {
				CELL	*coords;
				int	go = 1;
				if ( status > 1 )
				if ( display->delay )
					displayCaptions ( display, thinking, "" );
				while ( go && ( coords = popHistory ( hist ) ) ) {
					int	row = coords->row;
					int	column = coords->column;
					int	entry = puzzle->grid [row][column];
					go = displayEntry ( display, row, column, entry );
				}
				if ( go ) {
					if ( status == 1 )
						displayCaptions ( display, solved, "" );
					else if ( status > 1 ) {
						int	c;
						displayCaptions ( display, stumped, "" );
						c = confirm ( display, solveAnyway );
						if ( c == 'Y' || c == 'y' ) {
							displaySolution ( display, puzzle );
						} else go = 0;
						displayCaptions ( display, stumped, "" );
					} else displayCaptions ( display, noSolution, "" );
					if ( go ) confirm ( display, toExit );
				}
			}
			closePuzzle ( puzzle );
		}	
		closeDisplay ( display );
		if ( status == 1 ) {
		} else if ( analysis ) {
			if ( ! status ) fprintf ( stdout, "No solution possible.\n" );
			else  fprintf ( stdout, "Multiple solutions possible.\n" );
		}
   	}
   	return (status ^ 1);
}
