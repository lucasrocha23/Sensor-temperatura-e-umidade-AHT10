# include "lcd.h"

void LCD::LCD_Commandgiver( unsigned char comand )
{
	LCDER_Port = (LCDER_Port & 0x0F) | (comand & 0xF0); 
	LCDER_Port &= ~ (1<<RSel);		
	LCDER_Port |= (1<<ENable);		
	_delay_us(2);
	LCDER_Port &= ~ (1<<ENable);

	_delay_us(200);

	LCDER_Port = (LCDER_Port & 0x0F) | (comand << 4);  
	LCDER_Port |= (1<<ENable);
	_delay_us(2);
	LCDER_Port &= ~ (1<<ENable);
	_delay_ms(2);
}




void LCD::LCD_Initializer (void)			{
	LCD_Dir_00 = 0xFF;			
	_delay_ms(20);			
	
	LCD_Commandgiver(0x02);		
	LCD_Commandgiver(0x28);            
	LCD_Commandgiver(0x0c);              
	LCD_Commandgiver(0x06);              
	LCD_Commandgiver(0x01);              
	_delay_ms(2);
}

void LCD::LCD_Character( unsigned char data_00 )
{
	LCDER_Port = (LCDER_Port & 0x0F) | (data_00 & 0xF0);
	LCDER_Port |= (1<<RSel);
	LCDER_Port|= (1<<ENable);
	_delay_us(1);
	LCDER_Port &= ~ (1<<ENable);

	_delay_us(200);

	LCDER_Port = (LCDER_Port & 0x0F) | (data_00 << 4);
	LCDER_Port |= (1<<ENable);
	_delay_us(1);
	LCDER_Port &= ~ (1<<ENable);
	_delay_ms(2);
}


void LCD::LCD_String (char *string_00)		
{
	int ci;
	for(ci=0;string_00[ci]!=0;ci++)		
	{
		LCD_Character (string_00[ci]);
	}
}




void LCD::LCD_Clear()
{
	LCD_Commandgiver (0x01);
	_delay_ms(2);
	LCD_Commandgiver (0x80);
}


void LCD::LCD_String_xy (char row_1, char position, char *string_00)	
{
	if (row_1 == 0 && position<16)
	LCD_Commandgiver((position & 0x0F)|0x80);	
	else if (row_1 == 1 && position<16)
	LCD_Commandgiver((position & 0x0F)|0xC0);	
	LCD_String(string_00);		
}
