#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/domino.h"
#include "../include/graphics.h"
#include "../include/minmax.h"



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

int update(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, game Game){
    DrawBackground(renderer, texture);
    DrawHands(renderer, Game);
    DrawBoard(renderer, Game);
    DrawInfo(renderer, font, Game);
    SDL_RenderPresent(renderer);
    return 0;
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

	if ( RoundEnded(Game.currentRound, pass) ){
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

