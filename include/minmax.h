#ifndef MINMAX_H
#define MINMAX_H

typedef struct {
    double eval;
    int DomInd; //-1 for non-bot moves, index of domino to place in hand for bot moves.
    int side;
}decision;

decision *minimax(game_round* Round, int depth, int maximizingPlayer);

#endif

