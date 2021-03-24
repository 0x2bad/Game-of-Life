#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "core.h"

void static inline
compute_row(uint8_t *new, uint8_t *old_top, uint8_t *old_mid, uint8_t *old_bot)
{
    uint8_t live_count;
    {// compute left-most pixel
        live_count = old_top[0] + old_top[1]
                                + old_mid[1]
                   + old_bot[0] + old_bot[1];
        switch (live_count) {
        case 2: new[0] = old_mid[0]; break;
        case 3: new[0] = 1; break;
        default: new[0] = 0;
        }
    }
    // compute pixels in between
    for (int i = 1; i < WIDTH-1; i++) {
        live_count = old_top[i-1] + old_top[i] + old_top[i+1]
                   + old_mid[i-1] +              old_mid[i+1]
                   + old_bot[i-1] + old_bot[i] + old_bot[i+1];
        switch (live_count) {
        case 2: new[i] = old_mid[i]; break;
        case 3: new[i] = 1; break;
        default: new[i] = 0;
        }
    }    
    {// compute right-most pixel
        live_count = old_top[WIDTH-2] + old_top[WIDTH-1]
                   + old_mid[WIDTH-2]
                   + old_bot[WIDTH-2] + old_bot[WIDTH-1];
        switch (live_count) {
        case 2: new[WIDTH-1] = old_mid[WIDTH-1]; break;
        case 3: new[WIDTH-1] = 1; break;
        default: new[WIDTH-1] = 0;
        }
    }
}


uint8_t *GOL_buff;
uint8_t *old_buff;

__attribute__ ((constructor))
void init_buffs()
{
    GOL_buff = calloc(WIDTH*HEIGHT, 1);
    old_buff = calloc(WIDTH*HEIGHT, 1);
}

void evolve()
{
    {// swap buffs
        uint8_t *temp = GOL_buff;
        GOL_buff = old_buff;
        old_buff = temp;
    }

    for (uint64_t i = 0; i < HEIGHT; i++) {
        compute_row(GOL_buff + i*WIDTH,
                    old_buff + (i - 1 + HEIGHT) % HEIGHT * WIDTH, // top
                    old_buff + i * WIDTH, // middle
                    old_buff + (i + 1 + HEIGHT) % HEIGHT * WIDTH); // bottom
    }
}
