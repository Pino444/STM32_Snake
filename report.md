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

### 1. Draw snake 
- draw snake on LCD

```c
 POINT_COLOR = WHITE;
            LCD_Draw_Circle(snake_x[snake_length], snake_y[snake_length], snake_shape);
            LCD_Draw_Circle(snake_x[snake_length], snake_y[snake_length], snake_shape - 1);
            LCD_Draw_Circle(snake_x[snake_length], snake_y[snake_length], snake_shape - 3);

            POINT_COLOR = snake_color;
            for (m = snake_length; m >= 0; m--) {
                if (m > 0) {
                    snake_x[m] = snake_x[m - 1];
                    snake_y[m] = snake_y[m - 1];
                }
                LCD_Draw_Circle(snake_x[m], snake_y[m], snake_shape);
                LCD_Draw_Circle(snake_x[m], snake_y[m], snake_shape - 1);
                LCD_Draw_Circle(snake_x[m], snake_y[m], snake_shape - 3);
            }
```

### 2. Snake move
- move snake by change position


```c
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
```
### 3. Turn right/left
- turn right and turn left by KEY0 and KEY1
- key-vibration eliminate 


```c
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
```

### 4. Refresh stone and candy

- generate a stone randomly


```c
void refreshStone() {
    stone_cnt = 0;
    POINT_COLOR = WHITE;
    for (m = stone_num - 1; m >= 0; m--) {
        LCD_Draw_Circle(stone_x[m], stone_y[m], snake_shape);
    }
    POINT_COLOR = RED;
    for (m = stone_num - 1; m >= 0; m--) {
        stone_x[m] = rand() % (x2 - x1 - 2 * snake_shape) + x1 + snake_shape;
        stone_y[m] = rand() % (y2 - y1 - 2 * snake_shape) + y1 + snake_shape;
        LCD_Draw_Circle(stone_x[m], stone_y[m], snake_shape);
    }
}
```

- generate a apple randomly

```c

void refreshApple() {
    apple_cnt = 0;
    POINT_COLOR = WHITE;
    LCD_Draw_Circle(apple_x, apple_y, apple_shape);
    LCD_Draw_Circle(apple_x, apple_y, apple_shape - 1);
    apple_x = rand() % (x2 - x1 - 2 * apple_shape) + x1 + apple_shape;
    apple_y = rand() % (y2 - y1 - 2 * apple_shape) + y1 + apple_shape;
    POINT_COLOR = apple_color;
    LCD_Draw_Circle(apple_x, apple_y, apple_shape);
    LCD_Draw_Circle(apple_x, apple_y, apple_shape - 1);
}
```

#### result![屏幕快照 2020-01-01 下午5.41.38](media/15778523408429/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202020-01-01%20%E4%B8%8B%E5%8D%885.41.38.png)



### 5. Snake dead and grow
- eat bean and grow up, LED blink
- eat stone and dead, LED blink
- change color

```c
void snake_eat(void) {            //判断是否吃到苹果
    if (abs(snake_x[0] - apple_x) < (snake_shape + apple_shape) &&
        abs(snake_y[0] - apple_y) < (snake_shape + apple_shape)) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        LCD_ShowxNum(200, 0, snake_length - 1, 3, 16, 0);
        snake_length++;
        snake_color = apple_color;
        apple_color = color[rand() % 12];
        refreshApple();
//        snake_speed -= 2;
        // 总共有5关
        if (snake_length - 1 > 0 && (snake_length - 1) % 10 == 0 && apple_change > 20) {
            stone_num += 1;
            snake_speed -= 15;
            apple_change -= 5;
            refreshStone();
            POINT_COLOR = BLACK;
            LCD_ShowxNum(110, 0, (snake_length - 1) / 10, 2, 16, 0);
            LCD_ShowxNum(110, 0, (snake_length - 1) / 10 + 1, 2, 16, 0);
            POINT_COLOR = RED;
            LCD_ShowString(80, lcddev.height / 2 - 16, 200, y1, 16, "Level Up!");
            for (m = 20; m >= 0; m--) {
                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                HAL_Delay(50);
            }
            POINT_COLOR = WHITE;
            LCD_ShowString(80, lcddev.height / 2 - 16, 200, y1, 16, "Level Up!");
        }


        HAL_Delay(100);
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        POINT_COLOR = BLACK;
        LCD_ShowxNum(200, 0, snake_length - 1, 3, 16, 0);
    }
}
```

#### result
![屏幕快照 2020-01-01 下午5.41.19](media/15778523408429/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202020-01-01%20%E4%B8%8B%E5%8D%885.41.19.png)


## 6 Potential Improvement

Due to the time limit, some features still can be improved. The level setting we implement in this project just simply increase the number of obstacles instead of various mazes that are common in usual Snake Game. Besides, a special bonus system that some bonus item with different size can increase more socre and bring some abilities to the snake in a period of time.
