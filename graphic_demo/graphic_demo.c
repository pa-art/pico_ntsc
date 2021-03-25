/**
 * Graphic demonstration of NTSC signal generation
 * ATTENTION: You should compile this source with Release option of ARM compiler.
 * Mar.25, 2021 Pa@ART 
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico_ntsc_grph.h"
#include "mini_gamepad.h"
#include "graphic_demo.h"

static bool state = true;

// flip LED
void flip_led( void ) {
    if (state == true) {
        LEDON;
    } else {
        LEDOFF;
    }
    state = !state;
}

// draw a circle
void draw_circle( uint16_t x0, uint16_t y0, uint16_t r, uint8_t col ) {
    int x, y;
    for (int i = 0; i < CIRSTEP; i++) {
        x = x0 + r * cos(2.0 * PI * i / CIRSTEP);
        y = y0 + r * sin(2.0 * PI * i / CIRSTEP);
        if ((x > 0) && (x < GVRAM_W) && (y > 0) && (y < GVRAM_H)) {
            gvram_write(x, y, col);
        }
    }
}

int main() {
    // initialize video and LED GPIO
    init_video_and_led_GPIO();
    // init stdio
    stdio_init_all();
    // initialize random seed
    init_random();

    // initialize and start PWM interrupt by 64us period
    enable_PWM_interrupt( );

    // initialize key GPIO
    init_key_GPIO();

    int countup = 0;
    uint32_t keys;
    char mes[VRAM_W];
    bool state = true;
    int x0, y0, r0, px0, py0, pr0;
    px0 = py0 = pr0;

    while (1) {
        // monitoring process speed
        if (countup % 200000 == 0) {
            // flip LED
            flip_led();
        }
        // draw circle randomly 
        if (countup % 50000 == 0) {
            // draw message
            gvram_strings(150, 0, "Drawing a circle", WDOT);
            // randomly (x0, y0, r0)
            x0 = rand() % GVRAM_W;
            y0 = rand() % (GVRAM_H - CHAR_H) + CHAR_H;
            r0 = rand() % (GVRAM_W / 2);
            if (state == true) {
                // draw circle
                draw_circle(x0, y0, r0, WDOT);
                px0 = x0; py0 = y0; pr0 = r0;
            } else {
                draw_circle(px0, py0, pr0, BDOT);
            }
            state = !state;
        }

        countup++;
    }

    return 0;
}
