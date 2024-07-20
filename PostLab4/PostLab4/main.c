//***********************************************************************
// Universidad del Valle de Guatemala
// IE2023 Programación de Microcontroladores
// PostLab4.c
// Autor: Larsson González
// Hardware: ATMega328P
// Proyecto: Manejo del ADC
// Created: 8/4/2024 18:54:26
//***********************************************************************

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char lista[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void ADC_init(void);
void dechex(uint8_t);

int adcread;
int adcreadL;
int adcreadH;
uint8_t contador;


int main(void)
{
    cli();
    ADC_init();
	
	DDRC = 0xFF;		// Puerto C como salida
	PORTC = 0;			// Puerto C inicializado apagado
	
	DDRB &= ~(1<<DDB0);	// PB0 como entrada
	PORTB |= (1<<PORTB0);	// Activamos Pull-Up
	
	DDRB &= ~(1<<DDB1);	// PB1 como entrada
	PORTB |= (1<<PORTB1);	// Activamos Pull-Up
	
	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5);	// Salidas del Puerto B
	
	DDRD = 0b11111111;	// Puerto D como salida
	
	// Configuracion de interrupcion PCINT
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1);
	PCICR |= (1<<PCIE0);	// Habilita Interrupcion PCINT en el puerto B
    sei();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);		// Iniciamos la conversion
		
		// Desplegar Display 1
		dechex(adcread);
		//adcreadL = adcread & 0x0F;
		PORTC = lista[adcreadL];
		switch (adcreadL)
		{
			case 0:
			PORTB &= ~(1<<PORTB5);
			break;
			case 1:
			PORTB &= ~(1<<PORTB5);
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
			PORTB &= ~(1<<PORTB5);
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
			PORTB &= ~(1<<PORTB5);
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
		PORTB |= (1<<PORTB3);		// Encendiendo display 2
		PORTB &= ~(1<<PORTB2);
		_delay_ms(4);
		PORTB &= ~(1<<PORTB3);		// Apaga display 2
		
		// Desplegamos Display 2
		dechex(adcread);
		//adcreadH = (adcread>>4) & 0x0F;
		PORTC = lista[adcreadH];
		switch (adcreadH)
		{
			case 0:
			PORTB &= ~(1<<PORTB5);
			break;
			case 1:
			PORTB &= ~(1<<PORTB5);
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
			PORTB &= ~(1<<PORTB5);
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
			PORTB &= ~(1<<PORTB5);
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
		
		PORTB |= (1<<PORTB2);		// Encendemos display 1
		PORTB &= ~(1<<PORTB3);		// Apaga display 2
		_delay_ms(4);
		PORTB &= ~(1<<PORTB2);		// Apagamos display 1
		
		
		if (adcread > contador){
			PORTB |= (1<<PORTB4);
		} else{
			PORTB &= ~(1<<PORTB4);
		}
    }
}

void ADC_init(){
	// Seleccion de Canal ADC (A)
	ADMUX = 0x07;
	
	// Utilizando AVCC = 5V internos
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion a la Izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	// Habilitando el ADC
	ADCSRA |= (1<<ADEN);
	
	//Habilitamos las interrupciones
	ADCSRA |= (1<<ADIE);
	
	// Habilitamos el Prescaler de 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	DIDR0 |= (1<<ADC0D);
}

void dechex(uint8_t val){
	adcreadH = val/16;		// Divido para bits mas significativos
	adcreadL = val%16;		// Residuo para bits menos significativos
}

ISR(ADC_vect){
	adcread = ADCH;		// Guardamos el valor
	ADCSRA |= (1<<ADIF);		// Apagamos bandera de interrupcion
}

ISR(PCINT0_vect){
	if (!(PINB & (1<<PINB0)))	// PB0 = 0?
	{
		if (contador < 255)		// Si el contador llega a 255 que se quede ahi
		{
			contador = contador + 1;	// Incrementa el contador en uno en uno
			PORTD = contador;	// Muestra el valor del contador
		}
	}
	
	if (!(PINB & (1<<PINB1)))	// PB1 = 0?
	{
		if (contador > 0)		// Si el contador llega a 0 que no se pase
		{
			contador = contador - 1;	// Decrementa el contador en uno en uno
			PORTD = contador;		// Muestra el valor del contador
		}
	}
}

