#include<stdio.h>
#include<assert.h>
#include<time.h>
#include<stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include"../include/domino.h"

/**
*@brief Creates an empty domino with initial values of right and left set to 0
*
*@return A domino variable with initial values set.
*/
domino* CreateDomino(int right, int left){
    domino* buffer = NULL;
    buffer = (domino*) malloc(sizeof(domino));
    buffer->right = right;
    buffer->left = left;
    return buffer;
}


void shuffle(domino* array[], size_t n){
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          domino* t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

// Takes pre-allocated pointers for hands
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

// Allocates new round, allocates hands and generates them.
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
void TurnDomino(domino* domino){
	int tmp = domino->left;
	domino->left = domino->right;
	domino->right = tmp;
}

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

int RoundEnded(game* Game, int pass){
	player_hand* hand1 = Game->currentRound->player[0];
	player_hand* hand2 = Game->currentRound->player[1];
	return hand1->size == 0 || hand2->size == 0 || pass == 2;
}

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

void handsCopy(player_hand* hand1, player_hand *hand2, game Game){
    hand1->size = Game.currentRound->player[0]->size;
    hand2->size = Game.currentRound->player[1]->size;
    game_round* round = Game.currentRound;
    for(int i = 0; i < round->player[0]->size; i++){
        hand1->hand[i] = malloc(sizeof(domino));
        hand1->hand[i]->left = round->player[0]->hand[i]->left;
        hand1->hand[i]->right = round->player[0]->hand[i]->right;
    }
    for(int i = 0; i < round->player[1]->size; i++){
        hand2->hand[i] = malloc(sizeof(domino));
        hand2->hand[i]->right = round->player[1]->hand[i]->right;
        hand2->hand[i]->left = round->player[1]->hand[i]->left;
    }
}

game_round *roundCopy(game Game){
    player_hand* hand1 = (player_hand*) malloc(sizeof(player_hand));
    player_hand* hand2 = (player_hand*) malloc(sizeof(player_hand));
    handsCopy(hand1, hand2, Game);

    game_round* round = (game_round*) malloc(sizeof(game_round));
    round->player[0] = hand1;
    round->player[1] = hand2;
    round->left_end = Game.currentRound->left_end;
    round->right_end = Game.currentRound->right_end;
    round->turn = Game.currentRound->turn;
    for (int i = 0; i < BOARD_SIZE; i++){
        round->board[i] = NULL;
        if(Game.currentRound->board[i]!=NULL){
//            *(round->board[i]) = (domino) malloc(sizeof(domino));
            round->board[i]->right = Game.currentRound->board[i]->right;
            round->board[i]->left = Game.currentRound->board[i]->left;
        }
    }
    return round;
}

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

