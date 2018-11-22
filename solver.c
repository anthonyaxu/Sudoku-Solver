/* 
	C program that solves a Sudoku puzzle using the backtracking algorithm
	Created by Anthony Xu, 22nd November 2018
	Website: https://github.com/anthonyaxu/Sudoku-Solver
	Input Sudoku puzzles used for testing taken from http://lipas.uwasa.fi/~timan/sudoku/
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Define N as the size of the grid
#define N 9
// Define an unassigned number (0) as UNSOLVED
#define UNSOLVED 0

typedef struct Coords *Coordinates;

// Struct that contains the coordinates for the 2D puzzle array
typedef struct Coords {
	int row, col;
} Coords;

// Initialise coord struct
Coordinates newCoordinates() {
	Coordinates coord;
	if ((coord = malloc(sizeof(Coords))) == NULL) {
		fprintf(stderr, "ERROR!\n");
		exit(1);
	}
	coord->row = coord->col = -1;
	return coord;
}

// Checks the entire row to check if the given digit is allowed
// Returns false if the digit is already in the row
bool checkRow(int puzzle[N][N], int digit, int row) {
	for (int col = 0; col < N; col++) {
		if (puzzle[row][col] == digit) return false;
	}
	return true;
}

// Checks the entire column to check if the given digit is allowed
// Returns calse if the digit is already in the column
bool checkCol(int puzzle[N][N], int digit, int col) {
	for (int row = 0; row < N; row++) {
		if (puzzle[row][col] == digit) return false;
	}
	return true;
}

// Checks the surrounding 3x3 box to check if the given digit is allowed
// Returns false if the digit is already in the 3x3 box
bool checkBox(int puzzle[N][N], int digit, int row, int col) {
	int startRow, startCol, difference;
	difference = row % 3; 
	startRow = row - difference;

	difference = col % 3;
	startCol = col - difference;

	for (row = startRow; row < startRow + 3; row++) {
		for (col = startCol; col < startCol + 3; col++) {
			if (puzzle[row][col] == digit) return false;
		}
	}
	return true;
}

// Checks the entire 2D puzzle array for any 'unsolved' (value of 0) entries
// Returns a struct containing the row and column of the unsolved entry
Coordinates checkCompletion(int puzzle[N][N]) {
	Coordinates coord = newCoordinates();
	for (int row = 0; row < N; row++) {
		for (int col = 0; col < N; col++) {
			if (puzzle[row][col] == UNSOLVED) {
				coord->row = row;
				coord->col = col;
				return coord;
			}
		}
	}
	return coord;
}

// Checks if the given digit is allowed to be placed in the row and column
bool checkAllowed(int puzzle[N][N], int digit, int row, int col) {
	if (checkRow(puzzle, digit, row) && checkCol(puzzle, digit, col) 
		&& checkBox(puzzle, digit, row, col) && puzzle[row][col] == UNSOLVED) return true;
	return false;
}

// Prints out the values in the puzzle array
void printPuzzle(int puzzle[N][N]) {
	for (int row = 0; row < N; row++) {
		if (row % 3 == 0) {
			printf("+-----+-----+-----+\n");
		}
		for (int col = 0; col < N; col++) {
			if (col % 3 == 0) {
				printf("|");
			}
			printf("%d", puzzle[row][col]);
			if ((col + 1) % 3 != 0) {
				printf(" ");
			}
		}
		printf("|\n");
	}
	printf("+-----+-----+-----+\n");
}

// Recursive function to solve the Sudoku puzzle
bool solve(int puzzle[N][N]) {
	// Checks and returns the row and column of the first 'unsolved' entry
	Coordinates coord = checkCompletion(puzzle);
	if ((coord->row == -1) && (coord->col == -1)) return true;

	int row = coord->row;
	int col = coord->col;

	for (int digit = 1; digit <= 9; digit++) {
		if (puzzle[row][col] == UNSOLVED) {
			// Check if digit is allowed to be placed in the row and column
			if (checkAllowed(puzzle, digit, row, col)) {
				puzzle[row][col] = digit;
				// Moves down to the next 'unsolved' entry
				if (solve(puzzle)) return true;
				// If recursion fails, try again with next digit
				puzzle[row][col] = UNSOLVED;
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	int c, num, row, col;
	// Initialise a 9x9 grid
	int puzzle[N][N] = {0};

	// Makes sure that a text file has been inputed to program
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	// Check to make sure the file can be opened
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Could not open file %s\n", argv[1]);
		return 1;
	}

	row = col = 0;
	// Read all the numbers from input file to puzzle array
	while ((c = fgetc(f)) != EOF) {
		if (c != ' ' && c != '\n') {
			// Convert char to int
			num = c - '0';
			if (col == 9) {
				row++;
				col = 0;
			}
			puzzle[row][col] = num;
			col++;
		}
	}

	// Solve the sudoku, prints the grid when completed
	if (solve(puzzle)) {
		printf("The completed Sudoku puzzle is:\n");
		printPuzzle(puzzle);
	} else {
		printf("Sudoku cannot be solved\n");
	}
	return 0;
}
