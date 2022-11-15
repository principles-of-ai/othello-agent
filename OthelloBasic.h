#include <iostream>
#include <map>
//#include <algorithm> 
#include <stdlib.h> 
#include <vector>
using namespace std;

struct Point {
	int x, y;
    
    Point() {
    	x = y = 0;	
	}
	
	Point(int a, int b) {
		x = a;
		y = b;
	}
};

map <int, int> direcX {
    {0, -1},
    {1, 0},
    {2, 1},
    {3, -1},
    {4, 1},
    {5, -1},
    {6, 0},
    {7, 1}
};

map <int, int> direcY {
    {0, -1},
    {1, -1},
    {2, -1},
    {3, 0},
    {4, 0},
    {5, 1},
    {6, 1},
    {7, 1}
};

bool outOfRange(Point p) {
//	cout << p.x << ", " << p.y << endl;
	if (p.x < 0 || p.x > 7 || p.y < 0 || p.y > 7) return true;
//	cout << "here";	
	return false;
}

void printScore (int** board) {
	int dark = 0, light = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == -1) light++;
			if (board[i][j] == 1) dark++;
		}
	}
	cout << "Dark " << dark << " - " << light << " Light" << endl;
}

int sumBoard (int** board) {
	int sum = 0; 
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sum+= board[i][j];
		}
	}
	return sum;
}

int** getFreshBoard () {
	int** board = new int*[8];
	for (int i = 0; i < 8; i++) {
		board[i] = new int[8];
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			board[i][j] = 0;
		}
	}
	board[3][3] = 1;
	board[3][4] = -1;
	board[4][3] = -1;
	board[4][4] = 1;
	return board;
}

void printBoard(int** b) {
	cout << "   abcdefgh" << endl;
	for (int i = 0; i < 8; i++) {
		cout << i+1 << "[ ";
		for (int j = 0; j < 8; j++) {
			if (b[i][j] == -1)	cout << "O";
			else if (b[i][j] == 1) cout << "X";
			else cout << " ";
		}
		cout << " ]" << i+1 << endl;
	}
	cout << "   abcdefgh" << endl;
}

bool isLegalPath (int** board, bool darkPlayer, Point space, int direc) {
	
	int player = -1; //assume light
		if (darkPlayer) player = 1; //change to dark if dark
	
	Point change (direcX[direc], direcY[direc]);
	
	Point step(space.x + change.x, space.y + change.y); //one step in the intended direction creates a new poin
	if (outOfRange(step)) return false;//intended step out of range
//		cout << step.x << ", " << step.y << "(" << direc << "); " << board[step.x][step.y] << endl;
	 
	if (board[step.x][step.y] == 0) return false; //if step reaches an empty space, there isn't a possible path
	else if (board[step.x][step.y] == player) return false; //step matches player, also no possible path
	else 	{//step is found, possible path
		bool pathFound = false;
		while (!outOfRange(step) && !pathFound) {
			//take one more step
			step.x += change.x;
			step.y += change.y;
			if (outOfRange(step)) return false;
			if (board[step.x][step.y] == 0) return false;
			if (board[step.x][step.y] == player) return true; //if light path has reached another dark
		}
		return pathFound; //if loop broken because path found, true is returned. if loop broke because step was out of range, it returns false
	}
}

bool isLegalMove(int** board, Point move, bool darkPlayer){
	if (outOfRange(move)) {
		cout << "Out of range. ";
		return false; //intended move space out of range, not legal move
	}
	if (board[move.x][move.y] != 0) {
		cout << "Space occupied. ";
		return false; //if move space is legal but space is already taken
	}
	//determine if any path is legal
	for (int k = 0; k < 8; k++) {
		if (isLegalPath(board, darkPlayer, move, k)) return true; //if even a single path is a legal path, it is a legal move
	}
	cout << "No paths found. ";
	return false;
}

bool hasLegalMoves(int** board, bool player) {
//	return true; //troubleshooting ahoy
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
	
			if (board[i][j] == 0) { //step 1: check the empty spaces/ valid move areas
				for (int k = 0; k < 8; k++)	{//starting at NorthWest and going clockwise

					if (isLegalPath(board, player, Point(i, j), k)) return true; //soon as a legal move is found, the player has their turn
				}	
			}
		}	
	}
	return false; //if no legal moves are found, return false and end the game
}

void processMove (int** board, Point move, bool darkPlayer) {
	cout << "Processing " << move.x+1 << char(97+move.y) << " for ";
		if (darkPlayer) cout << "dark" << endl;
		else cout << "light" << endl;
	int place;
	if (darkPlayer) place = 1;
	else place = -1;
	board[move.x][move.y] = place;
	for (int k = 0; k< 8; k++) { //for each possible direction, determine if a path exists, and if so, to process that path
		if (isLegalPath(board, darkPlayer, move, k))	{ //if the path exists
			Point change (direcX[k], direcY[k]); //get step adjustment
			Point step(move.x + change.x, move.y + change.y); //first step
			while (board[step.x][step.y] != place) { //until you reach the next of the same color
				board[step.x][step.y] *= -1; //inverting the color of the disc
				step.x += change.x; //advance step
				step.y += change.y; //advance step
			}
		} //no else case since nothing happens if there is no path
	}
	
}

class Move { //a move needs nothing more than a space to go and the updated board 
	public: 
		Point space;	//the desired move
		bool dark;	//specifies if the mover is dark or light
		int** board;	//the updated board as a result of taking the move
		
		Move (Point p, bool d, int** b) {
			space.x = p.x;
			space.y = p.y;
			dark = d;
			board = b;
		}
};

vector<Move> findMoves(int** board, bool max) { //the agent determines its move within the board given
	vector<Move> possibleMoves;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (isLegalMove(board, Point(i,j), max)) { //add the processed move to the possible move list
				int** newBoard = board;
				possibleMoves.push_back(Move(Point(i,j), max, newBoard));
			}
		}
	}
	return possibleMoves;
}

void listPossibleMoves (vector<Move> v) {
	cout << "Possible moves: ";
	for (int i = 0; i < v.size(); i++) {
		cout << v[i].space.x+1 << char(97 + v[i].space.y);
		if (i != v.size()-1) cout << ", ";
	}
	cout << endl;
}

void listPossibleMoves (vector<Move> v, bool d) {
		if (d) cout << "Dark ";
		else cout << "Light ";
	cout << " moves: ";
	for (int i = 0; i < v.size(); i++) {
		cout << v[i].space.x+1 << char(97 + v[i].space.y);
		if (i != v.size()-1) cout << ", ";
	}
	cout << endl;
}

bool takeTurn(int** board, bool darkPlayer) {
	cout << "It is ";
		if (darkPlayer) cout << "dark's (X) turn.";
		else cout << "light's (O) turn.";
	cout << endl;
	bool turnEnd = false;
	while (!turnEnd) {
		//do turn stuff here
		//Player inputs their coordinates
			//if User player, prompt for coordinates to input with cout
		//coordinates are converted to a Point
		listPossibleMoves(findMoves(board,darkPlayer), darkPlayer);
		string in;
		cout << "Please enter your move in row-column format (e.g. 3f): ";
		cin >> in;
		if (in.length() < 2) {
			cout << "Too short. Not a legal move." << endl;
		} else {
			int x = int(in[0]) - 49; 
			int y = int(in[1]) - 97; 
			Point move(x,y);
//			cout << "Move is " << x << y << endl;
			if (!isLegalMove(board, move, darkPlayer)) {	//check if Point is a legal move
				cout << "Not a legal move." << endl;	//if not, tell player it is invalid and then recurse
				printBoard(board);
			} else {//else, process move and change turn
				processMove(board, move, darkPlayer);
					if (darkPlayer) cout << "Dark captures ";
					else cout << "Light captures ";
				cout << in << endl;
				printBoard(board);
				printScore(board);
				turnEnd = true;
			}
		}
	}
	//change darkCurrentTurn
	return !darkPlayer;	
}
