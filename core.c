#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 1920
#define HEIGHT 1080

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

void evolve(uint8_t *new, uint8_t *old)
{
    for (int i = 0; i < HEIGHT; i++) {
        compute_row(new + i*WIDTH,
                    old + (i - 1 + HEIGHT) % HEIGHT * WIDTH, // top
                    old + i * WIDTH, // middle
                    old + (i + 1 + HEIGHT) % HEIGHT * WIDTH); // bottom
    }
}

void print_grid(uint8_t *buff);

int main()
{
    uint8_t *new = calloc(WIDTH*HEIGHT, 1);
    uint8_t *old = calloc(WIDTH*HEIGHT, 1);
    new[11] = 1;
    new[12] = 1;
    new[13] = 1;

    for (int i = 0; i < 1000; i++) {
     //   print_grid(new);
        uint8_t *temp = old;
        old = new;
        new = temp;
        evolve(new, old);
    }
 //   print_grid(new);
}

void print_grid(uint8_t *buff)
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf((buff[WIDTH*i+j]) ? "1" : "0");
        printf("\n");
    }
    printf("\n");
}
