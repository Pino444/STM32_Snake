//#include <cstdlib>
#include "stm32f1xx.h"
#include "lcd.h"
#include "gpio.h"
#include "snake.h"
#include "string.h"


uint8_t snake_pos = RIGHT;
uint8_t key;
short snake_alive = 0;
uint16_t snake_x[100];
uint16_t snake_y[100];
uint16_t snake_speed;
uint8_t snake_shape;
uint16_t apple_x = 100;
uint16_t apple_y = 100;
uint16_t stone_x = 50;
uint16_t stone_y = 150;
uint16_t snake_length = 5;
uint16_t apple_shape = 3;
short m;
short stone_cnt = 9;


void stateJudgement();

// 小蛇恰东西
void snake_eat(void) {            //判断是否吃到苹果
    if (abs(snake_x[0] - apple_x) < (snake_shape + apple_shape) &&
        abs(snake_y[0] - apple_y) < (snake_shape + apple_shape)) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        snake_length++;
        POINT_COLOR = WHITE;
        LCD_Draw_Circle(apple_x, apple_y, apple_shape);
        snake_x[snake_length] = snake_x[snake_length - 1];
        snake_y[snake_length] = snake_y[snake_length - 1];
        apple_x = rand() % (lcddev.width - 5);
        apple_y = rand() % (lcddev.height - 5);
        snake_speed -= 3;
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }
}


// 小蛇移动 蛇头是0
void snake_move(void) {
    switch (snake_pos) {
        case UP:
            snake_y[0] -= snake_shape * 2;
            break;
        case DOWN:
            snake_y[0] += snake_shape * 2;
            break;
        case LEFT:
            snake_x[0] -= snake_shape * 2;
            break;
        case RIGHT:
            snake_x[0] += snake_shape * 2;
            break;
        default:
            break;
    }
}

// 小蛇扫描
void snake_scan(uint16_t GPIO_Pin) {  //按键检测及方向确定
    switch (GPIO_Pin) {
        case KEY0_Pin:
            if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET) {
                turnRight();
            }
            break;
        case KEY1_Pin:
            if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_SET) {
                turnLeft();
            }
            break;
        default:
            break;
    }
}


void turnRight() {
    switch (snake_pos) {
        case UP:
            snake_pos = RIGHT;
            break;
        case RIGHT:
            snake_pos = DOWN;
            break;
        case DOWN:
            snake_pos = LEFT;
            break;
        default:
            snake_pos = UP;
            break;
    }
}

void turnLeft() {
    switch (snake_pos) {
        case UP:
            snake_pos = LEFT;
            break;
        case RIGHT:
            snake_pos = UP;
            break;
        case DOWN:
            snake_pos = RIGHT;
            break;
        default:
            snake_pos = DOWN;
            break;
    }
}

void refreshStone() {
    stone_cnt = 0;
    POINT_COLOR = WHITE;
    LCD_Draw_Circle(stone_x, stone_y, snake_shape);
    stone_x = rand() % (lcddev.width - snake_shape);
    stone_y = rand() % (lcddev.height - snake_shape);
    POINT_COLOR = RED;
    LCD_Draw_Circle(stone_x, stone_y, snake_shape);
}

void snake_init(uint16_t x, uint16_t y, uint16_t speed, uint8_t shape) { //主运行函数
    if (x % shape != 0) snake_x[0] = shape * 2;
    else snake_x[0] = x;
    if (y % shape != 0) snake_y[0] = shape * 2;
    else snake_y[0] = y;
    snake_speed = speed;
    snake_shape = shape;
    int i = 0;
    while (!snake_alive) {
        POINT_COLOR = BLACK;
//        snake_scan();
        if (i < snake_speed) {
            HAL_Delay(3);
            i++;
        } else {
            stone_cnt++;
            HAL_Delay(3);
            i = 0;
            snake_eat();
            snake_move();
            stateJudgement();
            while (snake_alive) {
                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                LCD_ShowString(lcddev.width / 2 - 40, lcddev.height / 2 - 16, 200, 16, 16, "Game Over!");
                LCD_ShowString(lcddev.width / 2 - 100, lcddev.height / 2, 200, 16, 16, "Press KEY_WK To Play Again!");
                HAL_Delay(100);
                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED0_Pin);

                if (HAL_GPIO_ReadPin(KEY_WK_GPIO_Port, KEY_WK_Pin) == GPIO_PIN_SET) {
                    memset(snake_x, 0, 100);
                    memset(snake_y, 0, 100);
                    snake_alive = 0;
                    snake_pos = RIGHT;
                    if (x % shape != 0) snake_x[0] = shape * 2;
                    else snake_x[0] = x;
                    if (y % shape != 0) snake_y[0] = shape * 2;
                    else snake_y[0] = y;
                    LCD_Clear(WHITE);
                }
            }
            if (stone_cnt == 25) refreshStone();
            POINT_COLOR = GBLUE;
            LCD_Draw_Circle(apple_x, apple_y, apple_shape);
            POINT_COLOR = WHITE;
            LCD_Draw_Circle(snake_x[snake_length], snake_y[snake_length], snake_shape);
            POINT_COLOR = BLACK;
            LCD_DrawRectangle(snake_shape - 1, snake_shape - 1, lcddev.width - snake_shape + 1,
                              lcddev.height - snake_shape + 1);
            LCD_Draw_Circle(snake_x[0], snake_y[0], snake_shape);
            for (m = snake_length; m >= 0; m--) {
                if (m > 0) {
                    snake_x[m] = snake_x[m - 1];
                    snake_y[m] = snake_y[m - 1];
                }
            }
        }
    }
}

void stateJudgement() {
    if (abs(snake_x[0] - stone_x) < 2 * snake_shape && abs(snake_y[0] - stone_y) < 2 * snake_shape) {
        snake_alive = 1;
        return;
    }
    if (snake_y[0] < snake_shape * 2 || snake_y[0] > lcddev.height - snake_shape ||
        snake_x[0] < snake_shape * 2 || snake_x[0] > lcddev.width - snake_shape) {
        snake_alive = 1;
        return;
    }

    for (m = snake_length; m > 0; m--) {
        if (snake_x[m] == snake_x[0] &&
            snake_y[m] == snake_y[0])
            snake_alive = 1;
    }
}
