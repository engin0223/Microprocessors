/*-------------------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 6 - Ćwiczenie 3: sterowanie urządzeniami zewnętrznymi
					autor: Mariusz Sokołowski
					wersja: 16.11.2020r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "uart0.h"
#include "led.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LF	0xa		// Enter
char rx_buf[16];
char LED_R_ON[]="LRON";
char LED_R_OFF[]="LROFF";
char Error[]="Zla komenda";
char Too_Long[]="Zbyt dlugi ciag";
uint8_t rx_buf_pos=0;
char temp,buf;
uint8_t rx_FULL=0;
uint8_t too_long=0;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Odczyt wartości z bufora odbiornika i skasowanie flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=LF)
			{
				if(!too_long)	// Jeśli za długi ciąg, ignoruj resztę znaków
				{
					rx_buf[rx_buf_pos] = temp;	// Kompletuj komendę
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// Za długi ciąg
				}
			}
			else
			{
				if(!too_long)	// Jeśli za długi ciąg, porzuć tablicę
					rx_buf[rx_buf_pos] = 0;
				rx_FULL=1;
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}
int main (void)
{	
	uint8_t i;
	LED_Init();
	UART0_Init();		// Inicjalizacja portu szeregowego UART0
	while(1)
	{
		if(rx_FULL)		// Czy dana gotowa?
		{
			if(too_long)
			{
				for(i=0;Too_Long[i]!=0;i++)	// Zbyt długi ciąg
					{
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
						UART0->D = Too_Long[i];
					}
					while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
					UART0->D = 0xa;		// Następna linia
					too_long=0;
			}
			else
			{
				if(strcmp (rx_buf,LED_R_ON)==0){	// Zaświeć czerwoną diodę LED 
					PTB->PCOR = (1<<8);
					LED_Ctrl(LED_RED, LED_ON);
				}
				else
					if(strcmp (rx_buf,LED_R_OFF)==0)
						PTB->PSOR = (1<<8);					// Zgaś czerwoną diodę LED
					else
					{
						for(i=0;Error[i]!=0;i++)	// Zła komenda
						{
							while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
							UART0->D = Error[i];
						}
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Czy nadajnik gotowy?
						UART0->D = 0xa;		// Następna linia
					}
				}
			rx_buf_pos=0;
			rx_FULL=0;	// Dana skonsumowana
		}
	}
}
