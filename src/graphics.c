#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../include/domino.h"
#include "../include/graphics.h"




int init_SDL(SDL_Window **window_p, SDL_Renderer **renderer_p){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

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
		x += 20;
	}
    printf("in Draw domino \n rend=%x \n x,y = %d,%d \n r,l = %d, %d\n ", renderer, x, y, d.right, d.left);
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

