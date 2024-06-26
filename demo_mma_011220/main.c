/******************************************************************************
 * This file is a part of the MMA Demo (C).                                   *
 ******************************************************************************/

/**
 * @file main.c
 * @author Koryciak
 * @date Nov 2020 
 * @brief File containing the main function. 
 * @ver 0.7
 */
 
#include "frdm_bsp.h" 
#include "led.h" 
#include "uart.h" 
#include "stdio.h"
#include "i2c.h"

/******************************************************************************\
* Private prototypes
\******************************************************************************/
void SysTick_Handler(void);
void bus_scan (void);
void expander_looptest (uint8_t address);
/******************************************************************************\
* Private memory declarations
\******************************************************************************/
static uint8_t msTicks = 0;
static uint8_t newTick = 0;
static char temp[99];
static uint8_t readout = 0;
static uint8_t arrayXYZ[6];

/**
 * @brief The main loop. 
 * 
 * @return NULL 
 */
int main (void) { 
	
	SysTick_Config(1000000); 								/* initialize system timer */
	
	LED_Init ();	 													/* initialize all LEDs */ 
	LED_Welcome();  												/* blink with all LEDs */
	
	UART_Init(9600);												/* initialize UART */ 
	UART_Println("\n\r### Microprocessor Technology 2 - I2C tutorial ###");

/******************************************************************************\
* EXERCISE 1. Ping all connected devices.
\******************************************************************************/
	
	// ToDo 1.7 Uncomment next 3 lines, build and run.
	//					Fill gaps in I2C initialization in file i2c.c
	//			    Write down MMA8451 (accelerometer) address.

	I2C_Init();															/* initialize I2C */
	
	UART_Println("\n-> List of connected to I2C bus devices:");
	bus_scan();															/* check what is connected */
	// address of accelerometer 0x1D
	// address of I/O expander used by LCD module 0x27
	
	// ToDo 1.9 Connect I/O expander (connect to 3.3V this time) and rerun exercise.
	//			    Write down expander address.
	
/******************************************************************************\
* EXERCISE 2. Loop test with I/O expander.
\******************************************************************************/

	// ToDo 2.1 Uncomment next 2 lines, build and run.
	//			    Complete address of expander.
	
	UART_Println("\n-> Loopback test with PCF8574:");
	expander_looptest(0x27);
	//I2C_Write(0x27, 4);
	
	// ToDo 2.4 Turn on LCD1602 backlight - 4th bit on I/O expander.
	
/******************************************************************************\
* EXERCISE 3. WHO AM I? Read MMA8451 device ID. 
\******************************************************************************/

	// ToDo 3.1 Uncomment next 4 lines, build and run
	//			    Complete address of accelerometer. Find in which register is dev ID.

	UART_Println("\n-> First contact with MMA8451.");
	
	I2C_ReadReg (0x1D, 0x0D, &readout);
	
	sprintf(temp, "MMA8451 device ID: 0x%02X", readout);
	UART_Println(temp);
	// value is 0x1A

/******************************************************************************\
* EXERCISE 4. Gravity! MMA8451 activate and first measurement. 
\******************************************************************************/

	// ToDo 4.1 Uncomment next 5 lines, build and run
	//			 		Complete address of accelerometer.
	//			 		Find register address to control (CTRL_REG1) and ACTIVE bit position. 	
	//			 		Find register address of Z-axis (OUT_Z_MSB).
	//			 		Increase Stack_Size to 0x00000300 in startup_MKL05Z4.s

	UART_Println("\n-> MMA8451 first measurement.");
	
	I2C_WriteReg(0x1D, 0x2A, 1);	 		// Set ACTIVE bit in CTRL_REG1 in MMA8451

	I2C_ReadReg (0x1D, 0x05, &readout); // Get OUT_Z_MSB from MMA8451 (MSB 8 bits from 14 in total)
	
	sprintf(temp,"Gravity: %+1.2fg", ((double)((int16_t)(readout<<8)>>2)/4096)); // default 4096 counts/g sensitivity
	UART_Println(temp);
	
/******************************************************************************\
* EXERCISE 5 - Get XYZ from MMA8451. 
\******************************************************************************/	

	// ToDo 5.1 Uncomment next 8 lines, build and run
	//			 		Complete address of accelerometer.	
	//			 		Find register address of X-axis (OUT_X_MSB).
	//					How many registers do we need to read?

	UART_Println("\n-> MMA8451 full data.");

	I2C_ReadRegBlock(0x1D, 0x01, 8, arrayXYZ);

	sprintf(temp,"X: %+1.6fg", ((double)((int16_t)((arrayXYZ[0]<<8)|arrayXYZ[1])>>2)/4096)); // default 4096 counts/g sensitivity
	UART_Println(temp);
	DELAY(2000);

	sprintf(temp,"Y: %+1.6fg", ((double)((int16_t)((arrayXYZ[2]<<8)|arrayXYZ[3])>>2)/4096)); // default 4096 counts/g sensitivity
	UART_Println(temp);
	DELAY(2000);

	sprintf(temp,"Z: %+1.6fg", ((double)((int16_t)((arrayXYZ[4]<<8)|arrayXYZ[5])>>2)/4096)); // default 4096 counts/g sensitivity
	UART_Println(temp);
	DELAY(2000);

/******************************************************************************\
* main loop
\******************************************************************************/	

	UART_Println("\n-> Starting the main loop. Sleep ...\n\r");

	while(1) {

		__WFI();															/* sleep & wait for interrupt */

		// simple schedule
		if (newTick) {
			newTick = 0;												/* clear flag & choose task */
			// task 1 - heartbeat
			if( msTicks%20 == 0 ) {
				LED_Ctrl(HEART_BEAT, LED_TOGGLE);
			}
			// task 2
			if( msTicks%50 == 0) {
				// Your code here ... ToDo 6.1 Continuous readout from accelerometer 
			}
		}
	} /* end_while */
}
/**
 * @brief System time update. 
 */
void SysTick_Handler(void) {
	msTicks++;
	newTick = 1;
}
/**
 * @brief Simple bus (I2C) scan. Print via UART present devices. 
 */
void bus_scan (void) {
	
	uint8_t idx = 1;
	
	for(uint8_t address=1; address<128; address++) {
		if( (I2C_Ping(address) & I2C_ERR_NOACK) == 0 ) { // Send address and check ACK from slave
			sprintf(temp, "%d. Device address: 0x%02X", idx++, address);
			UART_Println(temp);
		}
	}
}
/**
 * @brief Loop test with Read/Write operations of I/O expander. 
 */
void expander_looptest (uint8_t address) {
	
	uint8_t idx = 1;
	
	for(uint8_t data=2; data<7; data++) {
		I2C_Write(address, data);													// Write data to address
		I2C_Read (address, &readout);											// Read address to readout
		sprintf(temp, "Test %02d - Write: 0x%02X Read: 0x%02X", idx++, data,readout);
		UART_Println(temp);
	}	
}
