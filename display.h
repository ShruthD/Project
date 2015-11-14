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
#include	<curses.h>
#define		PUZZLE_TITLE	"Sudoku Solver"
#define		ESC		27	
#define		HT		0x09	
#define		CR		0x0D	
#define		DEL		0x7F	
#define		BIG		0x1
#define		isBig(a)	((a)->flags&BIG)
typedef struct {
	int	y0;	
	int	x0;	
	int	flags;	
	int	delay;
	short	rowMasks [9];
	short	columnMasks [9];
	short	blockMasks [3][3];
	char	board [9][9];
} DISPLAY;
DISPLAY	*openDisplay ( int );
extern const char	*gameName;
extern const char	*instructions1;
extern const char	*instructions2;
extern const char	*quit;
extern const char	*thinking;
extern const char	*paused;
extern const char	*stumped;
extern const char	*noSolution;
extern const char	*solved;
extern const char	*toExit;
extern const char	*solveAnyway;
int	confirm ( DISPLAY *, const char * );
void	displayCaptions ( DISPLAY *, const char *, const char * );
int	displayEntry ( DISPLAY *, int, int, int );
