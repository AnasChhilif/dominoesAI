#include<stdio.h>
#include<stdlib.h>
#include "../include/dominoes.h"

int eval(game_round* Round){
	return 0;
}
int minimax(game_round* Round, int depth, int maximizingPlayer){
	if (depth == 0 or RoundEnded(game_round* Round)){
		return eval(Round);
	}
	if (maximizingPlayer) {
		maxEval = -inf;
		for move {
			// make child
			eval = minimax(child, depth-1, 0);
			maxEval = max(eval, maxEval);
		}
		return maxEval;
	}
	else {
		minEval = +inf;
		for move {
			// make child
			eval = minimax(child, depth-1, 1);
			maxEval = min(eval, maxEval);
		return maxEval

