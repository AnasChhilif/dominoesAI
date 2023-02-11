#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../include/domino.h"
#include "../include/graphics.h"

int X[27] = {962, 887, 811, 735, 660, 584, 507, 430, 384, 384, 430, 507, 584, 660, 735, 811, 887, 962, 962, 887, 811, 735, 660, 584, 507, 430, 380};
//int X[27] = {1032, 957, 881, 805, 730, 654, 577, 500, 454, 454, 500, 577, 654, 730, 805, 881, 957, 1032, 1032, 957, 881, 805, 730, 654, 577, 500, 450};
//int Y[27] = {340, 373, 373, 373, 373, 373, 373, 373, 373, 484, 491, 491, 491, 491, 491, 491, 491, 491, 580, 609, 609, 609, 609, 609, 609, 609, 609};
int Y[27] = {233, 260, 260, 260, 260, 260, 260, 260, 260, 333, 359, 359, 359, 359, 359, 359, 359, 359, 433, 457, 457, 457, 457, 457, 457, 457, 457};
int vertical[27] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1};

int init_SDL(SDL_Window **window_p, SDL_Renderer **renderer_p, TTF_Font** font_p){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }
    TTF_Init();
    *font_p = TTF_OpenFont("resources/font.ttf", 64); // Loading up the font

    // Create a window with the desired resolution
    *window_p = SDL_CreateWindow("My Window",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            WINDOW_WIDTH,
                                            WINDOW_HEIGHT,
                                            SDL_WINDOW_OPENGL);


    if (*window_p == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create a renderer for the window
    *renderer_p = SDL_CreateRenderer(*window_p, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer_p == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window_p);
        SDL_Quit();
        return -1;
    }
    return 0;
}
SDL_Texture* initBackground(SDL_Renderer *renderer){
    SDL_Surface* image = IMG_Load("resources/background.png");
    if (image == NULL) {
        printf("Failed to load image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
	exit(-1);
    }

    // Create a texture from the image
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == NULL) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(image);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
	exit(-1);
    }
    SDL_FreeSurface(image);
    return texture;

}
int DrawBackground(SDL_Renderer *renderer, SDL_Texture *texture){

    int width, height;
    width = 1600;
    height = 900;
    // Calculate the position to draw the image at
    SDL_Rect destination;
    destination.x = (WINDOW_WIDTH - width) / 2;
    destination.y = (WINDOW_HEIGHT - height) / 2;
    destination.w = width;
    destination.h = height;

    // Clear the window to black
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderClear(renderer);

    // Draw the image to the window
    SDL_RenderCopy(renderer, texture, NULL, &destination);
    return 0;

    return 0;
}

int DrawEmptyDomino(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, int vertical){
    if(vertical == 0){
        if(roundedBoxRGBA(renderer, x1, y1, x1 + DOMINO_HEIGHT, y1 + DOMINO_WIDTH, 5, 255, 255, 255, 255)==-1){
            printf("drawing empty Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }
        if(vlineRGBA(renderer, (2*x1+DOMINO_HEIGHT)/2,y1, y1+ DOMINO_WIDTH, 0, 0, 0, 255)==-1){
            printf("drawing middle line of Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }
    }
    else{
        if(roundedBoxRGBA(renderer, x1, y1, x1 + DOMINO_WIDTH, y1 + DOMINO_HEIGHT, 5, 255, 255, 255, 255)==-1){
            printf("drawing empty Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }
        if(hlineRGBA(renderer, x1 ,x1 + DOMINO_WIDTH,(2*y1+DOMINO_HEIGHT)/2 , 0, 0, 0, 255)==-1){
            printf("drawing middle line of Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }

    }
    return 0;
}

int DrawSelectedBorder(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, int vertical){
    if(vertical == 0){
        if(roundedRectangleRGBA(renderer, x1, y1, x1 + DOMINO_HEIGHT, y1 + DOMINO_WIDTH, 5, 0, 0, 0, 255)==-1){
            printf("drawing empty Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }
    }
    else{
        if(roundedRectangleRGBA(renderer, x1, y1, x1 + DOMINO_WIDTH, y1 + DOMINO_HEIGHT, 5, 0, 0, 0, 255)==-1){
            printf("drawing empty Domino failed");
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            return -1;
        }
    }
    return 0;
}

int DrawPips(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, int NumPips, int vertical){
    if(vertical == 0){
        if(NumPips == 1){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/4, y1 + DOMINO_WIDTH/2, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 2){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 3){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/4, y1 + DOMINO_WIDTH/2, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 4){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + DOMINO_WIDTH/4 , PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 5){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/4, y1 + DOMINO_WIDTH/2, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + DOMINO_WIDTH/4 , PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 6){
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/4, y1 + DOMINO_WIDTH/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/8, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_HEIGHT/4, y1 + (3*DOMINO_WIDTH)/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_HEIGHT)/8, y1 + DOMINO_WIDTH/4 , PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
    }
    else{
        if(NumPips == 1){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/2, y1 + DOMINO_HEIGHT/4, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 2){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 3){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/2, y1 + DOMINO_HEIGHT/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 4){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 5){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/2, y1 + DOMINO_HEIGHT/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
        else if(NumPips == 6){
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + DOMINO_HEIGHT/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + (3*DOMINO_HEIGHT)/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + DOMINO_HEIGHT/8, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + (3*DOMINO_WIDTH)/4, y1 + DOMINO_HEIGHT/4, PIP_RADIUS, 0, 0, 0, 255);
            filledCircleRGBA(renderer, x1 + DOMINO_WIDTH/4, y1 + (3*DOMINO_HEIGHT)/8 , PIP_RADIUS, 0, 0, 0, 255);
            return 0;
        }
    }
    return -1;
}

int DrawDomino(SDL_Renderer *renderer, Sint16 x, Sint16 y, domino d, int selected, int vertical){
	if (selected == 1){
		y -= 20;
	}
    //printf("in Draw domino \n rend=%x \n x,y = %d,%d \n r,l = %d, %d\n ", renderer, x, y, d.right, d.left);
    DrawEmptyDomino(renderer, x, y, vertical);
    DrawPips(renderer, x, y, d.left, vertical);
    if(vertical == 0){
        DrawPips(renderer, x + DOMINO_HEIGHT/2, y, d.right, vertical);
    }
    else{
        DrawPips(renderer, x, y + DOMINO_HEIGHT/2, d.right, vertical);
    }
    return 0;
}

int DrawHands(SDL_Renderer *renderer, game Game){
    player_hand* hand1 = Game.currentRound->player[0];
    player_hand* hand2 = Game.currentRound->player[1];
    domino** board = Game.currentRound->board;
    for (int i = 0; i < hand1->size; i++){
        if (Game.selected == i){
            DrawDomino(renderer, 600 + i*(DOMINO_WIDTH+10) , 800, *(hand1->hand[i]), 1, 1);
        }
        else{
            DrawDomino(renderer, 600  + i*(DOMINO_WIDTH+10), 800, *(hand1->hand[i]), 0, 1);
        }
    }
    for(int i = 0; i < hand2->size; i++){
        //DrawEmptyDomino(renderer, 600 + i*(DOMINO_WIDTH+10), 5, 1);
        DrawDomino(renderer, 600  + i*(DOMINO_WIDTH+10), 5, *(hand2->hand[i]), 0, 1);
    }
}


int DrawBoard(SDL_Renderer *renderer, game Game){
    game_round* round = Game.currentRound;
    domino** board = Game.currentRound->board;
    int end;

    if (board[0] != NULL) DrawDomino(renderer, X[0], Y[0], *board[0], 0, vertical[0]);
    for (int i = 1; i <= 7; i++){
        if (board[i] != NULL){
		domino tmp;
		tmp.right = board[i]->left;
		tmp.left = board[i]->right;
            DrawDomino(renderer, X[i], Y[i], tmp, 0, vertical[i]);
        }
//	else DrawEmptyDomino(renderer, X[i] , Y[i], vertical[i]);
    }
    for (int i = 8; i < 19; i++){
        if (board[i] != NULL){
            DrawDomino(renderer, X[i], Y[i], *board[i], 0, vertical[i]);
	}
//	else DrawEmptyDomino(renderer, X[i] , Y[i], vertical[i]);
    }
    for (int i = 19; i < 27; i++){
        if (board[i] != NULL){
		domino tmp;
		tmp.right = board[i]->left;
		tmp.left = board[i]->right;
            DrawDomino(renderer, X[i], Y[i], tmp, 0, vertical[i]);
        }
//	else DrawEmptyDomino(renderer, X[i] , Y[i], vertical[i]);
    }
    if (Game.side == 0){
        end = round->left_end;
    }
    if (Game.side == 1){
       end = round->right_end;
    }
    DrawSelectedBorder(renderer, X[end], Y[end], vertical[end]);
}

int DrawScore(SDL_Renderer *renderer, TTF_Font* font, game Game){
    //Displaying whatever text we want based on whatever dimensions we give
    SDL_Color color = {100, 100, 255}; // setting up the color of the text

    char score1[4], score2[4];
    char RoundNum[3];
    sprintf(score1, "%d", Game.score[0]);
    sprintf(score2, "%d", Game.score[1]);
    sprintf(RoundNum, "%d", Game.roundNum);

    //Declaring the surface variable
    SDL_Surface* surfaceMessage = NULL;
    SDL_Surface* surfaceMessage2 = NULL;
    SDL_Surface* surfaceMessage3 = NULL;

    surfaceMessage = TTF_RenderText_Blended(font, score1, color);
    surfaceMessage2 = TTF_RenderText_Blended(font, score2, color);
    surfaceMessage3 = TTF_RenderText_Blended(font, RoundNum, color);

    //printf("scores :%x %x \nfont: %x\n", score1, score2, font);
    //Testing if the text is rendered succesfully.
    if (surfaceMessage == NULL ){
	printf("Error creating surface : %s\n", SDL_GetError());
	exit(0);
    }
    if (surfaceMessage2 == NULL ){
	printf("Error creating surface : %s\n", SDL_GetError());
	exit(0);
    }
    if (surfaceMessage3 == NULL ){
	printf("Error creating surface : %s\n", SDL_GetError());
	exit(0);
    }

    //Creating the texture of the text to be displayed
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (Message == NULL ){
	printf("Error creating texture : %s\n", SDL_GetError());
	exit(0);
    }

    //Creating the texture of the text to be displayed
    SDL_Texture* Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
    if (Message2 == NULL ){
	printf("Error creating texture : %s\n", SDL_GetError());
	exit(0);
    }
    SDL_Texture* Message3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
    if (Message2 == NULL ){
	printf("Error creating texture : %s\n", SDL_GetError());
	exit(0);
    }

    SDL_Rect destination;
    destination.x = 100;
    destination.y = 100;
    destination.w = 24;
    destination.h = 24;
    //Copying the Message data to the renderer and storing the functions output for bug testing.
    int err3 = SDL_RenderCopy(renderer, Message3, NULL, &destination);
    destination.y = 200;
    int err = SDL_RenderCopy(renderer, Message, NULL, &destination);
    destination.x = 1300;
    int err2 = SDL_RenderCopy(renderer, Message2, NULL, &destination);
    if (err <0){
	printf("Error copying to renderer : %s\n", SDL_GetError());
	exit(0);
    }
    if (err2 <0){
	printf("Error copying to renderer : %s\n", SDL_GetError());
	exit(0);
    }
    if (err3 <0){
	printf("Error copying to renderer : %s\n", SDL_GetError());
	exit(0);
    }

    //Presenting the Result and freeing all unused memory space
    //SDL_RenderPresent(renderer);
    SDL_DestroyTexture(Message);
    SDL_DestroyTexture(Message2);
    SDL_DestroyTexture(Message3);
    SDL_FreeSurface(surfaceMessage);
    SDL_FreeSurface(surfaceMessage2);
    SDL_FreeSurface(surfaceMessage3);
}

