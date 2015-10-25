/*Sudoku Puzzle Solver, puzzle solution logic.*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	"sudoku.h"
#include	"countbits.h"
#include 	<ctype.h>
HISTORY *openHistory ( void ) {
	HISTORY	*hist;
	if ( hist = ( HISTORY * ) malloc ( sizeof ( HISTORY ) ) ) {
		int	i;
		hist->head = -1;
		for ( i = 0; i < 81; ++i ) {
			hist->list[i] = -1;
		}
	}
	return ( hist );
}

int pushHistory ( HISTORY *hist, int row, int column ) {
	int status = 0;
	int i = 9 * row + column;
	if( i < 81 && i >= 0 && hist->list [i] == -1 ) {
		status = 1;
		hist->list[i] = hist->head;
		hist->head = i;
	}
	return ( status );
}

CELL *popHistory ( HISTORY *hist ) {
	int i;
	CELL *coords = NULL;
	if ( hist->head > -1 ) {
		i = hist->head;
		hist->cellCoords.row = i / 9;
		hist->cellCoords.column = i % 9;
		coords = &( hist->cellCoords );
		hist->head = hist->list [i];
		hist->list [i] = -1;
	}
	return ( coords );
}

static void flipHistory ( HISTORY *hist ) {
	int head = hist->head;
	hist->head = -1;
	while ( head > -1 ) {
		int	i = head;
		head = hist->list [i];
		hist->list [i] = hist->head;
		hist->head = i;
	}
}

void closePuzzle ( PUZZLE *puzzle ) {
	if ( puzzle ) {
		if ( puzzle->history ) 
			closeHistory ( puzzle->history );
	free ( ( void * ) puzzle );
   	}
}

static void copyPuzzle ( PUZZLE *src, PUZZLE *dest ) {
	int puzzleSize = sizeof (PUZZLE);
   	char *p = ( char * ) src;
   	char *q = ( char * ) dest;
	while ( puzzleSize-- ) 
		*q++ = *p++;
   	dest->history = NULL;
}

static void clearPuzzle ( PUZZLE *puzzle ) {
	int i, j;
   	puzzle->technique = 0;
  	puzzle->difficulty = 0;
  	puzzle->history = NULL;
  	for ( i = 0; i < 9; ++i ) {
		puzzle->rowMasks [i] = (0x1FF);
		puzzle->columnMasks [i] = (0x1FF);
		for ( j = 0; j < 9; ++j ) {
			puzzle->grid [i][j] = 0;
			puzzle->masks [i][j] = (0x1FF);
		}
  	 }
  	for ( i = 0; i < 3; ++i )
		for ( j = 0; j < 3; ++j )
			puzzle->blockMasks [i][j] = (0x1FF);
}

PUZZLE *openPuzzle ( void ) {
	PUZZLE *puzzle = NULL;
   	/* Allocate the object and initialize it. */
	if ( puzzle = (PUZZLE *) malloc ( sizeof ( PUZZLE ) ) )
		clearPuzzle ( puzzle );
   	return puzzle;
}

static int makeEntry ( PUZZLE *puzzle, int i, int j, int entry ) {
	int status = 0, bit = digit2bit ( entry );
 	/* Is this a legal entry? */
   	if ( puzzle->rowMasks [i] & puzzle->columnMasks [j] & puzzle->blockMasks [i/3][j/3] & bit ) {
		status = 1;
		puzzle->rowMasks [i] ^= bit;
		puzzle->columnMasks [j] ^= bit;
		puzzle->blockMasks [i/3][j/3] ^= bit;
		puzzle->grid [i][j] = entry;
		puzzle->masks [i][j] = 0;
		if ( puzzle->history ) 
			pushHistory ( puzzle->history, i, j );
   	}
	return status;
}

static int enterClues(PUZZLE *puzzle, char *clues ) {
	int status = 0, row = 0, column = 0, clue;
   	while ( *clues ) {
		if ( isdigit ( *clues ) ) {
			clue = *clues - '0';
			if ( column > 8 ) {
				column = 0;
				if ( ++row > 8 ) 
					break;
			}
			if ( clue > 0 ) {
				if ( ! ( status	= makeEntry ( puzzle, row, column, clue ) ) )
					break;
			}
			++column;
		}
		++clues;
	}
	if(status) {
		for(row = 0; row < 9; ++row ) {
			for ( column = 0; column < 9; ++column ) {
				if ( puzzle->grid [row][column] == 0 )
					puzzle->masks [row][column] = ( puzzle->rowMasks [row] & puzzle->columnMasks [column] & puzzle->blockMasks [row/3][column/3] );
			}
		}
   	}
	return status;
}

PUZZLE *openPuzzleSolver ( char *clues ) {
	PUZZLE	*puzzle = NULL;
	/* Allocate the object and initialize it. */
	if ( puzzle = openPuzzle () ) {
		if ( ! enterClues ( puzzle, clues ) ) {
			closePuzzle ( puzzle );
			puzzle = NULL;
		}
   	}
	return puzzle;
}

static int countUnsolved ( PUZZLE *puzzle ) {
	int unsolved = 0;
	int i, j;
	for(i = 0; i < 9; ++i) {
		for(j = 0; j < 9; ++j) {
			if ( puzzle->grid [i][j] == 0 ) 
				++unsolved;
		}
   	}
	return unsolved;
}

static void updateRowMasks ( PUZZLE *puzzle, int row, int entry ) {
	int column;
   	int mask = ~digit2bit ( entry );
   	for(column = 0; column < 9; ++column ) {
		puzzle->masks [row][column] &= mask;
   	}
}

static void updateColumnMasks ( PUZZLE *puzzle, int column, int entry ) {
	int row;
	int mask = ~digit2bit ( entry );
	for(row = 0; row < 9; ++row ) {
		puzzle->masks [row][column] &= mask;
   	}
}

static void updateBlockMasks ( PUZZLE *puzzle, int row, int column, int entry ) {
  	int	rowMax = row + 3 - ( row % 3 );
   	int	columnMax = column + 3 - ( column % 3 );
   	int	mask = ~digit2bit ( entry );
	for ( row = rowMax - 3; row < rowMax; ++row ) {
		for ( column = columnMax - 3; column < columnMax; ++column ) {
			puzzle->masks [row][column] &= mask;
		}
   	}
}

static int missingDigitScan ( PUZZLE *puzzle ) {
   	int progress = 0;
   	int i, j, solution;
	puzzle->difficulty += 1;
   	for(i = 0; i < 9 && progress > -1; ++i ) {
		for ( j = 0; j < 9 && progress > -1; ++j ) {
			if ( puzzle->grid [i][j] == 0 ) {
				if ( solution = bit2digit ( puzzle->masks [i][j] ) ) {
					++progress;
					makeEntry ( puzzle, i, j, solution );
					updateRowMasks ( puzzle, i, solution );
					updateColumnMasks ( puzzle, j, solution );
					updateBlockMasks ( puzzle, i, j, solution );
				} else if(! puzzle->masks [i][j]) 
					progress = -1;
			}
		}
  	}
   	if ( progress > 0 ) puzzle->technique |= (0x400);
   	return progress;
}

static int crossHatch ( int *masks ) {
   	int progress = 0;
   	int solutions [9];
   	int i, j;
   	for(i = 0; i < 9; ++i) {
		solutions [i] = masks [i];
		for ( j = 0; j < 9; ++j ) if ( j != i ) solutions [i] &= ~masks [j];
	}
	for(i = 0; i < 9; ++i) {
		if(masks [i] = bit2digit(solutions [i] )) 
			++progress;
   	}
	return progress;
}
		

static int getRowMasks ( PUZZLE *puzzle, int row, int *masks ) {
   	int status = 0, mask = 0, column;
	if(puzzle->rowMasks [row]) {
		status = 1;
	/* Make masks for the row. */
		for(column = 0; column < 9; ++column) {
			masks [column] = puzzle->masks [row][column];
			mask |= masks [column];
		}
		if ( mask != puzzle->rowMasks [row] ) 
			status = -1;
   	}
	return status;
}


static int enterRowSolutions ( PUZZLE *puzzle, int row, int *solutions ) {
   	int progress = 0, column;
	/* Display the solutions. */
   	for(column = 0; column < 9; ++column) {
		if(solutions [column]) {
			if(makeEntry(puzzle, row, column, solutions [column])) {
				++progress;
				updateColumnMasks ( puzzle, column, solutions [column] );
				updateBlockMasks ( puzzle, row, column, solutions [column] );
			}
		}
   	}
	return progress;
}

static int getColumnMasks ( PUZZLE *puzzle, int column, int *masks ) {
	int status = 0, mask = 0, row;
 	/* Make sure the column is not already filled. */
   	if(puzzle->columnMasks [column]) {
		status = 1;
		/* Make masks for the column. */
		for ( row = 0; row < 9; ++row ) {
			masks [row] = puzzle->masks [row][column];
			mask |= masks [row];
		}
		if ( mask != puzzle->columnMasks [column] ) status = -1;
   	}
	return status;
}

static int enterColumnSolutions ( PUZZLE *puzzle, int column, int *solutions ) {
   	int progress = 0, row;
   	/* Enter the results of cross hatching the column. */
   	for(row = 0; row < 9; ++row) {
		if ( solutions [row] ) {
			if ( makeEntry ( puzzle, row, column, solutions [row] ) ) {
				++progress;
				updateRowMasks ( puzzle, row, solutions [row] );
				updateBlockMasks ( puzzle, row, column,
							solutions [row] );
			}
		}
	}
	return progress;
}


static int getBlockMasks ( PUZZLE *puzzle, int row, int column, int *masks ) {
	int status = 0, rowMax, columnMax, k, mask, i, j;
   	/* Make sure the 3x3 block is not already full. */
	if ( puzzle->blockMasks [row][column] ) {
		rowMax = ( row + 1 ) * 3;
		columnMax = ( column + 1 ) * 3;
		k = 0;
		mask = 0;
		status = 1;
		/* Make masks for the 3x3 block. */
		for(i = rowMax - 3; i < rowMax; ++i) {
			for ( j = columnMax - 3; j < columnMax; ++j ) {
				masks [k] = puzzle->masks [i][j];
				mask |= masks [k];
				++k;
			}
		}
		if ( mask != puzzle->blockMasks [row][column] ) status = -1;
   	}
	return status;
}


static int enterBlockSolutions ( PUZZLE *puzzle, int row, int column, int *solutions) {
	int progress = 0;
	int rowMax = ( row + 1 ) * 3;
   	int columnMax = ( column + 1 ) * 3;
   	int k = 0, i , j;
	for ( i = rowMax - 3; i < rowMax; ++i ) {
		for ( j = columnMax - 3; j < columnMax; ++j ) {
			if ( solutions [k] ) {
				if ( makeEntry ( puzzle, i, j, solutions [k] ) ) {
					++progress;
					updateRowMasks ( puzzle, i, solutions [k] );
					updateColumnMasks ( puzzle, j, solutions [k] );
				}
			}
			++k;
		}
   	}
   	return progress;
}


static int crossHatchScan ( PUZZLE *puzzle ) {
   	int	progress = 0;
   	int	i, j, masks[9], status;
   	puzzle->difficulty += 1;
   	/* Cross hatch the 3x3 blocks. */
   	for ( i = 0; i < 3 && progress > -1; i++ ) {
		for ( j = 0; j < 3 && progress > -1; j++ ) {
			if ( ( status = getBlockMasks ( puzzle, i, j, masks ) ) > 0 ) {
				if ( crossHatch ( masks ) ) {
					progress += enterBlockSolutions ( puzzle, i, j, masks );
				}
			} else if ( status < 0 ) 
				progress = -1;
		}
   	}
	if ( ! progress ) {
		puzzle->difficulty += 1;
		/* Cross hatch the rows. */
		for ( i = 0; i < 9 && progress > -1; ++i ) {
			if ( ( status = getRowMasks ( puzzle, i, masks ) ) > 0 ) {
				if ( crossHatch ( masks ) ) {
					progress += enterRowSolutions ( puzzle, i, masks);
				}
			} else if ( status < 0 ) 
				progress = -1;
		}
		/* Cross hatch the columns. */
		for ( i = 0; i < 9 && progress > -1; ++i ) {
			if ( ( status = getColumnMasks ( puzzle, i, masks ) ) > 0 ) {
				if ( crossHatch ( masks ) ) {
					progress += enterColumnSolutions ( puzzle, i,masks );
				}
			} else if ( status < 0 ) 
				progress = -1;
		}
		if ( progress > 0 ) 
			puzzle->technique |= (0x200);
   	} else if ( progress > 0 ) 
   		puzzle->technique |= (0x100);
   	return progress;
}

static int contradictions ( PUZZLE *puzzle ) {
	int progress = 0, minBitCount = 9, row, column, i, bit = 0x100, ambiguous = 0, mask, j, bits, entry, solutions, r, c;
	puzzle->difficulty += 1;
	/* Scan for a cell with a minimal candidate set. */
	for ( i = 0; ! progress && i < 9; i++ ) {
		for ( j = 0; ! progress && j < 9; j++ ) {
			bits = 0;
			if(puzzle->masks [i][j] && (bits = countBits(puzzle->masks [i][j])) == 2) {
				row = i;
				column = j;
				progress = 1;
			} else if(bits > 2 && bits < minBitCount) {
				minBitCount = bits;
				row = i;
				column = j;
			}
		}
	}
	mask = puzzle->masks [row][column];
	while(bit) {
		if(bit & mask) {
			PUZZLE	puzzle2;
			entry = bit2digit ( bit );
			copyPuzzle ( puzzle, &puzzle2 );
			makeEntry ( &puzzle2, row, column, entry );
			updateRowMasks ( &puzzle2, row, entry );
			updateColumnMasks ( &puzzle2, column, entry );
			updateBlockMasks ( &puzzle2, row, column, entry );
			if(!(solutions = solvePuzzle(&puzzle2, NULL))) {
				puzzle->masks [row][column] ^= bit;
				puzzle->technique = puzzle2.technique;
				progress = 1;
				break;
			} else if ( ambiguous || solutions > 1 ) {
				for ( r = 0; r < 9; ++r ) {
					for ( c = 0; c < 9; ++c ) {
						puzzle->grid [r][c] = puzzle2.grid [r][c];
					}
				}
				puzzle->difficulty = puzzle2.difficulty;
				progress = -1;
				break;
			} else 
				++ambiguous;
		}
		bit >>= 1;
	}
	if(progress > 0) 
		puzzle->technique |= (0x4000);
   	return progress;
}

int solvePuzzle ( PUZZLE *puzzle, HISTORY *hist ) {
	int solutions, progress = 1, i;
	puzzle->history = hist;
	solutions = countUnsolved ( puzzle );
	while ( solutions && progress > 0 ) {
		if((progress = crossHatchScan(puzzle)) || (progress = missingDigitScan(puzzle))) {
			if ( progress > 0 ) 
				solutions -= progress;
			else {
				solutions = -1;
				progress = 0;
			}
		} else 
			progress = contradictions ( puzzle );
   	}
   	if(puzzle->history)
   		flipHistory(puzzle->history);
   	if((solutions += 1) > 2) 
   		solutions = 2;
   	return solutions;
}

int main() {
	PUZZLE	*puzzle;
	int	technique = 0;
	int	difficulty = 0;
	int	row, status = 1;
	int	i = 0;
	char clues[] = {5,3,0,0,7,0,0,0,0,
		   6,0,0,1,9,5,0,0,0,
		   0,9,8,0,0,0,0,6,0,
		   8,0,0,0,6,0,0,0,3,
		   4,0,0,8,0,3,0,0,1,
		   7,0,0,0,2,0,0,0,6,
		   0,6,0,0,0,0,2,8,0,
		   0,0,0,4,1,9,0,0,5,
		   0,0,0,0,8,0,0,7,9,0}; 
	
	if ( puzzle = openPuzzleSolver(clues)) {
		HISTORY	*hist = NULL;
		hist = openHistory ();
		status = solvePuzzle ( puzzle, hist );
		if ( status == 1 ) {
			difficulty = puzzle->difficulty;
			technique = puzzle->technique;
		}
		if ( hist ) {
			CELL	*coords;
			int	go = 1;
			if ( status > 1 )
			while ( go && ( coords = popHistory ( hist ) ) ) {
				int	row = coords->row;
				int	column = coords->column;
				int	entry = puzzle->grid [row][column];
			}
		}
	}
	for ( i = 0; row < 9; ++row ) {
			int	column;
			printf("{ ");
			for ( column = 0; column < 9; ++column ) {
				printf("%d ", clues[i++]);
			}
			printf("}\n");
	}
	closePuzzle(puzzle);
	return 0;
}
