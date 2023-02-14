#ifndef DOMINO_H
#define DOMINO_H

#define HAND_SIZE 7
#define BOARD_SIZE 27
// -13 --------- 0 ----------13
//   0 --------- 13----------27

typedef struct {
    int right, left;
}domino;

typedef struct {
    domino* hand[HAND_SIZE];
    int size;
}player_hand;

typedef struct game_round {
    player_hand* player[2];             // 0 is the player, 1 is the bot
    domino *board[BOARD_SIZE];
    int left_end, right_end;
    int turn;
}game_round;

typedef struct game{
    game_round* currentRound;
    int score[2];
    int roundNum;
    int selected;
    int side;
}game;


domino* CreateDomino(int right, int left);
void shuffle(domino* array[], size_t n);
void generate_hands(player_hand* hand1, player_hand* hand2);
game_round* NewRound();
void PlaceDomino(game* Game);
int ValidMove(game* Game);
int Winner(game* Game);
int RoundEnded(game_round* Round, int pass);
void TurnDomino(domino* domino);
void UpdateScore(game* Game, int winner);
int HandleInput(int input, game* Game, int *pass);
game_round *roundCopy(game_round cRound);
void handsCopy(player_hand* hand1, player_hand *hand2, game_round cRound);
void freeRound(game_round *round);

#endif

