#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include <Arduino.h>
#include "AHT10.h"

AHT10 sensor(0x39);


int main()
{    
    sensor.begin();
    
    while (1)
    {
        sensor.exibeTempUmidade();
        _delay_ms(2000);
    }
    
}