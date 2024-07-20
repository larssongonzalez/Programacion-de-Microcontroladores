/*
 * PostLab5.c
 *
 * Created: 18/4/2024 08:20:22
 * Author : polst
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>


//************************************************************
// DECLARACION DE FUNCIONES
//************************************************************
void initADC(void);
void initT0(void);

//************************************************************
// DECLARACION DE VARIABLES
//************************************************************
uint8_t adcread1 = 0;
uint8_t adcread2 = 0;
uint8_t adcread3 = 0;
uint8_t canalADC = 0;
uint8_t contador = 0;

//************************************************************
// SETUP
//************************************************************
int main(void)
{
	cli();
	DDRB |= (1<<DDB1)|(1<<DDB3);
	DDRD |= (1<<DDD4);
	
	// Configuracion PWM Timer 1
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1<<COM1A1);
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<CS12)|(1<<CS10);
	
	// Configuracion PWM Timer 2
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	TCCR2B |= (1<<CS20)|(1<<CS21)|(1<<CS22);
	
	
    initADC();
	
	sei();
	//********************************************************
	// LOOP
	//********************************************************
    while (1){
		contador++;
		OCR1A = adcread1/6;
		OCR2A = adcread2/6;
		
		if (contador == 0){
			PORTD |= (1<<PORTD4);
		} else if (contador == (adcread3)){
			PORTD &= ~(1<<PORTD4);	
		}
    }
}

//************************************************************
// FUNCIONES
//************************************************************
void initADC(){
	// Seleccion de Canal ADC (A)
	ADMUX = 0;
	
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
	
	//DIDR0 |= (1<<ADC0D);
}

void initT0(){
	TCCR0B |= (1<<CS02)|(1<<CS00);
	TCNT0 = 100;
	TIMSK0 = (1<<TOIE0); 
}

//************************************************************
// ISR
//************************************************************
ISR(TIMER0_OVF_vect){
	if (canalADC==0){
		
		canalADC = 1;
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX |= (1<<MUX1)|(1<<MUX2);
		
		} else if (canalADC==1){
		
		canalADC = 2;
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		ADMUX |= (1<<MUX0)|(1<<MUX1)|(1<<MUX2);
		
		} else if (canalADC==2){
		
		canalADC = 0;
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
		
	}
	
	ADCSRA |= (1<<ADSC);
	TCNT0 = 100;
	TIFR0 |= (1<<TOV0);
}

ISR(ADC_vect){
	if (canalADC==0){
		adcread1 = ADCH;
	} else if (canalADC==1){
		adcread2 = ADCH;
	} else if (canalADC==2){
		adcread3 = ADCH;
	}
	ADCSRA |= (1 << ADIF);	//turn off flag
}

