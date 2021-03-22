#pragma once

#include <stdint.h>

#define WIDTH 1920
#define HEIGHT 1080

extern uint8_t *GOL_buff;
extern void (*render_row)(uint64_t);
void evolve();
