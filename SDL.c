#include <SDL2/SDL.h>

#include "SDL.h"


#define TEX_SNAKE      "gfx/snake.bmp"
#define TEX_SNAKE_DEAD "gfx/snake_dead.bmp"
#define TEX_APPLE      "gfx/apple.bmp" 

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer);


E_SDL SDLHelper_init(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[SDL] could not initialize. error: %s\n", SDL_GetError());
		return E_SDL_INIT_VIDEO;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("[SDL] linear texture filtering not enabled.\n");
	}

	sdl_window = SDL_CreateWindow(
		WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (sdl_window == NULL) {
		printf("[SDL] window could not be created. error: %s\n", SDL_GetError());
		return E_SDL_WINDOW;
	}

	sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl_renderer == NULL) {
		printf("[SDL] renderer could not be created. error: %s\n", SDL_GetError());
		return E_SDL_RENDERER;
	}

	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	tex_snake = load_texture(TEX_SNAKE, sdl_renderer);
	tex_snake_dead = load_texture(TEX_SNAKE_DEAD, sdl_renderer);
	tex_apple = load_texture(TEX_APPLE, sdl_renderer);

	return E_SDL_SUCCESS;
}


void SDLHelper_quit(void)
{
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);
	sdl_renderer = NULL;
	sdl_window = NULL;

	SDL_Quit();
}


SDL_Texture* load_texture(char* path, SDL_Renderer* renderer)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* loaded_surface = SDL_LoadBMP(path);

	if (loaded_surface == NULL) {
		printf("[SDL] unable to load image %s. error: %s\n", path, SDL_GetError());
	} else {
        texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if (texture == NULL) {
			printf("[SDL] unable to create texture from %s. error: %s\n", path, SDL_GetError());
		}

		SDL_FreeSurface(loaded_surface);
	}

	return texture;
}
