#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/param.h>
#include "../include/domino.h"

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
int minimax(game_round* Round, int depth, int maximizingPlayer){
    domino* domino_set[28];
    for(int i = 0; i<7; i++){
        for(int j = 0; j<=i; j++){
            domino_set[c] = CreateDomino(i, j);
            c++;
        }
    }
	if (depth == 0 or RoundEnded(game_round* Round)){
		return eval(Round);
	}
	if (maximizingPlayer) {
	    double	maxEval = -INFINITY;
        double eval;
		for(int i = 0; i<28; i++){
            if(1)
			// make child
			eval = minimax(child, depth-1, 0);
			maxEval = max(eval, maxEval);

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
*/

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

