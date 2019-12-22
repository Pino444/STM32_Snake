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
uint16_t apple_shape = 4;
uint16_t x1, x2, y1, y2;
short m;
short stone_cnt = 9;


void stateJudgement();

// 小蛇恰东西
void snake_eat(void) {            //判断是否吃到苹果
    if (abs(snake_x[0] - apple_x) < (snake_shape + apple_shape) &&
        abs(snake_y[0] - apple_y) < (snake_shape + apple_shape)) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        POINT_COLOR = WHITE;
        LCD_ShowxNum(200, 0, snake_length - 5, 3, 16, 0);
        snake_length++;
        LCD_Draw_Circle(apple_x, apple_y, apple_shape);
        LCD_Draw_Circle(apple_x, apple_y, apple_shape - 1);
        snake_x[snake_length] = snake_x[snake_length - 1];
        snake_y[snake_length] = snake_y[snake_length - 1];
        apple_x = rand() % (x2 - x1) + x1;
        apple_y = rand() % (y2 - y1) + y1;
        snake_speed -= 2;
        HAL_Delay(100);
        POINT_COLOR = BLACK;
        LCD_ShowxNum(200, 0, snake_length - 5, 3, 16, 0);
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
    stone_x = rand() % (x2 - x1) + x1;
    stone_y = rand() % (y2 - y1) + y1;

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
    // 小蛇能移动的边界
    x1 = snake_shape - 1;
    y1 = snake_shape + 14;
    x2 = lcddev.width - snake_shape + 1;
    y2 = lcddev.height - snake_shape + 1;
    POINT_COLOR = BLACK;
    LCD_DrawRectangle(x1, y1, x2, y2);
    LCD_ShowString(10, 0, 200, y1, 16, "STM32 Snake       Score:");
    LCD_ShowxNum(200, 0, snake_length - 5, 3, 16, 0);
    while (!snake_alive) {
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
                LCD_ShowString(lcddev.width / 2 - 40, lcddev.height / 2 - 32, 200, 16, 16, "Game Over!");
                LCD_ShowString(lcddev.width / 2 - 80, lcddev.height / 2 - 16, 200, 16, 16, "Press KEY_WK To Play");
                LCD_ShowString(lcddev.width / 2 - 20, lcddev.height / 2, 200, 16, 16, "Again!");
                HAL_Delay(100);
                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED0_Pin);
                if (HAL_GPIO_ReadPin(KEY_WK_GPIO_Port, KEY_WK_Pin) == GPIO_PIN_SET) {
                    memset(snake_x, 0, 100);
                    memset(snake_y, 0, 100);
                    snake_alive = 0;
                    snake_pos = RIGHT;
                    stone_cnt = 9;
                    snake_speed = speed;
                    snake_length = 5;
                    if (x % shape != 0) snake_x[0] = shape * 2;
                    else snake_x[0] = x;
                    if (y % shape != 0) snake_y[0] = shape * 2;
                    else snake_y[0] = y;
                    LCD_Clear(WHITE);
                    POINT_COLOR = BLACK;
                    LCD_DrawRectangle(x1, y1, x2, y2);
                }
            }
            if (stone_cnt == 25) refreshStone();
            // 画苹果
            POINT_COLOR = BRRED;
            LCD_Draw_Circle(apple_x, apple_y, apple_shape);
            LCD_Draw_Circle(apple_x, apple_y, apple_shape - 1);
            // 把尾巴变白
            POINT_COLOR = WHITE;
            LCD_Draw_Circle(snake_x[snake_length], snake_y[snake_length], snake_shape);
            POINT_COLOR = BLACK;

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
    if (snake_y[0] < snake_shape + y1 || snake_y[0] > y2 - snake_shape ||
        snake_x[0] < snake_shape + x1 || snake_x[0] > x2 - snake_shape) {
        snake_alive = 1;
        return;
    }

    for (m = snake_length; m > 0; m--) {
        if (snake_x[m] == snake_x[0] &&
            snake_y[m] == snake_y[0])
            snake_alive = 1;
    }
}
