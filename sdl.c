#include "sdl.h"

void sdl_init(int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL cannot be started: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Moon Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL) {
        SDL_Log("The window cannot be created: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Renderer cannot be created: %s", SDL_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;
}

void initializeSDLWindow(SDL_Window **window, SDL_Renderer **renderer) {
    sdl_init(640, 480, window, renderer);
}
