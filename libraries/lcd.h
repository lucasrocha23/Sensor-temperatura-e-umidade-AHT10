#ifndef LCD_h
#define LCD_h

#include <avr/io.h>			
#include <util/delay.h>		

#define LCD_Dir_00  DDRD			
#define LCDER_Port PORTD			
#define RSel PD2				
#define ENable PD3 	

class LCD
{
    public:
        void LCD_Commandgiver( unsigned char comand );
        void LCD_Initializer (void);
        void LCD_Character( unsigned char data_00 );
        void LCD_String (char *string_00);
        void LCD_Clear();
        void LCD_String_xy (char row_1, char position, char *string_00);
};
#endif