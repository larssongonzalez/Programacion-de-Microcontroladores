/*
 * Ejemplo_PCINT.c
 *
 * Created: 8/4/2024 10:50:41
 * Author : polst
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t contador;


int main(void)
{
	cli();
	
    DDRB &= ~(1<<DDB0);	// PB0 como entrada
	PORTB |= (1<<PORTB0);	// Activamos Pull-Up
	
	DDRB &= ~(1<<DDB1);	// PB1 como entrada
	PORTB |= (1<<PORTB1);	// Activamos Pull-Up
	
	DDRD = 0b11111111;	// Puerto D como salida
	
	// Configuracion de interrupcion PCINT
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1);
	PCICR |= (1<<PCIE0);	// Habilita Interrupcion PCINT en el puerto B
		
	sei();
    while (1) 
    {
		
    }
}



ISR(PCINT0_vect){
	if (!(PINB & (1<<PINB0)))
	{
		if (contador < 255)
		{
			contador = contador + 1;
			PORTD = contador;
		}
	}
	
	if (!(PINB & (1<<PINB1)))
	{
		if (contador > 0)
		{
			contador = contador - 1;
			PORTD = contador;
		}
	}
}