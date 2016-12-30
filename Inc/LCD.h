/**
  ******************************************************************************
  * File Name          : LCD.h
  * Description        : Header for driver code of 40x4 Character LCD LM40400 
                         (HD44780 equivalent). It consists of two controllers, one 
                         each for the upper and lower displays with each of them 
			 having 40x2 capacity. The two controllers share the 
			 pins and can be controlled by the separate enable pins 
			 povided.
  ******************************************************************************
  */

#ifndef __LCD_H
#define __LCD_H

/* BEGIN Includes */
#include "stm32f3xx_hal.h"

/* Macros for LCD */
#define RS_RESET				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
#define RS_SET					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
#define RW_WRITE_RESET				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
#define RW_WRITE_SET				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
#define EN1_ENABLE				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
#define EN1_DISABLE				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
#define EN2_ENABLE				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
#define EN2_DISABLE				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);								

typedef enum {CMD, DATA} type_t; //for the function to toggle enable pins
typedef enum {DISPLAY_1, DISPLAY_2} display_t; //to select upper or lower display

#define SET_LINE1(starting_column)		lcdcmd(DISPLAY_1, 0x80 + (starting_column - 1));
#define SET_LINE2(starting_column)		lcdcmd(DISPLAY_1, 0xC0 + (starting_column - 1));
#define SET_LINE3(starting_column)		lcdcmd(DISPLAY_2, 0x80 + (starting_column - 1));
#define SET_LINE4(starting_column)		lcdcmd(DISPLAY_2, 0xC0 + (starting_column - 1));
/* END Includes */

/* BEGIN Private function prototypes */
void lcd_init(void);
void delay(unsigned int time);
void toggle_EN1(type_t type);
void toggle_EN2(type_t type);
void lcd_write(unsigned char data);
void lcdcmd(display_t display, unsigned char value);
void lcddata(display_t display, unsigned char value);
void lcd_ports_clear(void);
void lcdint1(void);
void lcdint2(void);
ErrorStatus lcd_put_string(unsigned char line, unsigned char starting_column, unsigned char *S);
void clear_line(unsigned char line_no);
/* END Private function prototypes */

#endif
/**********************************END OF FILE***********************************/
