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

uint8_t inv[INV_W * INV_H] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 
    1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 
    0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0
};

uint8_t cls[INV_W * INV_H] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

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

// draw a entity
void draw_entity( entity *e ) {
    int16_t cnt = 0;
    for (int16_t y = e->y; y < e->y + e->ch; y++) {
        for (int16_t x = e->x; x < e->x + e->cw; x++) {
            if ((x >= 0) && (x < GVRAM_W) && (y >= 0) && (y < GVRAM_H)) {
                gvram_write(x, y, e->c[cnt]);
            }
            cnt++;
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
    bool statec = true;
    bool states = true;
    bool statep = true;
    int x0, y0, r0;
    int hx, hy, px, py;
    entity pat1, pat2;

    hx = px = 0;
    pat1.x = 0; pat1.y = 0; pat1.c = inv; pat1.cw = INV_W; pat1.ch = INV_H;
    pat2.x = 0; pat2.y = 0; pat2.c = cls; pat2.cw = INV_W; pat2.ch = INV_H;

    while (1) {
        // monitoring process speed
        if (countup % 200000 == 0) {
            // flip LED
            flip_led();
        }
        // draw circle randomly 
        if (countup % 200000 == 0) {
            // draw message
            gvram_strings(50, 0, "Drawing a circle and a heart on sin curve", WDOT);
            if (statec == true) {
                // randomly (x0, y0, r0)
                x0 = rand() % GVRAM_W;
                y0 = rand() % (GVRAM_H - CHAR_H) + CHAR_H;
                r0 = rand() % (GVRAM_W / 2);
                // draw circle
                draw_circle(x0, y0, r0, WDOT);
            } else {
                draw_circle(x0, y0, r0, BDOT);
            }
            statec = !statec;
        }
        // draw sin curve
        if (countup % 100000 == 0) {
            for (int x = 0, y = 0; x < GVRAM_W; x++) {
                y = AMP - AMP * sin(2 * PI / GVRAM_W * x) + CHAR_H;
                gvram_write(x, y, GDOT);
            }
        }
        // draw heart mark on sin curve
        if (countup % 5000 == 0) {
            hy = AMP - AMP * sin(2 * PI / GVRAM_W * hx) + CHAR_H;
            if (states == true) {
                gvram_put_char(hx, hy, CHEART, WDOT);
            } else {
                gvram_put_char(hx, hy, ' ', WDOT);
                hx++;
                if (hx > GVRAM_W) hx = 0;
            }
            states = !states;
        }
        
        // draw inv on cos curve
        if (countup % 3000 == 0) {
            py = AMP - AMP * cos(2 * PI / GVRAM_W * px) + CHAR_H;
            pat1.x = px; pat1.y = py; pat2.x = px; pat2.y = py;
            if (statep == true) {
                draw_entity(&pat1);
            } else {
                draw_entity(&pat2);
                px++;
                if (px > GVRAM_W) { px = 0; }
            }
            statep = !statep;
        }

        countup++;
    }

    return 0;
}
