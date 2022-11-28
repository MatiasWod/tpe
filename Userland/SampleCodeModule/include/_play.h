#ifndef PLAY_H
#define PLAY_H

#include <stdint.h>

#define DOWN    0
#define UP      1
#define LEFT    2
#define RIGHT   3

typedef struct color_t{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}color_t;

static color_t RED = {0xFF,0x00,0x00};
static color_t WHITE = {0xFF,0xFF,0xFF};
static color_t BLACK = {0x00,0x00,0x00};
static color_t BLUE = {0x00,0x00,0xFF};


#define WIDTH 128
#define HEIGHT 55
#define AREA WIDTH*HEIGHT

void play(unsigned int fd);
void drawMovement(char c, int player);
int checkCollision(int player);
void endGame();
#endif