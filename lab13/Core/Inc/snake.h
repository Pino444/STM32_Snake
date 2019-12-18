/*
 * snake.h
 *
 *  Created on: 05.04.2019
 *      Author: Samsung
 */

#ifndef SNAKE_H_
#define SNAKE_H_



#include "stm32f1xx.h"

extern uint8_t s_lenght;

void draw_snake(void);

void snake_update(uint8_t x, uint8_t y);

void snake_start(void);

void candy(void);
#endif /* SNAKE_H_ */
