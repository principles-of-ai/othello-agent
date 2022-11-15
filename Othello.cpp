#include <iostream>
#include <map>
//#include <algorithm> 
#include <stdlib.h> 
#include <vector>
#include "OthelloBasic.h"
using namespace std;
//X is dark is 1, O is light is 0. for scoring, dark = 1 and light = -1
//moves inserted by row+column; #@; 1a, 4e


bool isLegalAgentMove(int** board, Point move, bool darkPlayer){
	if (outOfRange(move)) {
		//cout << "Out of range. ";
		return false; //intended move space out of range, not legal move
	}
	if (board[move.x][move.y] != 0) {
		//cout << "Space occupied. ";
		return false; //if move space is legal but space is already taken
	}
	//determine if any path is legal
	for (int k = 0; k < 8; k++) {
		if (isLegalPath(board, darkPlayer, move, k)) return true; //if even a single path is a legal path, it is a legal move
	}
	//cout << "No paths found. ";
	return false;
}






int findSafes(int** board) {
	Point corners[4] = {Point(0,0), Point(0,7), Point(7,0), Point(7,7)};
	int safe;
	for (int i = 0; i<4; i++) {
		safe += (10 * board[corners[i].x][corners[i].y]);
	}
	return safe;
}

double findHeuristic(int** board, bool max) {
	double h = findSafes(board);
	h += findMoves(board, max).size();
	h += 0.1*sumBoard(board); //the board itself is more a tiebreaker than anything
}

class Option {
	public:
		Move *opt; //
		double heur;
		
		Option (Move o) {
			opt = &o;
			heur = findHeuristic(opt->board, opt->dark);
		}
};

Option* findBestOption(vector<Option> options) {
	if (options.size() == 1) return &options[0]; //if only one option given, return best option
	//step 1, reduce to the absolute best options
	double best = options[0].heur, bestindex = 0;
	for (int i = 1; i < options.size(); i++) {
		if (options[i].heur > best) {
			best = options[i].heur;
			bestindex = i;
		}
	}
	vector<Option> bestOptions;
	
	for (int i = 0; i < options.size(); i++) {	
		if (options[i].heur == best) {
			bestOptions.push_back(options[i]);
			cout << options[i].opt->space.x+1 << char(97 + options[i].opt->space.y) << " ";
		}	
		cout << endl;
	}
	cout << "There are " << bestOptions.size() << " best options" << endl;	
	
	if (bestOptions.size() > 1) { //if multiple moves are equal heuristically
		return &bestOptions[rand()%bestOptions.size()]; //returns one at random
	} else return &bestOptions[0];
}

class Agent {
	public:
		bool max; 		//determines if agent is the dark player (true) or light player (false)
		int depth; 		//the depth of search the Agent uses to determine moves
		
		Agent (bool m, int d){
			max = m;
			//depth = d;
		}
		//depth 0 (current turn)
		//find current possible moves
		//for each, 

/*		
		vector<Move> predictOpponentMoves (int** initialBoard) { //predicts opponent moves
			vector<Move> moves = findMoves(initialBoard, max); 
			
			bestmoves.push_back(moves[bestindex]);
			for (int i = bestindex+1; i < moves.size(); i++) {//get all best moves possible
				if (sumBoard(moves[i].board) == best) { //for every other option of equal value
					bestmoves.push_back(moves[i]);
				}
			}
			
			int choose = rand()% bestmoves.size();	//randomly determine a move to make
			
			//process move for agent
			processMove(board, bestmoves[choose].space, max);
			return !max; //return the opposite of what the agent is for the next turn setting
		}
*/		

		bool takeAgentMove(int** board, bool turn) {	//determines all currently legal moves and predicts best path, then returns the best point to move
			vector<Move> moves = findMoves(board, turn);
			listPossibleMoves(moves, turn);
			vector<Option> options;
			options.push_back(Option(moves[0]));
			int best = options[0].heur, bestindex = 0;
			for (int i = 1; i < moves.size(); i++) { //find best boardsum option
				options.push_back(Option(moves[i]));
			}
			
			Option *bestOption = findBestOption(options); //once all options have been created with their heuristics, 
				if (turn) cout << "Dark";
				else cout << "Light";
			cout << " captures " << bestOption->opt->space.x+1 << char(97+bestOption->opt->space.y) << endl;
			processMove(board, bestOption->opt->space, turn);
			printBoard(board);
			printScore(board);
	//looks ahead once
	/*
			for (int i = 1; i<moves.size();i++) {
				if (max) { //wants best value 
					if (sumBoard(moves[m].board) > sumBoard(moves[i].board)) m = i; //update the best choice for max/dark
				} else {
					if (sumBoard(moves[m].board) < sumBoard(moves[i].board)) m = i; //update the best choice for min/light
				}	
				
			}
	*/		return !turn; //return the turn change
		}
};


void Othello (int players) {
	//LATER VER: use players to determine players; 2 = User vs User, 1 = User vs Agent, 0 = Agent vs Agent
	int** board;
	board = getFreshBoard();
	bool gameCompleted;
	bool darkCurrentTurn = true; //1 means dark's turn, 0 means light's
	bool pass = false;
	if (players == 2) {
	//	int turnCount = 0;
		while (!gameCompleted) {
	//		if (turnCount == 60) gameCompleted = true;

			if (hasLegalMoves(board, darkCurrentTurn)) {
				darkCurrentTurn = takeTurn(board, darkCurrentTurn);	//each turn ends by altering the current turn so this always alternates
				pass = false;
	//			turnCount++;
			} else {//no legal moves
				if (pass) { //if both players have no legal moves
					cout << "No legal moves remain." << endl;
					gameCompleted = true; //no legal moves means the game is over
				} else { //no player has passed yet
						if (darkCurrentTurn) cout << "Dark";
						else cout << "Light";
					cout << " player has no legal moves; passing turn." << endl;
					pass = true;
					darkCurrentTurn = !darkCurrentTurn;
				}
			}
		}		
	} else if (players == 1) {
		bool userIsDarkPlayer;
		cout << "Are you playing dark (Xs)?"<< endl << "Please type 0 for No or 1 for Yes." << endl;
		cin >> userIsDarkPlayer;
		
		int depth;
		cout << "Depth of opponent calculation?" << endl;
		cin >> depth;
		
		Agent opponent(!userIsDarkPlayer, depth);
		bool agentTurn = false;
			if (!userIsDarkPlayer) agentTurn = true;
		int at = 0;
		while (!gameCompleted) {
			if (hasLegalMoves(board, darkCurrentTurn)) {
				if (agentTurn) {
					cout << "agent turn" << at << endl;
					darkCurrentTurn = opponent.takeAgentMove(board, darkCurrentTurn);
					at++;
					agentTurn = false;
				} else {
					darkCurrentTurn = takeTurn(board, darkCurrentTurn); //
					agentTurn = true;
				}
			} else {
				if (pass) { //if both players have no legal moves
					cout << "No legal moves remain." << endl;
					gameCompleted = true; //no legal moves means the game is over
				} else {
						if (darkCurrentTurn) cout << "Dark";
						else cout << "Light";
					cout << " player has no legal moves; passing turn." << endl;
					pass = true;
					darkCurrentTurn = !darkCurrentTurn;
				}
			}
		}
	} else if (players == 0) { //two agent game
		int d;
		cout << "Depth of dark player Agent?" << endl;
		cin >> d;
		Agent darkAgent (true, d);
		cout << "Depth of light player Agent?" << endl;
		cin >> d;
		Agent lightAgent (false, d);
		
	}

	int winner = sumBoard(board);
	if (winner > 0) {	//board is positive, dark is more present
		cout << "Dark wins!!" << endl;
	} else if (winner < 0) { //board is negative, light is more present
		cout << "Light wins!!" << endl;
	} else {//tie
		cout << "Tie game!!" << endl;
	}
	
	cout << "Good game!" << endl;	
}



int main () {
	int** board = getFreshBoard();
	printBoard(board);
	Othello(1);
	return 0;
}


