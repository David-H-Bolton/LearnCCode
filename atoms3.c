// Final version atoms. See https://learncgames.com/tutorials/atoms-game-aka-chain-reaction-an-alternative-c-tutorial/
// Copyright David Bolton. https://learncgames.com  (c) 2021
// You are free to copy, use or alter this but please leave these three comment lines in.
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>

// Game definitions
#define BOARDSIZE 8
#define MAXTRIES 1000
#define THRESHOLD 32
#define COMPUTEROWNED 2
#define PLAYEROWNED   1
#define UNOWNED 0
//#define DEBUGPLAYER

// Game variables
int board[BOARDSIZE][BOARDSIZE];
int playerCell[BOARDSIZE][BOARDSIZE]; // 1 means player piece 0 means empty or computer owned
int turnCount = 0;
int gameOver;

// Fast way to set all elements of board to 0
void ClearBoard() {
	memset(board, 0, sizeof(board));
	memset(playerCell, 0, sizeof(playerCell));
}

// Draw the board with 3 spaces for every point
void DrawBoard() {
#ifdef DEBUGPLAYER
	printf("    1  2  3  4  5  6  7  8");
#
#endif // DEBUGPLAYER
	printf("    1  2  3  4  5  6  7  8\n");
	for (int y = 0; y < 8; y++) {
		printf("%2d", y + 1);
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
		
		}	// Debug playerCell
#ifdef DEBUGPLAYER
		printf("%2d", y + 1);
		for (int x = 0; x < 8; x++) {
			if (playerCell[x][y] == 0) {
				printf("...");
			}
			else
			{
				if (playerCell[x][y] == 1)
					printf("PPP");
				else
					printf("CCC");
			}
		}
#endif // DEBUGPLAYER
		printf("%3d\n", y + 1);
	}
	printf("    1  2  3  4  5  6  7  8\n");
}

// Count number of computer owned cells
int CountCells() {
	int result = 0;
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) {
			if (board[x][y] > 0 && playerCell[x][y] == COMPUTEROWNED) {
				result++;
			}
		}
	return result;
}

// Game is over (return 1) if either player or Computer count == 0
int IsGameOver() {
	int playerCount = 0;
	int computerCount = 0;

	if (turnCount < 5) return 0; // Don't checvkj untiul turn 5
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) {
			if (board[x][y] > 0) {
				if (playerCell[x][y] == PLAYEROWNED) {
					playerCount++;
				}
				else 
					if (playerCell[x][y] == COMPUTEROWNED) {
					computerCount++;
				}
			}
		}	
	printf("Player: %d Computer: %d\n", playerCount, computerCount);
	if (playerCount > 0 && computerCount > 0)
		return 0; // Game not over
	if (playerCount > 0) {
		gameOver = 1;
	}
	else {
		gameOver = 2;
	}

	return 1; // Game is over if either count == 0
}

// Tries to find an empty cell, gives up after MAXTRIES tries
int FindEmptyCell(int* x, int* y) {
	int tries = 0;
	for (;;) {
		tries++;
		*x = rand() % BOARDSIZE;
		*y = rand() % BOARDSIZE;
		if (tries == MAXTRIES)
		{
			return 0;
		}
		if (board[*x][*y] == 0) {
			return 1;
		}
	};
}

// Finds random cell owned by computer
int FindComputerCell(int* x, int* y) {
	int tries = 0;
	do {
		tries++;
		*x = rand() % BOARDSIZE;
		*y = rand() % BOARDSIZE;
		if (tries == MAXTRIES)
		{
			return 0;
		}
		if (board[*x][*y] > 0 && playerCell[*x][*y] == COMPUTEROWNED) {
			return 1; // found one!
		}
	} while (1); // inmfinite loop
}

// Return 1 if coordinates are both in range 0..BOARDSIZE-1
int IsOnBoard(int x, int y) {
	if (x >= 0 && x < BOARDSIZE && y >=0 && y < BOARDSIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Return 1 if the coords are legit and there is a nonzero piece in the cell owned by a player
int IsPlayerPiece(int x, int y) {
	if (IsOnBoard(x, y) && board[x][y] >= 0 && playerCell[x][y] == PLAYEROWNED)
		{
			return 1;
		}
	else 
		{
			return 0;
		}

}

// returns 1 if the board has a player piece next to these coordinates
int NearPlayer(int x, int y) {
	if (IsPlayerPiece(x - 1, y)) return 1; // W
	if (IsPlayerPiece(x + 1, y)) return 1; // E
	if (IsPlayerPiece(x, y - 1)) return 1; // N
	if (IsPlayerPiece(x, y + 1)) return 1; // S
	return 0;
}

// Adds 1 to all four locations next to x and y (three on edges, two on corners)
void Explode(int x, int y, int owner) {
	if (!IsOnBoard(x, y)) return;
	board[x][y]++;
	playerCell[x][y] = owner;
	if (board[x][y] == 4) {
		board[x][y] = 0;
		Explode(x - 1, y, owner);
		Explode(x + 1, y, owner);
		Explode(x, y - 1, owner);
		Explode(x, y + 1, owner);
	}
}

// If move ok set it on the board and return 0. If a bad move return 1
int PlayMove(int x, int y, int owner) {  
	if (owner == PLAYEROWNED && playerCell[x][y] == COMPUTEROWNED && board[x][y] > 0) {
		printf("You cannot play on (%d,%d) as it's computer owned!\n", x+1, y+1);
		return 1;
	}
	Explode(x, y, owner);
	if (owner == COMPUTEROWNED) {
		printf("Computer plays %d,%d\n", x+1, y+1); // Computer plays 0-7 but display as 1-8
	}
	return 0;
}

// Returns 1 if it finds a player owned cell next to a computer owned cell
int FindComputerCellNearPlayer(int* x, int* y) {
	int tries = 0;
	do {
		if (!FindComputerCell(x, y))
			return 0; // Failed to find anywhere...
		if (NearPlayer(*x, *y)) {
			PlayMove(*x, *y, COMPUTEROWNED);
			return 1;
		}
		tries++;
	} while (tries < MAXTRIES);
	return 0;
}

// Computer makes a move
void PlayComputerMove() {
	int x, y;
	if (turnCount < 5 || (rand() %10) < 3) {  // 30% chance of picking empty cell or first 5 turns
		if (FindEmptyCell(&x, &y)) {
			PlayMove(x, y, COMPUTEROWNED);
		}
		else
		{
			FindComputerCellNearPlayer(&x, &y);
		}
	}
	else {
		FindComputerCellNearPlayer(&x, &y);
	}
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

// Gets the players move. If escape, q or Q is hit it returns 0 otherwise it returns 1 if a valid move has been made.
int GetPlayerMove() {
	int moveOk;
	do {
		printf("Your move X: 1-8 or Q/esc to exit:");
		int x = ReadKbd();
		if (x == 27 || x == 'q' || x == 'Q') {
			return 0;
		}
		printf("%c\nYour move Y: 1-8 or Q/esc to exit:", x);
		int y = ReadKbd();
		if (y == 27 || y == 'q' || y == 'Q') {
			return 0;
		}
		printf("%c\n", y);
		x -= '1';  // Convert '1' to 0, '2' to 1
		y -= '1';
		moveOk = PlayMove(x, y, 1); // 0 = ok, 1 = not ok
	} while (moveOk == 1);
	return 1;
}

// Main game code
int main(int arcg, char* args) {
	srand((int)time(0));
	ClearBoard();
	gameOver = 0;
	turnCount = 0;
	while (1) {
		if (!GetPlayerMove()) // Press escape, q etc.  - returns 0
			break;
		DrawBoard();
		if(IsGameOver()) break;
		PlayComputerMove(); 
		if(IsGameOver()) break;
		DrawBoard();
		turnCount++;
	}
	printf("\n");
	if (gameOver == 1) {
		printf("Congratulations- you won!");
	}
	else {
		printf("you were defeated by the computer player...");
	}
	return 0;
}
