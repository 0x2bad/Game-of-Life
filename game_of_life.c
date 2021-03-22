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

// https://stackoverflow.com/questions/35165716/sdl-mouse-click
void drawPixel_click(SDL_MouseButtonEvent* b, SDL_Renderer* renderer)
{
    uint32_t x;
    uint32_t y;
    printf("mouse button event detected\n");

    switch (b->button) {
    case SDL_BUTTON_LEFT:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("left mouse detected\n");
            break;
    case SDL_BUTTON_RIGHT:
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            printf("right mouse detected\n");
            break;
    }
    SDL_GetMouseState(&x, &y);
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawPixel_hold(SDL_MouseMotionEvent* b, SDL_Renderer* renderer)
{
    uint32_t x;
    uint32_t y;
    printf("mouse move event detected\n");

    switch (SDL_GetMouseState(&x, &y)) {
    case SDL_BUTTON(SDL_BUTTON_LEFT):
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
    case SDL_BUTTON(SDL_BUTTON_RIGHT):
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            break;
    default:
            return;
    }
    SDL_RenderDrawPoint(renderer, x, y);
}

void start_game_of_life(SDL_Renderer *renderer, int w, int h)
{
//  uint8_t *screen_buffer = calloc(PIXEL_COUNT, 1);

    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 60);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // clear with our set color
    SDL_RenderClear(renderer);
    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                    return;
            case SDL_MOUSEBUTTONDOWN:
                    drawPixel_click(&e.button, renderer);
                    break;
            case SDL_MOUSEMOTION:
                    drawPixel_hold(&e.motion, renderer);
                    break;
            }
        }

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

    {// clean up SDL
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

