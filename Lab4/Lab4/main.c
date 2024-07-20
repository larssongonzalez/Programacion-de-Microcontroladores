//**********************************************************************
// Universidad del Valle de Guatemala
// IE2023 Programacion de Microcontroladores
// Lab4.c
// Autor: Larsson Gonzalez
// Hardware: ATMega328p
// Proyecto:
// Created: 5/4/2024 15:12:14
//***********************************************************************
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t lista[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
				0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void ADC_init(void);

int adcread;
int adcreadL;
int adcreadH;

int main(void)
{
	cli();
	DDRC &= ~(1<<DDB0);	// A0 = ADC0 como entrada analogica
	DDRC |= (1<<DDB1)|(1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5)|(1<<DDB6);	// PC1 - PC6 como salida
	ADC_init();
	sei();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);		// Iniciamos la conversion
		_delay_ms(4);
		PORTB |= (1<<PORTB3);		// Encendiendo display 2
		adcreadL = adcread & 0x0F;
		PORTC = lista[adcreadL];
		switch (adcreadL)
		{
			case 0:
			PORTB |= (0<<PORTB5);
			break;
			case 1:
			PORTB |= (0<<PORTB5);
			break;
			case 2:
			PORTB |= (1<<PORTB5);
			break;
			case 3:
			PORTB |= (1<<PORTB5);
			break;
			case 4:
			PORTB |= (1<<PORTB5);
			break;
			case 5:
			PORTB |= (1<<PORTB5);
			break;
			case 6:
			PORTB |= (1<<PORTB5);
			break;
			case 7:
			PORTB |= (0<<PORTB5);
			break;
			case 8:
			PORTB |= (1<<PORTB5);
			break;
			case 9:
			PORTB |= (1<<PORTB5);
			break;
			case 10:
			PORTB |= (1<<PORTB5);
			break;
			case 11:
			PORTB |= (1<<PORTB5);
			break;
			case 12:
			PORTB |= (0<<PORTB5);
			break;
			case 13:
			PORTB |= (1<<PORTB5);
			break;
			case 14:
			PORTB |= (1<<PORTB5);
			break;
			case 15:
			PORTB |= (1<<PORTB5);
			break;
		}
		PORTB &= ~(1<<PORTB3);

		_delay_ms(4);
		PORTB |= (1<<PORTB2);		// Encendemos display 1
		adcreadH = (adcread>>4) & 0x0F;
		switch (adcreadH)
		{
			case 0:
			PORTB |= (0<<PORTB5);
			break;
			case 1:
			PORTB |= (0<<PORTB5);
			break;
			case 2:
			PORTB |= (1<<PORTB5);
			break;
			case 3:
			PORTB |= (1<<PORTB5);
			break;
			case 4:
			PORTB |= (1<<PORTB5);
			break;
			case 5:
			PORTB |= (1<<PORTB5);
			break;
			case 6:
			PORTB |= (1<<PORTB5);
			break;
			case 7:
			PORTB |= (0<<PORTB5);
			break;
			case 8:
			PORTB |= (1<<PORTB5);
			break;
			case 9:
			PORTB |= (1<<PORTB5);
			break;
			case 10:
			PORTB |= (1<<PORTB5);
			break;
			case 11:
			PORTB |= (1<<PORTB5);
			break;
			case 12:
			PORTB |= (0<<PORTB5);
			break;
			case 13:
			PORTB |= (1<<PORTB5);
			break;
			case 14:
			PORTB |= (1<<PORTB5);
			break;
			case 15:
			PORTB |= (1<<PORTB5);
			break;
		}

		PORTC = lista[adcreadH];
		PORTB &= ~(1<<PORTB2);
		
    }
}

void ADC_init(){
	// Seleccion de Canal ADC0
	ADMUX = 0;
	
	// Utilizando AVCC = 5V internos
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion a la Izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	ADCSRA |= (1<<ADEN);
	//Habilitamos las interrupciones
	ADCSRA |= (1<<ADIE);
	
	// Habilitamos el Prescaler de 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	// Habilitando el ADC
	ADCSRA |= (1<<ADEN);
}

ISR(ADC_vect){
	adcread = ADCH;		// Guardamos el valor 
	ADCSRA |= (1<<ADIF);		// Apagamos bandera de interrupcion
}