all:
	gcc src/game.c src/graphics.c src/domino.c -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -g -o z
clean:
	rm z
