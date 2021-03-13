#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PIXEL_COUNT SCREEN_WIDTH * SCREEN_HEIGHT

#define SDL_BLACK = {0};
#define SDL_WHITE = {255, 255, 255, 255};

void start_game_of_life(SDL_Renderer *renderer, int w, int h)
{
//    uint8_t *screen_buffer = calloc(PIXEL_COUNT, 1);

    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 30);
    SDL_Rect rect = {.x = 8, .y = 16, .w = 32, .h = 64};;
    for (;;) {
        SDL_Event e;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        while (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                return;
            if((e.type == SDL_KEYDOWN))
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderClear(renderer);
        // SDL_RenderFillRect(renderer, &rect);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }
}

int main(int argc, char *argv[])
{
    {// Initialize SDL
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "SDL could not be initialized!\n"
                            "SDL_Error: %s\n", SDL_GetError());
            return 0;
        }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
        // Disable compositor bypass
        if (! SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
            printf("SDL can not disable compositor bypass!\n");
            return 0;
        }
#endif
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("game of life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(! window) {
        fprintf(stderr, "Window could not be created!\n"
                        "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (! renderer) {
        fprintf(stderr, "Renderer could not be created!\n"
                        "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Start the game
    start_game_of_life(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();
}

