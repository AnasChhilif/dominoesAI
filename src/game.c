#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/domino.h"
#include "../include/graphics.h"


void PlaceDomino(int playerIndx, game* Game);
int get_input(){
    SDL_Event event;
    int PlayerInput = 0;
    SDL_PollEvent(&event);
    if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_LEFT){
            PlayerInput = 1;
        }
        if(event.key.keysym.sym == SDLK_RIGHT){
            PlayerInput = 2;
        }
        if(event.key.keysym.sym == SDLK_UP){
            PlayerInput = 3;
        }
        if(event.key.keysym.sym == SDLK_DOWN){
            PlayerInput = 4;
        }
        if(event.key.keysym.sym == SDLK_SPACE){
            PlayerInput = 5;
        }
    }
    return PlayerInput;
}

int HandleInput(int input, game* Game){
	if (input == 4){
		Game->selected ++;
		Game->selected %= Game->currentRound->player[0]->size;
	}
	if (input == 3){
		Game->selected += Game->currentRound->player[0]->size - 1;
		Game->selected %= Game->currentRound->player[0]->size;
	}
	if (input == 1){
		Game->side = 0;
	}
	if (input == 2){
		Game->side = 1;
	}
	if (input == 5){
		PlaceDomino(0, Game);
	}
}

void PlaceDomino(int playerIndx, game* Game){
	player_hand* hand = Game->currentRound->player[playerIndx];
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
		Game->currentRound->left_end--;
	}
	if (Game->side == 1){
		Game->currentRound->board[right] = selectedDomino;
		Game->currentRound->right_end++;
	}
}

int update(SDL_Renderer* renderer, SDL_Texture* texture, game Game){
    DrawBackground(renderer, texture);
    DrawHands(renderer, Game);
    DrawBoard(renderer, Game);
    SDL_RenderPresent(renderer);
    return 0;
}


int main(int argc, char* argv[]) {

    int X[27] = {1275, 1185, 1094, 1003, 912, 821, 729, 637, 581, 581, 637, 729, 821, 912, 1003, 1094, 1185, 1275, 1275, 1185, 1094, 1003, 912, 821, 729, 637, 577};
    int Y[27] = [340, 373, 373, 373, 373, 373, 373, 373, 373, 484, 491, 491, 491, 491, 491, 491, 491, 491, 580, 609, 609, 609, 609, 609, 609, 609, 609];
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_SDL(&window, &renderer);

    printf("pre_game\n");
    game Game;
    Game.currentRound = NewRound();
    printf("post Round\n");
    Game.roundNum = 0;
    Game.score[0] = 0;
    Game.score[1] = 0;
    Game.selected = 0;	 	// currently selected domino
    Game.side = 0;		// currently selected side  0: right, 1: left
    int input = 0;

    // Game loop
    SDL_Texture* background = initBackground(renderer);
    while (!SDL_QuitRequested()) {

	input = 0;
        input = get_input();

        // edit game data based on input
	HandleInput(input, &Game);

        // Then update screen
        update(renderer, background, Game);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

