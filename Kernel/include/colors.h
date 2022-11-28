#ifndef COLORS_H
#define COLORS_H

typedef struct color_t{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}color_t;

static color_t RED = {0xFF,0x00,0x00};
static color_t WHITE = {0xFF,0xFF,0xFF};
static color_t BLACK = {0x00,0x00,0x00};
#endif