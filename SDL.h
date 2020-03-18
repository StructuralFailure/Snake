#ifndef SDL_H
#define SDL_H


#include <SDL2/SDL.h>


#define WINDOW_TITLE  "Snake"
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 640


typedef enum E_SDL {
	E_SDL_SUCCESS = 0,
	E_SDL_INIT_VIDEO,
	E_SDL_TEX_FILTER,
	E_SDL_WINDOW,
	E_SDL_RENDERER,
} E_SDL;

SDL_Window*   sdl_window;
SDL_Renderer* sdl_renderer;

SDL_Texture*  tex_snake;
SDL_Texture*  tex_snake_dead;
SDL_Texture*  tex_apple;

E_SDL SDLHelper_init(void);
void  SDLHelper_quit(void);


#endif