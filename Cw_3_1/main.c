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

char LED_G_ON[]="LGON";
char LED_G_OFF[]="LGOFF";

char LED_B_ON[]="LBON";
char LED_B_OFF[]="LBOFF";

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
		temp=UART0->D;	// Read data s flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=LF)
			{
				if(!too_long)	// Timeout, ignore rest
				{
					rx_buf[rx_buf_pos] = temp;	// Take bytes
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// 
				}
			}
			else
			{
				if(!too_long)	// If too long, abandon
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
	UART0_Init();		// Init UART0
	while(1)
	{
		if(rx_FULL)		// Is data ready?
		{
			if(too_long)
			{
				for(i=0;Too_Long[i]!=0;i++)	// Too long
					{
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
						UART0->D = Too_Long[i];
					}
					while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is RX ready
					UART0->D = 0xa;		// Next line
					too_long=0;
			}
			else
			{
				if(strcmp (rx_buf,LED_R_ON)==0)	// Turn on red LED 
					PTB->PCOR = (1<<8);
				else
					if(strcmp (rx_buf,LED_R_OFF)==0)
						PTB->PSOR = (1<<8);					// Turn off red LED
					else
						if(strcmp(rx_buf, LED_G_ON)==0){
							PTB->PCOR = (1<<9);
						}
						else
							if(strcmp(rx_buf, LED_G_OFF)==0){
								PTB->PSOR = (1<<9);
							}
							else
								if(strcmp(rx_buf, LED_B_ON)==0){
									PTB->PCOR = (1<<10);
								}
								else
									if(strcmp(rx_buf, LED_B_OFF)==0){
										PTB->PSOR = (1<<10);
									}
									else
									{
										for(i=0;Error[i]!=0;i++)	// Zła komenda
										{
											while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
											UART0->D = Error[i];
										}
										while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
										UART0->D = 0xa;		// Next line
									}
				}
			rx_buf_pos=0;
			rx_FULL=0;	// Data taken
		}
		UART0->D = 2;
	}
}
