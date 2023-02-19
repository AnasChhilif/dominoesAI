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


decision *minimax(game_round* Round, int depth, int maximizingPlayer){
    int pass = 0;
    int right = Round->board[Round->right_end -1]->right;
    int left = Round->board[Round->left_end + 1]->left;
    player_hand *bothand = Round->player[1];
    decision *Rvalue;
	if (depth == 0 || RoundEnded(Round, pass)){
        Rvalue = (decision *) malloc(sizeof(decision));
        Rvalue->eval = eval(Round);
        Rvalue->DomInd = -1;
        Rvalue->side = -1;
        return Rvalue;
	}
	if (maximizingPlayer) {
	    double	maxEval = -INFINITY;
        double eval;
		for(int i = 0; i<7; i++){
                domino *newDom = CreateDomino(right, i);
                game_round *child = roundCopy(*Round);
                child->board[Round->right_end] = newDom;
                ++child->right_end;
                Rvalue = minimax(child, depth-1, 0);
                maxEval = MAX(Rvalue->eval, maxEval);
                Rvalue->eval = maxEval;
                Rvalue->DomInd = -1;
                Rvalue->side = 1;
                freeRound(child);
        }
		for(int i = 0; i<7; i++){
                domino *newDom = CreateDomino(i, left);
                game_round *child = roundCopy(*Round);
                child->board[Round->left_end] = newDom;
                child->left_end--;
                Rvalue = minimax(child, depth-1, 0);
                maxEval = MAX(Rvalue->eval, maxEval);
                Rvalue->eval = maxEval;
                Rvalue->DomInd = -1;
                Rvalue->side = 1;
                freeRound(child);
        }
		return Rvalue;
	}
	else {
		double minEval = INFINITY;
        double eval;
		for (int i = 0; i<bothand->size; i++){
            if(bothand->hand[i]->right == right){
                domino *newDom = CreateDomino(bothand->hand[i]->left, right);
                game_round *child = roundCopy(*Round);
                child->board[Round->right_end] = newDom;
                child->right_end++;
                Rvalue= minimax(child, depth-1, 1);
                minEval = MIN(Rvalue->eval, minEval);
                Rvalue->eval = minEval;
                Rvalue->DomInd = i;
                Rvalue->side = 1;
                freeRound(child);
            }
            if(bothand->hand[i]->left == right){
                domino *newDom = CreateDomino(bothand->hand[i]->right, right);
                game_round *child = roundCopy(*Round);
                child->board[Round->right_end] = newDom;
                child->right_end++;
                Rvalue= minimax(child, depth-1, 1);
                minEval = MIN(Rvalue->eval, minEval);
                Rvalue->eval = minEval;
                Rvalue->DomInd = i;
                Rvalue->side = 1;
                freeRound(child);
            }
            if(bothand->hand[i]->right == left){
                domino *newDom = CreateDomino(left, bothand->hand[i]->left);
                game_round *child = roundCopy(*Round);
                child->board[Round->left_end] = newDom;
                child->left_end--;
                Rvalue= minimax(child, depth-1, 1);
                minEval = MIN(Rvalue->eval, minEval);
                Rvalue->eval = minEval;
                Rvalue->DomInd = i;
                Rvalue->side = 0;
                freeRound(child);
            }
            if(bothand->hand[i]->left == left){
                domino *newDom = CreateDomino(left, bothand->hand[i]->right);
                game_round *child = roundCopy(*Round);
                child->board[Round->left_end] = newDom;
                child->left_end--;
                Rvalue= minimax(child, depth-1, 1);
                minEval = MIN(Rvalue->eval, minEval);
                Rvalue->eval = minEval;
                Rvalue->DomInd = i;
                Rvalue->side = 0;
                freeRound(child);
            }
        }
		return Rvalue;
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

