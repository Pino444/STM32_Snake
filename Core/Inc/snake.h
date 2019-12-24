#ifndef __SNAKE_H
#define __SNAKE_H

#include "lcd.h"

//扫描方向定义
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

void snake_init(uint16_t x, uint16_t y, uint16_t speed, uint8_t shape);

void snake_scan(uint16_t GPIO_Pin);

void snake_eat();

void snake_move();

void turnRight();

void turnLeft();

void refreshApple();

#endif
