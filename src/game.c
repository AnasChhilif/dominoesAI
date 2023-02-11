#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/domino.h"
#include "../include/graphics.h"


void PlaceDomino(game* Game);
int ValidMove(game* Game);

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
        if(event.key.keysym.sym == SDLK_p){
            PlayerInput = 6;
        }
    }
    return PlayerInput;
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

int update(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, game Game){
    DrawBackground(renderer, texture);
    DrawHands(renderer, Game);
    DrawBoard(renderer, Game);
//    DrawScore(renderer, font, Game);
    DrawInfo(renderer, font, Game);
    SDL_RenderPresent(renderer);
    return 0;
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
    if ( total1>total2 ) return 0;
    else return 1;
}

int main(int argc, char* argv[]) {

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    init_SDL(&window, &renderer, &font);

    printf("pre_game\n");
    game Game;
    printf("post Round\n");
    Game.roundNum = 1;
    Game.score[0] = 0;
    Game.score[1] = 0;
    Game.selected = 0;	 	// currently selected domino
    Game.side = 0;		// currently selected side  0: right, 1: left
    Game.currentRound = NewRound();
    int input = 0;

    // Game loop
    SDL_Texture* background = initBackground(renderer);
    int pass = 0;
    while (!SDL_QuitRequested()) {
	if (Game.currentRound->turn == 0){
		input = 0;
		input = get_input();

		// edit game data based on input
		HandleInput(input, &Game, &pass);

	}
	else {
		// Ai move
		// BotMove(&Game);

		input = 0;
		input = get_input();

		// edit game data based on input
		HandleInput(input, &Game, &pass);
	}

	if ( RoundEnded(&Game, pass) ){
		// TODO: following in a separate function InitRound
		UpdateScore(&Game,Winner(&Game));
		Game.currentRound = NewRound();
		Game.roundNum += 1;
        pass = 0;
	}
        // Then update screen
        update(renderer, background, font, Game);
        SDL_Delay(34);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

