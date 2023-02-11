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
    for (int i = 0; i < BOARD_SIZE; i++){
	round->board[i] = NULL;
    }
    return round;
}

