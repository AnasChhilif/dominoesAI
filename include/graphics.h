#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define DOMINO_WIDTH 50
#define DOMINO_HEIGHT 75
#define PIP_RADIUS 4

int init_SDL(SDL_Window **window, SDL_Renderer **renderer);
SDL_Texture* initBackground(SDL_Renderer *renderer);
int DrawBackground(SDL_Renderer *renderer, SDL_Texture* texture);
int DrawEmptyDomino(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, int vertical);
int DrawPips(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, int NumPips, int vertical);
int DrawDomino(SDL_Renderer *renderer, Sint16 x, Sint16 y, domino d, int selected, int vertical);
int DrawHands(SDL_Renderer *renderer, game Game);
int DrawBoard(SDL_Renderer *renderer, game Game);

#endif

