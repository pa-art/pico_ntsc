/**
 * Header file for
 * graphic demonstration of NTSC signal generation
 * ATTENTION: You should compile this source with Release option of ARM compiler.
 * Mar.25, 2021 Pa@ART
 */

#ifndef __GRAPHIC_DEMO__
#define __GRAPHIC_DEMO__

#define CHAR_W  8       // character width
#define CHAR_H  8       // character height
#define GX(x)   ((x) * CHAR_W)
#define GY(y)   ((y) * CHAR_H)

#define PI      3.14159265
#define CIRSTEP 256     // step size of drawing circle
#define CHEART  0x8     // heart mark
#define AMP     80
#define INV_W   16
#define INV_H   16

typedef struct {
    uint16_t x;         // character x
    uint16_t y;         // character y
    uint8_t *c;         // character pattern
    uint16_t cw;        // character width
    uint16_t ch;        // character height
} entity;

#endif