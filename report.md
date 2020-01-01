# Report for CS301 Final Project Snake Game

## 1 Project Introduction

This project is to implement the Snake Game on STM32 with LCD display and keys to control. In Snake Game, player attempts to eat items by running into them with the head of the snake and each item eaten make the snake longer. The game is over when the snake runs into the screen border, obstacles or itself.

## 2 Group Members

11710911 梁峻豪: 25%

11710807 贺贵岩: 25%

11710809 李艺聪: 25%

11712923 叶致景: 25%

## 3 Development Environment

**STM32F103RC** development board with **STM32CubeMX**, **Clion**, **arm-none-eabi-gcc**(a cross compilation toolchain) and **OpenOCD**(for debugging and downloading program)

## 4 Project Design

1. Extract the Snake logic module into `snake.h` and its implementation `snake.c`. This module includes:
   1. The Snake Game initialization.
   2. Key detection and direction determination.
   3. Items and Obstacles generation and refresh.
   4. The Snake movement and direction change.
   5. Score calculation. 
   6. Speed and level control.
   7. Judgement of the game status.
   8. Status clear and game restart
2.  LCD driver and image rendering are using module `LCD.h` and its implementation `LCD.c` provided by ALIENTEK(正点原子). This module includes some utility functions we use in the Snake Game:
    1. Draw points, circles, lines and rectangles.
    2. Fill area with color.
    3. Write character, number and string.

## 5 Implementation of Main Function

> TODO

## 6 Screenshots

> TODO

## 7 Potential improvement

> TODO
