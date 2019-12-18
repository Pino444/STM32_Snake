//#include <cstdlib>
#include "stm32f1xx.h"
#include "lcd.h"

#define LCD_MIN_X 1
#define LCD_MAX_X 400
#define LCD_MIN_Y 1
#define LCD_MAX_Y 300


uint8_t s_lenght = 0;
uint8_t x_candy;
uint8_t y_candy;

typedef struct {
	uint8_t x, y;
} t_point;

t_point tab[100];

void draw_snake() {				// funkcja odrysowujaca weza z tablicy elementow
	LCD_Clear(BLUE);
	for (uint8_t i = 0; i < s_lenght; i++) {
//		lcd_draw_pixel(tab[i].x - 1, tab[i].y);
//		lcd_draw_pixel(tab[i].x + 1, tab[i].y);
//		lcd_draw_pixel(tab[i].x, tab[i].y - 1);
//		lcd_draw_pixel(tab[i].x, tab[i].y + 1);
		LCD_DrawRectangle(tab[i].x-5,tab[i].y-5,tab[i].x+5,tab[i].y+5);
		//lcd_copy();
	}
//	lcd_draw_pixel(x_candy  ,y_candy  );
//	lcd_draw_pixel(x_candy - 1,y_candy);
//	lcd_draw_pixel(x_candy + 1,y_candy);
//	lcd_draw_pixel(x_candy,y_candy - 1);
//	lcd_draw_pixel(x_candy,y_candy + 1);
	LCD_DrawRectangle(x_candy-8,y_candy-8,x_candy+8,y_candy+8);
//	lcd_copy();
}

void snake_update(uint8_t x, uint8_t y) { 		// funkcja zmienia wartosci w tablicy, zmienia wsp jego polozenia.

	for (uint8_t i = s_lenght - 1; i > 0; i--) {
		tab[i].x = tab[i - 1].x;
		tab[i].y = tab[i - 1].y;
	}

	tab[0].x = tab[0].x + x;
	tab[0].y = tab[0].y + y;

	if (tab[0].x <= LCD_MIN_X)
		tab[0].x = LCD_MAX_X;
	else if (tab[0].x >= LCD_MAX_X)
		tab[0].x = LCD_MIN_X;
	else if (tab[0].y <= LCD_MIN_Y)
		tab[0].y = LCD_MAX_Y;
	else if (tab[0].y >= LCD_MAX_Y)
		tab[0].y = LCD_MIN_Y;
	draw_snake();
}

void snake_start(){
	uint8_t z = 0;
//	lcd_setup();
	for (uint8_t i = 0; i < 8; i++) {
		tab[i].x = 60 - z;
		tab[i].y = 20;
		z = z + 5;
		s_lenght++;
	}
	draw_snake();
	HAL_Delay(1500);
}

void candy(){
	x_candy = rand()%LCD_MAX_X;
	y_candy = rand()%LCD_MAX_Y;

}

