#include <stdio.h>
#include <string.h>

int Sudoku[9][9][10];		
int backupSudoku[9][9][10];	

int remove_region_duplicates();
int remove_row_duplicates();
int remove_col_duplicates();
int check_for_row_unique();
int check_for_col_unique();
void check_depth();
void print_sudoku();
int check_completed();
void guess_number(int *row, int *col, int *num);
void backup_sudoku();
void restore_sudoku();

void print_sudoku(){
	int row, col;
	for(row=0; row<=8; row++){
		printf("[ "); 
		for(col=0; col<=8; col++){
			printf("%d ", Sudoku[row][col][0]); 
		}
		printf("]\n"); 
		
	}
	printf("\n");
}

int remove_region_duplicates(){
	int currentnum=0;
	int cmax=2, rmax=2, rindex=0, cindex=0;
	int checkrow=0, checkcol=0;
	int rowchanged=0, row, col;
	for(row=0; row<=8; row++){
		for(col=0; col<=8; col++){
			if(row==3 || row==6){
				if(rindex!=row){
					rindex+=3;
					rmax+=3;
				}
			}
			currentnum=Sudoku[row][col][0];
			if(currentnum!=0){
				for(checkrow=rindex; checkrow<=rmax; checkrow++){
					for(checkcol=cindex; checkcol<=cmax; checkcol++){
						if(Sudoku[checkrow][checkcol][currentnum]!=0){
							Sudoku[checkrow][checkcol][currentnum]=0;
							rowchanged=1;
						}
					}
				}
			}
			if(col==2 || col==5){
				cindex=cindex+3;
				cmax=cmax+3;
			}
			if(col==8){
				cindex=0;
				cmax=2;
			}
		}

	}
	return rowchanged;
}

int remove_row_duplicates(){

	int rowchanged =0;
	int currentnum=0, row, col, checkcol;

	for(row=0; row<=8; row++){
		for(col=0; col<=8; col++){
			if(Sudoku[row][col][0]!=0){
				currentnum=Sudoku[row][col][0];
				for(checkcol=0; checkcol<=8; checkcol++){
					if(Sudoku[row][checkcol][currentnum]!=0){
						Sudoku[row][checkcol][currentnum]=0;
						rowchanged=1;
					}
				}
			}
		}
	}
	return rowchanged;
}

int remove_col_duplicates(){
	int colchanged=0;
	int currentnum=0;
	int row, col, checkrow;
	for(row=0; row<=8; row++){
		for(col=0; col<=8; col++){
			if(Sudoku[row][col][0]!=0){
				currentnum=Sudoku[row][col][0];
				for(checkrow=0; checkrow<=8; checkrow++){
					if(Sudoku[checkrow][col][currentnum]!=0){
						Sudoku[checkrow][col][currentnum]=0;
						colchanged=1;
					}
				}
			}
		}
	}
	return colchanged;
}
int check_for_row_unique(){
	
}

int check_for_col_unique(){
	return 0;
}


void check_depth(){
}



int check_completed(){
	return 0;
}

void guess_number(int *row, int *col, int *num){
}


void backup_sudoku(){
}


void restore_sudoku(){
}

int main() {
}
