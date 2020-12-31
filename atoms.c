#include <stdio.h>
#include <conio.h>
#include <memory.h>

// Game definitions
#define BOARDSIZE 8

// Game variables
int board[BOARDSIZE][BOARDSIZE];
int playerCell[BOARDSIZE][BOARDSIZE]; // 1 means player piece 0 means empty or computer owned

// Fast way to set all elements of board to 0
void ClearBoard() {
	memset(board,0,sizeof(board));
	memset(playerCell, 0, sizeof(playerCell));
}

// Draw the board with 3 spaces for every point
void DrawBoard() {
	printf("    1  2  3  4  5  6  7  8\n");
	for (int y = 0; y < 8; y++) {
		printf("%2d",y+1);
		for (int x = 0; x < 8; x++) {
			if (board[x][y] == 0) {
				printf("...");
			}
			else
			{
				printf("%2d", board[x][y]);
				if (playerCell[x][y] == 1)
					printf("P");
				else
					printf("C");
			}		
		}
		printf("%3d\n", y+1);
	}
	printf("    1  2  3  4  5  6  7  8\n");

}

// If move ok set it on the board and return 0. If a bad move return 1
int PlayMove(int x, int y) {
	x--; // array is 0..7,0..7 but we passed in 1..8 so subtract 1
	y--;
	if (playerCell[x][y] == 0 && board[x][y] > 0) {
		printf("You cannot play on (%d,%d) as it's computer owned", x, y);
		return 1;
	}
	board[x][y]++;
	playerCell[x][y] = 1;
	return 0;
}

// Read a single key from the keyboard
int ReadKbd() {
	int c,IsOk;
	do {
		c = _getch();
		IsOk = (c >= '1' && c <= '8') || (c == 27) || (c == 'q') || (c=='Q');
	}
	while (IsOk == 0);
	return c;
}

// Main game code
int main(int arcg, char* args) {
	int moveOk;
	ClearBoard();
	while (1) {
		printf("Your move X: 1-8 or Q/esc to exit:");
		int x = ReadKbd();
		if (x == 27 || x == 'q' || x == 'Q') {
			break;
		}
		printf("%c\nYour move Y: 1-8 or Q/esc to exit:",x);
		int y = ReadKbd();
		if (y == 27 || y == 'q' || y == 'Q') {
			break;
		}
		printf("%c\n",y);
		x -= '0';  // Convert '0' to 0, '1' to 1
		y -= '0';
		do {
		  moveOk=PlayMove(x, y); // 0 = ok, 1 = not ok
		} while (moveOk == 1);
		// PlayComputerMove(); To be coded
		DrawBoard();
	}
	printf("\n");
	return 0;
}
