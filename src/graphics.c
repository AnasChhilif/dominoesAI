#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../include/domino.h"
#include "../include/graphics.h"

int X[27] = {962, 887, 811, 735, 660, 584, 507, 430, 384, 384, 430, 507, 584, 660, 735, 811, 887, 962, 962, 887, 811, 735, 660, 584, 507, 430, 380};
int Y[27] = {233, 260, 260, 260, 260, 260, 260, 260, 260, 333, 359, 359, 359, 359, 359, 359, 359, 359, 433, 457, 457, 457, 457, 457, 457, 457, 457};
int vertical[27] = {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1};

/*
* @brief initializing SDL, window, renderer, and the font.
*
* @param window_p address of window we are going to initialize
* @param renderer_p address of renderer we are going to initialize
* @param font_p address of font we are going to initialize
*
* @return function status flag (0 if it executed successfully and -1 if some error occured)
*/
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

/*
* @brief loading texture to avoid a memory leak down the line when updating screen
*
* @param renderer where we are going to create the texture
* @return the sdl texture of the background
*/
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

/*
* @brief drawing the background with the pre-loaded texture
*
* @param renderer the sdl renderer
* @param texture the background texture we loaded
*
* @return function execution status
*/
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

    // Draw the image to the window
    SDL_RenderCopy(renderer, texture, NULL, &destination);

    return 0;
}

/*
* @brief draws an empty (no circles) domino (vertical or horizontal)
*
* @param renderer the sdl renderer
* @param x1 X coordinate of the first point (i.e. top right) of the box.
* @param y1 Y coordinate of the first point (i.e. top right) of the box.
* @param vertical int to indicate if domino to be drawn vertically or horizontally.
*
* @return execution status
*/
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

/*
* @brief draws a rectangle to indicate where next domino would go
*
* @param renderer the sdl renderer
* @param x1 X coordinate of the first point (i.e. top right) of the box.
* @param y1 Y coordinate of the first point (i.e. top right) of the box.
* @param vertical int to indicate if rectangle to be drawn vertically or horizontally.
*
* @return execution status
*/
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

/*
* @brief draws the pips (dots) in semi domino.
*
* @param renderer the sdl renderer.
* @param x1 X of top left of the semi domino.
* @param y1 Y of top left of the semi domino.
* @param NumPips how many pips to draw in that semi domino.
* @param vertical int to indicate if domino is drawn vertically or horizontally.
*
* @return function execution status.
*/
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

/*
* @brief draws the domino with the pips on it.
*
* @param renderer the sdl renderer.
* @param x X of top right of domino.
* @param y Y of top right of domino.
* @param d domino to draw.
* @param boolean if domino is selected (we elevate it a little).
* @param boolean if domino is drawn vertically.
*/
int DrawDomino(SDL_Renderer *renderer, Sint16 x, Sint16 y, domino d, int selected, int vertical){
	if (selected == 1){
		y -= 20;
	}
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

/*
* @brief draws the player and adversaries hands (dominoes in their hands).
*
* @param renderer the sdl renderer.
* @param Game the game info.
*/
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
        DrawEmptyDomino(renderer, 600  + i*(DOMINO_WIDTH+10), 5, 1);
    }
}

/*
* @brief draws the board and the border for the next domino.
*
* @param renderer the sdl renderer.
* @param Game the game info.
*/
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
    }
    for (int i = 8; i < 19; i++){
        if (board[i] != NULL){
            DrawDomino(renderer, X[i], Y[i], *board[i], 0, vertical[i]);
	}
    }
    for (int i = 19; i < 27; i++){
        if (board[i] != NULL){
		domino tmp;
		tmp.right = board[i]->left;
		tmp.left = board[i]->right;
            DrawDomino(renderer, X[i], Y[i], tmp, 0, vertical[i]);
        }
    }
    if (Game.side == 0){
        end = round->left_end;
    }
    if (Game.side == 1){
       end = round->right_end;
    }
    DrawSelectedBorder(renderer, X[end], Y[end], vertical[end]);
}

/*
* @brief draws inputted text to the screen in the inputted coordinates.
*
* @param renderer the sdl renderer.
* @param text the text to draw.
* @param font the font to draw with.
* @param x the x coordinate of where to put the text.
* @param y the y coordinate of where to put the text.
*/
int DrawText(SDL_Renderer *renderer, char text[],TTF_Font* font, int x, int y){
    SDL_Color color = {255, 255, 255}; // setting up the color of the text
    SDL_Surface* surfaceMessage = NULL;
    surfaceMessage = TTF_RenderText_Blended(font, text, color);
    if (surfaceMessage == NULL ){
	printf("Error creating surface : %s\n", SDL_GetError());
	exit(0);
    }
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (Message == NULL ){
	printf("Error creating texture : %s\n", SDL_GetError());
	exit(0);
    }
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = 12 * strlen(text);
    destination.h = 24;
    int err = SDL_RenderCopy(renderer, Message, NULL, &destination);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

/*
* @brief draws the scores of players and the number of round.
*
* @param renderer the sdl renderer.
* @param font the font to draw with.
* @param Game the game info.
*/
int DrawInfo(SDL_Renderer *renderer, TTF_Font* font, game Game){
    char score1[4], score2[4];
    char RoundNum[3];
    sprintf(score1, "%d", Game.score[0]);
    sprintf(score2, "%d", Game.score[1]);
    sprintf(RoundNum, "%d", Game.roundNum);
    DrawText(renderer, score1, font, 1453, 168);
    DrawText(renderer, score2, font, 1453, 268);
    DrawText(renderer, RoundNum, font, 1473, 367);
}

