#include<stdio.h>
#include<assert.h>
#include<time.h>
#include<stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include"../include/domino.h"

/**
* @brief Creates an empty domino with initial values of right and left set to 0
*
* @param right the number of pips in right side of domino.
* @param left the number of pips in left side of domino.
*
* @return A domino variable with initial values set.
*/
domino* CreateDomino(int right, int left){
    domino* buffer = NULL;
    buffer = (domino*) malloc(sizeof(domino));
    buffer->right = right;
    buffer->left = left;
    return buffer;
}

/*
* @brief shuffles the domino deck.
*
* @param array array of deck to shuffle.
* @param n size of deck to shuffle.
*/
void shuffle(domino* array[], size_t n){
    srand(time(NULL));
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          domino* t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

/*
* @brief generates the hands of the players.
*
* @param hand1 pre-allocated pointer for the first hand.
* @param hand2 pre-allocated pointer for the second hand.
*/
void generate_hands(player_hand* hand1, player_hand* hand2){
    domino* domino_set[28];
    int c = 0;
    for(int i = 0; i<7; i++){
        for(int j = 0; j<=i; j++){
            domino_set[c] = CreateDomino(i, j);
            c++;
        }
    }
    shuffle(domino_set, 28);

    hand1->size = HAND_SIZE;
    hand2->size = HAND_SIZE;
    c = 0;
    for(int i = 0; i < HAND_SIZE; i++){
        hand1->hand[i] = domino_set[c];
        c++;
    }
    for(int i = 0; i < HAND_SIZE; i++){
        hand2->hand[i] = domino_set[c];
        c++;
    }

}

/*
* @brief Allocates new round, allocates hands and generates them.
*
* @return a game round with all initial values set.
*/
game_round* NewRound(){
    player_hand* hand1 = (player_hand*) malloc(sizeof(player_hand));
    player_hand* hand2 = (player_hand*) malloc(sizeof(player_hand));
    generate_hands(hand1, hand2);

    game_round* round = (game_round*) malloc(sizeof(game_round));
    round->player[0] = hand1;
    round->player[1] = hand2;
    round->left_end = 13;
    round->right_end = 13;
    round->turn = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
	round->board[i] = NULL;
    }
    return round;
}

/*
* @brief handles user input (moves cursor or places domino).
*
* @param input the user input.
* @param Game the game info.
* @param pass the pass variable (to increment in case of pass).
*/
int HandleInput(int input, game* Game, int *pass){
	if (input == 4){
		Game->selected ++;
		Game->selected %= Game->currentRound->player[Game->currentRound->turn]->size;
	}
	if (input == 3){
		Game->selected += Game->currentRound->player[Game->currentRound->turn]->size - 1;
		Game->selected %= Game->currentRound->player[Game->currentRound->turn]->size;
	}
	if (input == 1){
		Game->side = 0;
	}
	if (input == 2){
		Game->side = 1;
	}
	if (input == 5){
		if (ValidMove(Game)){
			PlaceDomino(Game);
			Game->currentRound->turn = 1 - Game->currentRound->turn;
            *pass = 0;
		}
		else {
			printf("Cant put THAT domino\n");
		}
	}
    if(input == 6){
        Game->currentRound->turn = 1 - Game->currentRound->turn;
        *pass += 1;
    }
}

/*
* @brief places domino into the board depending on the info in Game.
*
* @param Game the game info.
*/
void PlaceDomino(game* Game){
	player_hand* hand = Game->currentRound->player[Game->currentRound->turn];
	if (hand->size == 0){
		printf("Cant place domino, hand is empty\n");
		return;
	}
	domino* selectedDomino = hand->hand[Game->selected];

	// remove selected domino from the hand
	hand->size --;
	for (int i = Game->selected; i < hand->size; i++){
		hand->hand[i] = hand->hand[i+1];
	}
	if(Game->selected >= hand->size){
		Game->selected = hand->size - 1;
	}

	// Put it in correct side
	int left = Game->currentRound->left_end;
	int right = Game->currentRound->right_end;
	if (Game->side == 0){
		Game->currentRound->board[left] = selectedDomino;
		if (Game->currentRound->left_end == 13){
			Game->currentRound->left_end--;
			Game->currentRound->right_end++;
		}
		else {
			Game->currentRound->left_end--;
		}
	}
	if (Game->side == 1){
		Game->currentRound->board[right] = selectedDomino;
		if (Game->currentRound->right_end == 13){
			Game->currentRound->left_end--;
			Game->currentRound->right_end++;
		}
		else {
			Game->currentRound->right_end++;
		}
	}
}

/*
* @brief updates score of the winnder.
*
* @param Game the game info.
* @param winner the winner index.
*/
void UpdateScore(game* Game, int winner){
	// The loser index is 1-winner
	player_hand* hand = Game->currentRound->player[1-winner];
	int score = 0;
	for (int i = 0; i < hand->size; i++){
		score += hand->hand[i]->right;
		score += hand->hand[i]->left;
	}
	Game->score[winner] += score;
}

/*
* @brief turns the domino to face the right way befoe placing into the board.
*
* @param domino the domino to turn.
*/
void TurnDomino(domino* domino){
	int tmp = domino->left;
	domino->left = domino->right;
	domino->right = tmp;
}

/*
* @brief checks if the move that is getting played is valid.
*
* @param Game the game info.
*
* @return 0 if not valid, 1 if move is valid.
*/
int ValidMove(game* Game){
	player_hand* hand = Game->currentRound->player[Game->currentRound->turn];
	if (hand->size == 0){
		return 0;
	}

	domino* selectedDomino = hand->hand[Game->selected];

	int left = Game->currentRound->left_end;
	int right = Game->currentRound->right_end;
	int pips = 0;

	// First move is valid
	if (left == 13){
		return 1;
	}
	if (Game->side == 0){
		pips = Game->currentRound->board[left+1]->left;
		if (pips == selectedDomino->right) return 1;
		if (pips == selectedDomino->left){
			TurnDomino(selectedDomino);
			return 1;
		}
	}
	else if (Game->side == 1){
		pips = Game->currentRound->board[right-1]->right;
		if (pips == selectedDomino->left) return 1;
		if (pips == selectedDomino->right){
			TurnDomino(selectedDomino);
			return 1;
		}
	}
	return 0;
}

/*
* @brief checks if round has ended or not.
*
* @param Round the current round.
* @param pass how many times players have passed.
*
* @return "boolean" value. ("true" if round ended, "false" otherwise).
*/
int RoundEnded(game_round* Round, int pass){
	player_hand* hand1 = Round->player[0];
	player_hand* hand2 = Round->player[1];
	return hand1->size == 0 || hand2->size == 0 || pass == 2;
}

/*
* @brief determines the winnder of the game.
*
* @param Game the game info.
*
* @return the winner of the game (player[0] or player[1]).
*/
int Winner(game* Game){
	player_hand* hand1 = Game->currentRound->player[0];
	player_hand* hand2 = Game->currentRound->player[1];
	if ( hand1->size == 0 ) return 0;
	if ( hand2->size == 0 ) return 1;
    int total1 = 0, total2 = 0;
    for(int i = 0; i<hand1->size; i++){
        total1+= hand1->hand[i]->right;
        total1+= hand1->hand[i]->left;
    }
    for(int i = 0; i<hand2->size; i++){
        total2+= hand2->hand[i]->right;
        total2+= hand2->hand[i]->left;
    }
    if ( total1<=total2 ) return 0;
    else return 1;
}

/*
* @brief copies hands in current round to pre-allocated hand variables.
*
* @param hand1 the hand of the first player.
* @param hand2 the hand of the second player.
* @param Round the current round to copy.
*/
void handsCopy(player_hand* hand1, player_hand *hand2, game_round Round){
    hand1->size = Round.player[0]->size;
    hand2->size = Round.player[1]->size;
    for(int i = 0; i < Round.player[0]->size; i++){
        hand1->hand[i] = malloc(sizeof(domino));
        hand1->hand[i]->left = Round.player[0]->hand[i]->left;
        hand1->hand[i]->right = Round.player[0]->hand[i]->right;
    }
    for(int i = 0; i < Round.player[1]->size; i++){
        hand2->hand[i] = malloc(sizeof(domino));
        hand2->hand[i]->right = Round.player[1]->hand[i]->right;
        hand2->hand[i]->left = Round.player[1]->hand[i]->left;
    }
}

/*
* @brief copies the current round to a game_round variable it allocates.
*
* @param Round the current round.
*
* @return a game_round variable with its values copied from the current round.
*/
game_round *roundCopy(game_round Round){
    player_hand* hand1 = (player_hand*) malloc(sizeof(player_hand));
    player_hand* hand2 = (player_hand*) malloc(sizeof(player_hand));
    handsCopy(hand1, hand2, Round);

    game_round* round = (game_round*) malloc(sizeof(game_round));
    round->player[0] = hand1;
    round->player[1] = hand2;
    round->left_end = Round.left_end;
    round->right_end = Round.right_end;
    round->turn = Round.turn;
    for (int i = 0; i < BOARD_SIZE; i++){
        round->board[i] = NULL;
        if(Round.board[i]!=NULL){
            round->board[i] = (domino *) malloc(sizeof(domino));
            round->board[i]->right = Round.board[i]->right;
            round->board[i]->left = Round.board[i]->left;
        }
    }
    return round;
}

/*
* @brief frees the allocated memory space for the round.
*
* @param round the round to free.
*/
void freeRound(game_round *round){
    for(int i = 0; i<round->player[0]->size;i++){
        free(round->player[0]->hand[i]);
    }
    for(int i = 0; i<round->player[1]->size;i++){
        free(round->player[1]->hand[i]);
    }
    free(round->player[0]);
    free(round->player[1]);
    round->player[0] = NULL;
    round->player[1] = NULL;
    for(int i = 0; i<BOARD_SIZE; i++){
        if(round->board[i]!=NULL){
            free(round->board[i]);
            round->board[i]=NULL;
        }
    }
    free(round);
    round = NULL;
}

