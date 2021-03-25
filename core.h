#pragma once

#include <stdint.h>

#define WIDTH 64
#define HEIGHT 64

extern uint8_t *GOL_buff;
extern void (*render_row)(uint64_t);
void evolve();
