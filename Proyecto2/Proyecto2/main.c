/*****************************************************************
 * Universidad del Valle de Guatemala
 * IE2023 Programación de Microcontroladores
 * Proyecto2.c
 * Autor: Larsson González 
 * Proyecto: Arm Robotic
 * Hardware: ATMega328P
 * Created: 26/4/2024 17:10:25
 *****************************************************************/ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include "PWM/PWM0.h"
#include "PWM/PWM1.h"
#include "PWM/PWM2.h"

/*****************************************************************
* Declaración de Variables
******************************************************************/
float adcV1 = 0;
float adcV2 = 0;
float adcV3 = 0;
float adcV4 = 0;
float dutycycle = 0;
uint8_t duty1 = 0;
uint8_t duty2 = 0;
uint8_t modo = 0;
uint8_t estado = 0;


/*****************************************************************
* Declaración de Funciones
******************************************************************/
void initADC(void);	// CONFIGURACION DEL ADC
void initUART(void); // CONFIGURACION DE UART
uint16_t adcread(uint8_t canal);	// SELECCION DE CANAL DE ADC

/*****************************************************************
* Codigo Principal
******************************************************************/
int main(void)
{
	/*Salidas del PWM1 y PWM2*/
    //DDRD |= (1<<DDD3);		// OC2B
	//DDRB |= (1<<DDB1);		// OC1A
	//DDRB |= (1<<DDB2);		// OC1B
	//DDRB |= (1<<DDB3);		// OC2A
	//DDRD |= (1<<DDD6);		// OC0A
	//DDRD |= (1<<DDD5);		// OC0B
	
	/*Salidas de Leds*/
	DDRD |= (1<<DDD2);
	DDRD |= (1<<DDD4);
	
	/*Entradas de botones*/
	//DDRD &= ~(1<<DDD5);		// BOTON DE CAMBIO DE ESTADO - 1. MANUAL 2. EEPROM
	//DDRD &= ~(1<<DDD6);		// BOTON DE ESCRITURA EN EEPROM
	//DDRD &= ~(1<<DDD7);		// BOTON DE LECTURA EN EEPROM
	
	initADC();
	initUART();
	PWM0_init();
	initPWM1();
	initPWM2A(No_invertido, 1024);
	initPWM2B(No_invertido, 1024);
	
    while (1) 
    {
		
		
		adcV1  = adcread(0);
		writecanalA(adcV1);
		_delay_ms(10);
		
		adcV2 = adcread(1);
		writecanalB(adcV2);
		_delay_ms(10);
		
		duty1 = adcread(2);
		updateDutyCA2(duty1);
		_delay_ms(10);
		
		duty2 = adcread(3);
		dutycycle = map0(duty2, 0, 1024, 0, 180);
		PWM0_dca(dutycycle, no_invertido);
		
    }
}

/*****************************************************************
* Funciones
******************************************************************/
void initADC(void){
	ADMUX |= (1<<REFS0);	// VREF = AVCC
	ADMUX &= ~(1<<REFS1);	// VREF = AVCC
	ADMUX &= ~(1<<ADLAR);	// 10bits  
	
	// Prescaler 128 - 16M/128 = 125kHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	ADCSRA |= (1<<ADEN);	// ON ADC
	
	// DESABILITAR ENTRADAS DIGITALES DE PC0, PC1 Y PC2.
	DIDR0 |= (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D);	
}

uint16_t adcread(uint8_t canal){
	ADMUX = (ADMUX & 0xF0)|canal;	// SELECCIONA EL CANAL A USAR 
	ADCSRA |= (1<<ADSC);	// INICIA LA CONVERSION DEL ADC
	while ((ADCSRA)&(1<<ADSC));	// BUCLE DE QUE HASTA QUE FINALICE LA CONVERSION
	return(ADC);
}

void initUART(void){
	DDRD &= ~(1<<DDD0);		// RX como entrada
	DDRD |= (1<<DDD1);		// TX como salida
	
	// Configuration UCSR0A
	UCSR0A = 0;
	
	// Configuration UCSR0B when enable ISR Reception, enable RX and TX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	// Configuration UCSR0C - Mode asynchronous, Parity = None, One bit Stop, Data bits = 8bits
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	// Configuration velocity baudrate = 9600B
	UBRR0 = 103;
}

