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
#define	closeHistory(deadMeat)	free((void*)deadMeat)
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
