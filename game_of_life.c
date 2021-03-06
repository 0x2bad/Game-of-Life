#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "core.h"

#define SDL_BLACK = {0};
#define SDL_WHITE = {255, 255, 255, 255};

SDL_Renderer *renderer;

void render_row_callback(uint64_t row)
{
    for (uint64_t i = 0; i < WIDTH; i++)
        if (GOL_buff[row*WIDTH + i])
            SDL_RenderDrawPoint(renderer, i, row);
}

// https://stackoverflow.com/questions/35165716/sdl-mouse-click
void drawPixel_click(SDL_MouseButtonEvent* b)
{
    uint32_t x;
    uint32_t y;
    printf("mouse button event detected\n");

    SDL_GetMouseState(&x, &y);
    switch (b->button) {
    case SDL_BUTTON_LEFT:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            GOL_buff[y*WIDTH + x] = 1;
            printf("left mouse detected\n");
            break;
    case SDL_BUTTON_RIGHT:
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            GOL_buff[y*WIDTH + x] = 0;
            printf("right mouse detected\n");
            break;
    }
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawPixel_hold(SDL_MouseMotionEvent* b)
{
    uint32_t x;
    uint32_t y;
    printf("mouse move event detected\n");

    switch (SDL_GetMouseState(&x, &y)) {
    case SDL_BUTTON(SDL_BUTTON_LEFT):
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            GOL_buff[y*WIDTH + x] = 1;
            break;
    case SDL_BUTTON(SDL_BUTTON_RIGHT):
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            GOL_buff[y*WIDTH + x] = 0;
            break;
    default:
            return;
    }
    SDL_RenderDrawPoint(renderer, x, y);
}

uint8_t start_drawing(FPSmanager *fpsmanager)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // clear with our set color
    SDL_RenderClear(renderer);
    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                    return 0;
            case SDL_MOUSEBUTTONDOWN:
                    drawPixel_click(&e.button);
                    break;
            case SDL_MOUSEMOTION:
                    drawPixel_hold(&e.motion);
                    break;
            case SDL_KEYUP:
                    return 1;
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(fpsmanager);
    }
}

void start_GOL(FPSmanager *fpsmanager)
{
    for (;;) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                return;

        evolve();

        SDL_RenderPresent(renderer);
        SDL_framerateDelay(fpsmanager);
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
                                          WIDTH, HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(! window) {
        fprintf(stderr, "Window could not be created!\n"
                        "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (! renderer) {
        fprintf(stderr, "Renderer could not be created!\n"
                        "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Set callback
    render_row = &render_row_callback;

    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 60);

    // Start the game
    if (start_drawing(&fpsmanager))
        start_GOL(&fpsmanager);


    {// clean up SDL
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

