/*
 * circular buffer test
 */

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "Snake.h"
#include "SDL.h"


#define GRID_WIDTH         10
#define GRID_HEIGHT        10
#define START_TICK_RATE    4
#define TICK_RATE_INCREASE 0.1

#define CELL_WIDTH      (WINDOW_WIDTH / GRID_WIDTH)
#define CELL_HEIGHT     (WINDOW_HEIGHT / GRID_HEIGHT)


typedef enum E_GAME {
	E_GAME_SUCCESS = 0,
	E_GAME_SDL,
} E_GAME;

void play(void);


int main(int argc, char** argv) {
	E_SDL sdl_e;
	if ((sdl_e = SDLHelper_init()) != E_SDL_SUCCESS) {
		printf("[Game] failed to initialize SDL. error: %d\n", sdl_e);
		return E_GAME_SDL;
	}
	play();
	SDLHelper_quit();

	return E_GAME_SUCCESS;
}


void play(void) {
	Snake* snake = Snake_create(GRID_WIDTH, GRID_HEIGHT);
	double tick_rate = START_TICK_RATE;

	struct timespec time_last_tick;
	struct timespec time_current;
	

	SDL_Event e;
	int quit = 0;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					Snake_turn(snake, DIR_LEFT);
					break;
				case SDLK_UP:
					Snake_turn(snake, DIR_UP);
					break;
				case SDLK_RIGHT:
					Snake_turn(snake, DIR_RIGHT);
					break;
				case SDLK_DOWN:
					Snake_turn(snake, DIR_DOWN);
					break;
				case SDLK_ESCAPE:
					quit = 1;
					break;
				}
			}
		}

		/* calculate time since last tick */
		clock_gettime(CLOCK_MONOTONIC, &time_current);
		long us_per_tick = (long)(1000000 / tick_rate);
		long us_last_tick = (time_last_tick.tv_sec * 1000000) + (time_last_tick.tv_nsec / 1000);
		long us_current = (time_current.tv_sec * 1000000) + (time_current.tv_nsec / 1000);
		long us_delta = us_current - us_last_tick;

		if (us_delta >= us_per_tick) {
			Update update_result = Snake_update(snake); 
			if (UPDATE_EAT == update_result) {
				tick_rate += TICK_RATE_INCREASE;
			}

			printf("t[%ld] ", us_delta);
			fflush(stdout);

			time_last_tick = time_current;
		}

		/* rendering */
		SDL_RenderClear(sdl_renderer);

		/* render snake */
		int length = Snake_length(snake);
		SDL_Texture* tex;
		if (Snake_state(snake) == STATE_LOST) {
			tex = tex_snake_dead;
		} else {
			tex = tex_snake;
		}

		SDL_Rect rect_dest;
		rect_dest.w = CELL_WIDTH;
		rect_dest.h = CELL_HEIGHT;

		for (int i = 0; i < length; ++i) {
			Pos pos = Snake_position(snake, i);
			rect_dest.x = pos.x * CELL_WIDTH;
			rect_dest.y = pos.y * CELL_HEIGHT;

			SDL_RenderCopy(sdl_renderer, tex, NULL, &rect_dest);
		}

		/* render apple */
		Pos apple_pos = Snake_apple(snake);
		rect_dest.x = apple_pos.x * CELL_WIDTH;
		rect_dest.y = apple_pos.y * CELL_HEIGHT;
		SDL_RenderCopy(sdl_renderer, tex_apple, NULL, &rect_dest);

		SDL_RenderPresent(sdl_renderer);
	}


	Snake_destroy(snake);
}