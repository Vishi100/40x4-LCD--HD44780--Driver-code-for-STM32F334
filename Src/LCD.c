/**
  ******************************************************************************
  * File Name          : LCD.c
  * Description        : Driver code for 40x4 Character LCD LM40400 (HD44780 
			 equivalent). It consists of two controllers, one each 
			 for the upper and lower displays with each of them 
			 having 40x2 capacity. The two controllers share the 
			 ins and can be controlled by the separate enable pins 
			 povided.
  ******************************************************************************
  */

/* BEGIN Includes */
#include "stm32f3xx_hal.h"
#include "LCD.h"
/* END Includes */

/**
  * @brief  A simple delay function using for loops.
  * @param  time.
  * @retval None
  */
void delay(unsigned int time)  //Time delay function
{
	unsigned int i,j;
	for(i=0;i<time;i++)
		for(j=0;j<5;j++);
}

/**
  * @brief  	Function to toggle enable pin of upper controller. This should be
		called from lcdcmd or lcddata functions.
  * @param 	 type parameter (type_t enum variable) can be any one among the two - 
		CMD or DATA.
  * @retval 	None
  */
void toggle_EN1(type_t type)
{
	uint16_t toggle_period;
	
	if(type == CMD)
		toggle_period = 50;
	else if(type == DATA)
		toggle_period = 500;
	
	EN1_ENABLE             		
	delay(toggle_period);
	EN1_DISABLE
	delay(50);
}

/**
  * @brief 	 Function to toggle enable pin of lower controller. This should be
		called from lcdcmd or lcddata functions.
  * @param  	type parameter (type_t enum variable) can be any one among the two - 
		CMD or DATA.
  * @retval 	None
  */
void toggle_EN2(type_t type)
{
	uint16_t toggle_period;
	
	if(type == CMD)
		toggle_period = 50;
	else
		toggle_period = 500;
	
	EN2_ENABLE             		
	delay(toggle_period);
	EN2_DISABLE
	delay(50);
}

/**
  * @brief  	Function to send data bits (8-bit mode) to the respective pins. This 
		should be called from lcdcmd or lcddata functions.
  * @param  	data - 8 bit data.
  * @retval 	None
  */
void lcd_write(uint8_t data)
{
	/* set the data bits */
	if(data & 0x01) 
	{
		HAL_GPIO_WritePin(DB0_GPIO_Port, DB0_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB0_GPIO_Port, DB0_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x02) 
	{
		HAL_GPIO_WritePin(DB1_GPIO_Port, DB1_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB1_GPIO_Port, DB1_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x04) 
	{
		HAL_GPIO_WritePin(DB2_GPIO_Port, DB2_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB2_GPIO_Port, DB2_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x08) 
	{
		HAL_GPIO_WritePin(DB3_GPIO_Port, DB3_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB3_GPIO_Port, DB3_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x10) 
	{
		HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x20) 
	{
		HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x40) 
	{
		HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin,GPIO_PIN_RESET);
	}
	if(data & 0x80) 
	{
		HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin,GPIO_PIN_SET);
	} 
	else 
	{
		HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin,GPIO_PIN_RESET);
	}
}

/**
  * @brief  	Function for sending values to the command register of LCD. This 
		can be called by the user to give instructions to the LCD.
  * @param  	display - display_t variable to indicate which controller should 
		receive the command. It can be any one among the two - 
		DISPLAY_1 or DISPLAY_2.
	* @param  value - 8 bit command.
  * @retval 	None
  */
void lcdcmd(display_t display, uint8_t value)
{
	lcd_write(value);
	RS_RESET
	RW_WRITE_RESET
	
	if(display == DISPLAY_1)
		toggle_EN1(CMD);
	else
		toggle_EN2(CMD);
}

/**
  * @brief  	Function for sending values to the data register of LCD. This 
		can be called by the user to display characters on the LCD.
  * @param 	display - display_t variable to indicate which controller should 
		receive the command. It can be any one among the two - 
		DISPLAY_1 or DISPLAY_2.
* @param  	value - 8 bit data.
  * @retval 	None
  */
void lcddata(display_t display, uint8_t value)
{
	lcd_write(value);
	RS_SET
	RW_WRITE_RESET
	
	if(display == DISPLAY_1)
		toggle_EN1(DATA);
	else
		toggle_EN2(DATA);
}

/**
  * @brief  	Function to reset the pins used for the LCD. Control pins have
		been taken on Port B and data pins on Port C.
  * @param  	None.
  * @retval	None
  */
void lcd_ports_clear(void)
{
	HAL_GPIO_WritePin(GPIOB, R_W_Pin|RS_Pin|E1_Pin|E2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, DB0_Pin|DB1_Pin|DB2_Pin|DB3_Pin 
                          |DB4_Pin|DB5_Pin|DB6_Pin|DB7_Pin, GPIO_PIN_RESET);
}

/**
  * @brief 	Function to initialize the registers and pins of LCD (upper display).
		This shouldn't be called by the user as both the displays need to be
		initialized and a separate function has been provided for 
		initialization of the LCD.
  * @param  None.
  * @retval None
  */
void lcdint1(void)         
{
	lcd_ports_clear();	
	delay(15000);
	lcddata(DISPLAY_1, 0x30);
	delay(4500);
	lcddata(DISPLAY_1, 0x30);
	delay(300);
	lcddata(DISPLAY_1, 0x30);
	delay(650);
	lcdcmd(DISPLAY_1, 0x38);
	delay(50);
	lcdcmd(DISPLAY_1, 0x0C);
	delay(50);
	lcdcmd(DISPLAY_1, 0x01);
	delay(50);
	lcdcmd(DISPLAY_1, 0x06);
	delay(50);
}

/**
  * @brief 	Function to initialize the registers and pins of LCD (lower display).
		This shouldn't be called by the user as both the displays need to be
		initialized and a separate function has been provided for 
		initialization of the LCD.
  * @param  	None.
  * @retval 	None
  */
void lcdint2(void)         
{
	lcd_ports_clear();	
	delay(15000);
	lcddata(DISPLAY_2, 0x30);
	delay(4500);
	lcddata(DISPLAY_2, 0x30);
	delay(300);
	lcddata(DISPLAY_2, 0x30);
	delay(650);
	lcdcmd(DISPLAY_2, 0x38);
	delay(50);
	lcdcmd(DISPLAY_2, 0x0C);
	delay(50);
	lcdcmd(DISPLAY_2, 0x01);
	delay(50);
	lcdcmd(DISPLAY_2, 0x06);
	delay(50);
}

/**
  * @brief  This is the main initialization function for the LCD and it can be 
						called by the user.
  * @param  None.
  * @retval None
  */
void lcd_init(void)
{
	lcdint1();
	lcdint2();
}

/**
  * @brief  	Function to display string on LCD. Can be called by the user.
  * @param  	line_no - Specifies where the string should be displayed. Takes values 
		from 1 to 4 for 40x4 LCD.
* @param   	sarting_column - Specifies where the string should be displayed. Takes 
		values from 1 to 40 for 40x4 LCD.
  * @param  	S pointer to the unsigned char string to be displayed.
  * @retval 	ERROR if line_no or starting_column is out of range.
		SUCCESS if the string is displayed.
  */
ErrorStatus lcd_put_string(uint8_t line_no, uint8_t starting_column, uint8_t *S)
{
	display_t display;
	
	if(!(starting_column >= 1) && !(starting_column <= 40))
	{
		return ERROR;
	}
	
	if(line_no == 1)
	{
		display = DISPLAY_1;
		SET_LINE1(starting_column)		
	}
	else if(line_no == 2)
	{
		display = DISPLAY_1;
		SET_LINE2(starting_column)
	}
	else if(line_no == 3)
	{
		display = DISPLAY_2;
		SET_LINE3(starting_column)	
	}
	else if(line_no == 4)
	{
		display = DISPLAY_2;
		SET_LINE4(starting_column)
	}
	else
	{
		return ERROR;
	}
	
	while(*S)
		lcddata(display, *S++);
	
	return SUCCESS;
}

/**
  * @brief  	Function to clear a single line on LCD. Can be called by the user.
  * @param  	line_no - Specifies which line has to be cleared. Takes values 
		from 1 to 4 for 40x4 LCD.
  * @retval 	None.
  */
void clear_line(uint8_t line_no)
{
	lcd_put_string(line_no, 1, (uint8_t *)"                                        ");
}
