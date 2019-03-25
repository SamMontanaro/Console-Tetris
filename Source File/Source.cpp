// Program created by Michael Warmbier
// Functions identifiers written in camelCasing, variable identifiers in spaced_words
#include <iostream> // Input-Output
#include <string> // Used for system call to create game window
#include <conio.h> // Keyboard Input
#include <ctime> // sRNG
#include <Windows.h> // Windows functions
#include <iomanip> // I/O Manipulation
using namespace std;
/// Game Map
char map_temp[20][12]{ // Default map template
	'#','#','#','#','#','#','#','#','#','#','#','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ','S','T','A','R','T','?',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#','#','#','#','#','#','#','#','#','#','#','#'
};
char map_copy[20][12];
/// Program Variables
bool EXIT_Program = false, EXIT_Menu = false, EXIT_Game = false; // EXIT conditions
COORD coord; // Coordinate for setCursor
/// Game Variables
int score = 0, level = 1; // Score and level
unsigned char wall = 219u, block = 219u, background = ' '; // Character tiles
bool Game_Has_Started = false, Block_Is_Active = false, failure = false; // Conditions
int X_coord = 6, Y_coord = 1; // Reference coordinate for block creation and mapping
int next_block = 0, current_block = 0, block_position = 1; // Block variables
int counter = 0, counter_limit = 60; // Frame counter and its limit
bool may_move_left = true, may_move_right = true; // Movement conditions
int f_count = 0; // Frame counter for failureChec
/// Game Functions
void gameDraw(); // Draws the game map
void gameInput(); // Searches for input, then apply necessary changes
void gameLogic(); // Applys game logic
void clearStart(); // Clears start text
void drawMap(); // Output map
void setWalls(); // Set wall tiles
bool rotationCheck(); // Returns true if rotation may occur
void clearScreen(int); // Clear screen using one of two methods
void resetBlock(int); // Reset block to top of screen and finds next block
void mapBlock(); // Map block relative to reference coordinate position
void incrementY(); // Increment reference Y value
bool checkForCollision(); // Check for collision with floor or wall and return for floor
void checkForRow(); // Check to see if blocks landed in a row
void decrement(int); // Decrements all above rows after row is cleared
bool failureCheck(); // Check if game has been lost, return true if so
void loss(); // 'Animation' of failure
void copyMap(); // Copy map to use as reference
char getBlockVisual(); // Get block to use in Next Block visual.

void SetConsoleCursor(bool);
bool keyIsDown(char, bool = true, bool = true);
/////////////////////////////////////////////////////////////////////////// MAIN START
int main() {
	SetConsoleCursor(false);
	SetConsoleTitleA("Tetris");
	int gameWidth = 30, gameHeight = 24;
	system(("MODE " + to_string(gameWidth) + ", " + to_string(gameHeight)).c_str());
	do { // ALWAYS DO ONCE // While program runs..
		do { // ALWAYS DO ONCE // While menu runs..
			EXIT_Menu = true;
		} while (!EXIT_Menu);
		while (!EXIT_Game) { // While game runs..
			clearScreen(2);
			gameDraw();
			gameInput();
			gameLogic();
		}
	} while (!EXIT_Program);
	return EXIT_SUCCESS; // Exit successful
}
/////////////////////////////////////////////////////////////////////////// MAIN END
/// Sub-Functions
void clearStart() {
	for (int i = 1; i <= 10; i++) { // Set each character to a blank space
		map_temp[9][i] = ' ';
	}
}
void drawMap() {
	if (!failure) {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 12; j++) {
				if (!Game_Has_Started)
					if (i == 9 && j >= 2 && j <= 7)
						cout << map_temp[i][j];
				if (map_temp[i][j] == '#') { // Set all # to wall tiles
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
					cout << wall;
				}
				if (map_temp[i][j] == '@') { // Set all @ (moving blocks) to block tiles
					switch (current_block) {
					case 1:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						break;
					case 2:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
						break;
					case 3:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
						break;
					case 4:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						break;
					case 5:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
						break;
					case 6:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						break;
					case 7:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
						break;
					}
					cout << block;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				}
				if (map_temp[i][j] == '$') // Set all $ (stationary blocks) to block tiles
					cout << block;
				if (map_temp[i][j] == ' ') // Set empty spaces to.. well, empty spaces
					cout << background;
				if (i == 0 && j == 11)
					cout << wall << wall << wall << wall << wall << wall;
				if (i == 1 && j == 11)
					cout << "Next:" << wall;
				if (i == 2 && j == 11)
					cout << "  " << getBlockVisual() << "  " << wall;
				if (i == 3 && j == 11)
					cout << wall << wall << wall << wall << wall << wall;
			}
			cout << endl;
		}
	}
	else {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 12; j++) {
				if (map_copy[i][j] == '#') { // Set all # to wall tiles
					cout << wall;
				}
				if (map_copy[i][j] == '@') { // Set all @ (moving blocks) to block tiles
					cout << block;
				}
				if (map_copy[i][j] == '$') // Set all $ (stationary blocks) to block tiles
					cout << block;
				if (map_copy[i][j] == ' ') // Set empty spaces to.. well, empty spaces
					cout << background;
			}
			cout << endl;
		}
		cout << "---Game Over---";
	}
}
char getBlockVisual() {
	switch (next_block) {
	case 1:
		return 'O';
		break;
	case 2:
		return 'T';
		break;
	case 3:
		return 'Z';
		break;
	case 4:
		return 'S';
		break;
	case 5:
		return 'I';
		break;
	case 6:
		return 'J';
		break;
	case 7:
		return 'L';
		break;
	default:
		return '?';
		break;
	}
}
void clearScreen(int d) {
	coord.X = 0; // Set X to 0
	coord.Y = 0; // Set Y to 0
	switch (d) {
	case 1: // Clear screen using system("cls") to avoid issues with menu
		system("cls");
	case 2: // Clear screen using SetConsoleCursorPosition for faster clearing
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
}
void setWalls() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			if ((i == 0 || i == 19) || (j == 0 || j == 11))
				map_temp[i][j] = '#';
		}
	}
}
void resetBlock(int a) {   // Optional integer to set block
	srand(static_cast<int>(time(nullptr)));
	bool blockHasFallenOnce = false;
	if (failureCheck()) {
		failure = true;
		copyMap();
	}
	X_coord = 6;
	Y_coord = 1;
	// Reset coordinates
	if (a == 0) {
		if (next_block == 0) {
			blockHasFallenOnce = true;
			current_block = rand() % 7 + 1;
		}
		else
			current_block = next_block;
		next_block = rand() % 7 + 1;
		// Find next block
	}
	else {
		current_block = a;
		next_block = a;
	}
}
void mapBlock() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			if (map_temp[i][j] == '@')
				map_temp[i][j] = ' ';
		}
	}
	switch (current_block) { // Based on the current block, map..
	case 1: // O block
		switch (block_position) {
		case 1:
		case 2:
		case 3:
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord + 1] = '@';
		}
		break;
	case 2: // T block
		switch (block_position) {
		case 1:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			break;
		case 2:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			break;
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			break;
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			break;
		}
		break;
	case 3: // Z block
		switch (block_position) {
		case 1:
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			break;
		case 2:
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord - 1][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			break;
		}
		break;
	case 4: // S block
		switch (block_position) {
		case 1:
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			break;
		case 2:
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord + 1][X_coord + 1] = '@';
			break;
		}
		break;
	case 5: // I block
		switch (block_position) {
		case 1:
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord + 2][X_coord] = '@';
			break;
		case 2:
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord - 2] = '@';
			break;
		}
		break;
	case 6: // L block
		switch (block_position) {
		case 1:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord + 1] = '@';
			break;
		case 2:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord + 1] = '@';
			break;
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord - 1] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			break;
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			map_temp[Y_coord - 1][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			break;
		}
		break;
	case 7: // J block
		switch (block_position) {
		case 1:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			map_temp[Y_coord + 1][X_coord + 1] = '@';
			break;
		case 2:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			break;
		case 3:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord - 1] = '@';
			map_temp[Y_coord + 1][X_coord] = '@';
			break;
		case 4:
			map_temp[Y_coord][X_coord] = '@';
			map_temp[Y_coord - 1][X_coord + 1] = '@';
			map_temp[Y_coord][X_coord - 1] = '@';
			map_temp[Y_coord][X_coord + 1] = '@';
			break;
		}
		break;
	}
}
void incrementY() {
	Y_coord++;
}
bool checkForCollision() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			if (map_temp[i][j] == '@') {
				if (map_temp[i + 1][j] == '#' || map_temp[i + 1][j] == '$') { // If there is verticle collision..
					for (int k = 0; k < 20; k++) {
						for (int g = 0; g < 12; g++) {
							if (map_temp[k][g] == '@') // Blocks become stationary
								map_temp[k][g] = '$';
						}
					}
					Block_Is_Active = false; // New block falls
					return true;
				}
				if (map_temp[i][j - 1] == '#' || map_temp[i][j - 1] == '$')
					may_move_left = false;
				if (map_temp[i][j + 1] == '#' || map_temp[i][j + 1] == '$')
					may_move_right = false;
			}
		}
	}
	return false;
}
void checkForRow() {
	int row_counter = 0; // Counter for each landed block in row
	int column_counter = 0; // Counter for columns, necessary to detect 'Tetris'
	for (int i = 0; i < 20; i++) {
		for (int j = 1; j < 11; j++) {
			if (map_temp[i][j] == '$') // Increase row counter for each landed block found
				row_counter++;
		}
		if (row_counter == 10) { // If a row if full..
			// Visual
			Sleep(300);
			clearScreen(2);
			drawMap();
			column_counter++; // Add to column counter
			for (int s = 1; s < 11; s++) { // Clear row
				map_temp[i][s] = ' ';
			}
			decrement(i);
		}
		row_counter = 0; // Reset row counter for next row
	}
	if (column_counter != 0)
		score += static_cast<int>(pow(column_counter, 2) * 100); // Score multiplied by columns
	column_counter = 0;
}
void decrement(int r) {
	for (int i = r - 1; i > 1; i--) {
		for (int j = 0; j < 12; j++) {
			if (map_temp[i][j] == '$') {
				map_temp[i + 1][j] = '$';
				map_temp[i][j] = ' ';
			}
		}
	}
}
bool rotationCheck() {
	switch (current_block) {
	case 1: // O
		// No rotation on O block
		return true;
		break;
	case 2: // T
		switch (block_position) {
		case 1:
			if (map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$')
				return false;
		case 2:
			if (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$')
				return false;
		case 3:
			if (map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$')
				return false;
		case 4:
			if (map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$')
				return false;
		default:
			return true;
		}
	case 3: // Z
		switch (block_position) {
		case 1:
		case 3:
			if ((map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$') || (map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$'))
				return false;
		case 2:
		case 4:
			if ((map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$'))
				return false;
		default:
			return true;
		}
	case 4: // S
		switch (block_position) {
		case 1:
		case 3:
			if (map_temp[Y_coord + 1][X_coord - 1] == '#' || map_temp[Y_coord + 1][X_coord - 1] == '$')
				return false;
		case 2:
		case 4:
			if (map_temp[Y_coord - 1][X_coord + 1] == '#' || map_temp[Y_coord - 1][X_coord + 1] == '$')
				return false;
		default:
			return true;
		}
	case 5: // I
		switch (block_position) {
		case 1:
		case 3:
			if ((map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$') || (map_temp[Y_coord][X_coord - 2] == '#' || map_temp[Y_coord][X_coord - 2] == '$') || (map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$'))
				return false;
		case 2:
		case 4:
			if ((map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord + 2][X_coord] == '#' || map_temp[Y_coord + 2][X_coord] == '$') || (map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$'))
				return false;
		default:
			return true;
		}
		break;
	case 6: // L
		switch (block_position) {
		case 1:
			if ((map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$') || (map_temp[Y_coord + 1][X_coord + 1] == '#' || map_temp[Y_coord + 1][X_coord + 1] == '$') || (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$'))
				return false;
		case 2:
			if ((map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord - 1] == '#' || map_temp[Y_coord + 1][X_coord - 1] == '$'))
				return false;
		case 3:
			if ((map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$') || (map_temp[Y_coord - 1][X_coord - 1] == '#' || map_temp[Y_coord - 1][X_coord - 1] == '$') || (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$'))
				return false;
		case 4:
			if ((map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord - 1][X_coord + 1] == '#' || map_temp[Y_coord - 1][X_coord + 1] == '$'))
				return false;
		default:
			return true;
		}
	case 7: // J
		switch (block_position) {
		case 1:
			if ((map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$') || (map_temp[Y_coord - 1][X_coord - 1] == '#' || map_temp[Y_coord - 1][X_coord - 1] == '$') || (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$'))
				return false;
		case 2:
			if ((map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord - 1][X_coord - 1] == '#' || map_temp[Y_coord - 1][X_coord - 1] == '$'))
				return false;
		case 3:
			if ((map_temp[Y_coord][X_coord + 1] == '#' || map_temp[Y_coord][X_coord + 1] == '$') || (map_temp[Y_coord - 1][X_coord + 1] == '#' || map_temp[Y_coord - 1][X_coord + 1] == '$') || (map_temp[Y_coord][X_coord - 1] == '#' || map_temp[Y_coord][X_coord - 1] == '$'))
				return false;
		case 4:
			if ((map_temp[Y_coord - 1][X_coord] == '#' || map_temp[Y_coord - 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord] == '#' || map_temp[Y_coord + 1][X_coord] == '$') || (map_temp[Y_coord + 1][X_coord + 1] == '#' || map_temp[Y_coord + 1][X_coord + 1] == '$'))
				return false;
		default:
			return true;
		}
	default:
		return true;
	}
}
bool failureCheck() {
	for (int i = 0; i < 12; i++) {
		if (map_temp[2][i] == '$') {
			f_count++;
			break;
		}
	}
	if (f_count == 2)
		return true;
	else
		return false;

}
void copyMap() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			map_copy[i][j] = map_temp[i][j];
		}
	}
}
void loss() {
	system("color 4");
	Game_Has_Started = false;
	for (int i = 1; i < 19; i++) {
		for (int j = 1; j < 11; j++) {
			clearScreen(2);
			Sleep(10);
			map_copy[i][j] = '#';
			drawMap();
		}
	}
	Sleep(2000);
	EXIT_Program = true;
	EXIT_Game = true;
}
/// Primary Functions
void gameDraw() {
	drawMap();
	cout << endl << "   Score: " << score << endl;
	cout << "   Level: " << level << endl;
}
void gameInput() {
	checkForCollision();
	char enterKey = 13;
	if (keyIsDown(enterKey)) {
		if (!Game_Has_Started) {
			Game_Has_Started = true;
			clearStart();
		}
	}
	if (keyIsDown('A', true, false) && !keyIsDown('D', true, false)) {
		if (may_move_left)
			X_coord--;
		counter_limit = 60;
	}
	else if (keyIsDown('D', true, false) && !keyIsDown('A', true, false)) {
		if (may_move_right)
			X_coord++;
		counter_limit = 60;
	}
	else if (keyIsDown('S')) {
		if (counter != 0) {
			if (counter > 4)
				counter = 3;
			counter_limit = 4;
		}
	}
	else if (keyIsDown('Q', true, false)) {
		if (rotationCheck())
			block_position++;
	}
	else
		counter_limit = 60;
	may_move_right = true;
	may_move_left = true;
	/*
	if (_kbhit()) { // If a key is hit..
		switch (_getch()) { // Get the key..
		case 13: // ENTER
			if (!Game_Has_Started) {
				Game_Has_Started = true;
				clearStart();
			}
			break;
		case 'a':
			if (may_move_left)
				X_coord--;
			counter_limit = 60;
			break;
		case 'd':
			if (may_move_right)
				X_coord++;
			counter_limit = 60;
		case 's':
			if (counter != 0) {
				if (counter > 4)
					counter = 3;
				counter_limit = 4;
			}
			break;
		case 'q':
			if (rotationCheck())
				block_position++;
			break;
		}
	}
	else
		counter_limit = 60;
	may_move_right = true;
	may_move_left = true;
	*/
}
void gameLogic() {
	checkForCollision();
	if (block_position == 5)
		block_position = 1;
	if (Game_Has_Started) {
		counter++;
		if (Block_Is_Active) {
			mapBlock();
			if (counter == counter_limit) {
				if (!checkForCollision())
					incrementY();
				counter = 0;
			}
			mapBlock();
		}
		else {
			resetBlock(0); // Parameter 0 - Random; Parameter 1-7 - Associated block
			mapBlock();
			Block_Is_Active = true;
			block_position = 1;
		}
	}
	checkForCollision();
	checkForRow();
	setWalls();
	if (score > 1000)
		level = (score / 1000) + 1;
	if (failure) {
		loss();
	}
}

void SetConsoleCursor(bool input) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = input;
	SetConsoleCursorInfo(out, &info);
}

bool keyIsDown(char key, bool pressed, bool held) {
	int keyState = GetAsyncKeyState(static_cast<int>(key));
	return (pressed && (keyState & 1)) || (held && (keyState & 0xA000));
}
