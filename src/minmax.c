#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/param.h>
#include <limits.h>
#include "../include/domino.h"
#include "../include/minmax.h"

int eval(game_round* Round){
    int count = 0;
    player_hand *hand = Round->player[1];
    for(int i = 0; i<hand->size; i++){
        count += hand->hand[i]->right;
        count += hand->hand[i]->left;
    }
	return count;
}

/*
int minimax(game_round Round) {
    int bestMove = -1;
    int bestScore = INT_MIN;
    int currentScore;

    for (int i = 0; i < Round.player[1]->size; i++) {
        game_round* tempRound = roundCopy(Round);
        PlaceDomino(tempRound, i, 0);
        if (RoundEnded(tempRound)) {
            currentScore = Winner(tempRound);
            freeRound(tempRound);
            if (currentScore == 1) {
                return i;
            } else if (currentScore == 0) {
                bestMove = i;
                bestScore = 0;
                break;
            }
        } else {
            int opponentMove = minimax(*tempRound);
            currentScore = -Winner(tempRound);
            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestMove = i;
            }
            freeRound(tempRound);
        }
    }

    return bestMove;
}
*/
decision *minimax(game_round* Round, int depth, int maximizingPlayer){
    int pass = 0;
    int right = Round->board[Round->right_end]->right;
    int left = Round->board[Round->left_end]->left;
    decision *Rvalue;
	if (depth == 0 || RoundEnded(Round, pass)){
        Rvalue->eval = eval(Round);
        Rvalue->DomInd = -1;
        Rvalue->side = -1;
        return Rvalue;
	}
	if (maximizingPlayer) {
	    double	maxEval = -INFINITY;
        double eval;
		for(int i = 0; i<7; i++){
            if(i!=right){
                domino *newDom = CreateDomino(right, i);
                game_round *child = roundCopy(*Round);
                child->board[Round->right_end] = newDom;
                eval = minimax(child, depth-1, 0);
                maxEval = MAX(eval, maxEval);
            }
		return maxEval;
	}
	else {
		double minEval = INFINITY;
        double eval;
		for move {
			// make child
			eval = minimax(child, depth-1, 1);
			minEval = min(eval, maxEval);
		return minEval;
        }
    }
}

/*
int dumbAI(game *Game){
    int max[2] = {0, 0};
    player_hand *hand = Game->currentRound->player[1];
    for(int i = 0; i< hand->size; i++){
        int buffer = 0;
        if(ValidMove(Game, 1, i, 0)){
            buffer+=hand->hand[i]->right;
            buffer+=hand->hand[i]->left;
            if(buffer>max[0]){
                max[0] = buffer;
                max[1] = i;
            }
        }
        if(ValidMove(Game, 1, i, 0)){
            buffer+=hand->hand[i]->right;
            buffer+=hand->hand[i]->left;
            if(buffer>max[0]){
                max[0] = buffer;
                max[1] = i;
            }
        }
    }
    return max[1];
}
*/

