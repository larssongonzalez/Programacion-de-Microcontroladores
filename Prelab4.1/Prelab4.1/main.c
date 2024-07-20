//*****************************************************************
// Universidad del Valle de Guatemala
// IE2023 Programacion de Microcontroladores
// Prelab4.1.c
// Autor: Larsson Gonzalez
// Hardware: ATMega328p
// Proyecto: Contador de 8bits.
//*****************************************************************

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    uint8_t contador;
    
    // Boton de Incremento
    DDRB &= ~(1<<DDB1);		// PB1 como entrada
    PORTB |= (1<<PORTB1);	// Activamos Pull-Up
    
    // Boton de Decremento
    DDRB &= ~(1<<DDB2);		// PB2 como entrada
    PORTB |= (1<<PORTB2);	// Activamos Pull-Up
    
    DDRD = 0xFF;				// Puerto D como salida
    
    contador = 0;
	
    while (1) 
    {
		if (!(PINB & (1<<PINB1)))
		{
			if (contador < 255)
			{
				contador = contador + 1;
				PORTD = contador;	
			}
			_delay_ms(250);
		}
		if (!(PINB & (1<<PINB2)))
		{
			if (contador > 0)
			{
				contador = contador -1;
				PORTD = contador;
			}
			_delay_ms(250);
		}
    }
}

