/*
 * CambiaEstado.c
 *
 * Created: 17/5/2024 17:54:43
 * Author : polst
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile int state = 0;

ISR(PCINT2_vect){
	if (!(PIND & (1<<PIND4)))
	{
		if (state < 2)
		{
			state++;
		} else {
			state = 0;
		}
	}
}


int main(void)
{
	cli();
	
	// Configura el boton como entrada
	DDRD &= ~(1<<DDD4);		//Pin PD4 como entrada
    PORTD |= (1<<PORTD4);	// Habilita Pull-Up
	
	//Configura los leds como salida
	DDRB |= (1 << DDB4); // Pin PB4 como salida
	//PORTB &= ~(1<<PORTB4);	// Inicia apagado
	DDRB |= (1<<DDB0);	// Pin PB0 como salida
	//PORTB &= ~(1<<PORTB0);	// Pin PB0 como salida
	DDRD |= (1<<DDD7);	// Pin PD7 como salida
	//PORTD &= (1<<PORTD7);
	
	// Configura la interrupcion PCINT2
	PCICR |= (1 << PCIE2);			    // Habilita la interrupcion PCINT2
	PCMSK2 |= (1 << PCINT20);          // Habilita PCINT20 (PD4)
	
	
	
	/*PORTB |= (1 << PORTB4); // Enciende el LED
	PORTD &= ~(1 << PORTB0); // Apaga el LED
	PORTD &= ~(1 << PORTD7); // Apaga el LED*/
	
	
	sei();
	
    while (1) 
    {
		if (state == 0)
		{
			PORTB |= (1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD &= ~(1<<PORTD7);
			
		} else if (state == 1)
		{
			PORTD &= ~(1<<PORTD7);
			PORTB &= ~(1<<PORTB4);
			PORTB |= (1<<PORTB0);
		
		} else if (state == 2)
		{
			PORTB &= ~(1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD |= (1<<PORTD7);
		
		} else if (state == 3)
		{
			PORTB &= ~(1<<PORTB4);
			PORTB &= ~(1<<PORTB0);
			PORTD &= ~(1<<PORTD7);
			state = 0;
		}
	}   
}



