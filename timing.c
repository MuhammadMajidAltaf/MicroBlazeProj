/*
 *timing.c: simple starter application for lab 1A and 1B
 *
 */

#include <stdio.h>		// Used for printf()
#include "xparameters.h"	// Contains hardware addresses and bit masks
#include "xil_cache.h"		// Cache Drivers
#include "xintc.h"		// Interrupt Drivers
#include "xtmrctr.h"		// Timer Drivers
#include "xtmrctr_l.h" 		// Low-level timer drivers
#include "xil_printf.h" 	// Used for xil_printf()
#include "extra.h" 		// Provides a source of bus contention
#include "xgpio.h" 		// LED driver, used for General purpose I/i
#include "sevenSeg.h" 		// Custom peripheral driver for Seven Segment display


#define NUMBER_OF_TRIALS 1000
#define NUM_OF_ITERATIONS 1000
/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 * 
 */
#define LED_DELAY     1000000

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1

void print(char *str);

/*
 *  The following functions are used for testing the timing of the different Microblaze configurations
 */
void blinkLED(int numberOfBlinks);
void offLED();
void onLED();
void sevenSegment();
void printLongerStrings(); // Write this function
void printShortStrings(); // Write this function
void printfShortStrings(); // Write this function
void xil_printfShortStrings(); // Write this function
void intAddAndMultiply(); // Write this function
void floatAddAndMultiply(); // Write this function


/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XGpio Gpio; /* The Instance of the GPIO Driver used for LED 0 */

int main() {
	int numClockCycles = 0;
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	print("---Entering main---\n\r");
	print("---Trial Name \t Trial # \t Clock Cycles---\n\r");
	int i = 0;
	// Extra Method contains an interrupt routine which is set to go off at timed intervals
    extra_method();


    for( i=0; i < NUMBER_OF_TRIALS; i++) {
     	//TIMER RESET CODE
 		//Turn off the timer
 		XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
 		//Put a zero in the load register
 		XTmrCtr_SetLoadReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
 		//Copy the load register into the counter register
 		XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, XTC_CSR_LOAD_MASK);
 		//Enable (start) the timer
 		XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1,	XTC_CSR_ENABLE_TMR_MASK);
 		//END TIMER RESET CODE

 		//blinkLED(int numberOfBlinks);
		//offLED();
		//onLED();
		//sevenSegment();
		//printLongerStrings(); // Write this function
		//printShortStrings(); // Write this function
		//printfShortStrings(); // Write this function
		//xil_printfShortStrings(); // Write this function
		//intAddAndMultiply(); // Write this function
		//floatAddAndMultiply(); // Write this function


 		numClockCycles = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, 1);
 		xil_printf("print  \t%d\t%d\n", i,numClockCycles );

    }
    return 0;
}

void sevenSegment() {
    // The last column is for controlling the anode and cathodes of the seven segment displays
    // The Last seven digits 0b....7 control the lighted segments. 1 is off, 0 is on
    // SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, 0b1111111101111001); // Prints 1
    SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, 0b1111111101000000); // Prints 0
    //SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, 0xF731);
}

void onLED() {
    // The parameter XPAR_GPIO_1_DEVICE_ID is found in xparameters.h
    XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);
    XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0xff);
}
void offLED() {
    // Fill in this Function
    XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);
    XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x00);
}


void blinkLED(int numberOfBlinks) {
    int i;
    long j;

    XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);

    for (i = 0; i < numberOfBlinks; i++) {
        XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0xff);
        for (j = 0; j < LED_DELAY; j++) ; // NOP

        XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x00);
        for (j = 0; j < LED_DELAY; j++) ; // NOP
    }
}

void printLongerStrings() {
    //int length = 256;
    char *LongString = "Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! Hello, world! ";
    print(LongString);

}

void printShortStrings(){
    //int length = 64;

	// data dependence
    char *ShortString = "Hello, world!";
    //char *ShortString = "1234567890123";
	//char *ShortString = "Data dependen";
	//char *ShortString = "qwertyuiopasd";

	// lengths dependence
	//char *ShortString = "aaaaaa"
    //char *ShortString = "aaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    print(ShortString);

}

void printfShortStrings(){

	// data dependence
    char *ShortString = "Hello, world!";
    //char *ShortString = "1234567890123";
	//char *ShortString = "Data dependen";
	//char *ShortString = "qwertyuiopasd";

	// lengths dependence
	//char *ShortString = "aaaaaa"
    //char *ShortString = "aaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";


    printf("%s", ShortString);

}

void xil_printfShortStrings(){
	// data dependence
    char   *ShortString = "Hello, world!";
    //char *ShortString = "1234567890123";
	//char *ShortString = "Data dependen";
	//char *ShortString = "qwertyuiopasd";

	// lengths dependence
	//char *ShortString = "aaaaaa"
    //char *ShortString = "aaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    //char *ShortString = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	xil_printf("%s", ShortString);

}

void intAddAndMultiply(){
	int a = 1;
	int b = 15;
	int c;
	int i;

	for(i = 0;i < NUM_OF_ITERATIONS;i++){
		//Add
		c = a + b;
		//Mult
		//c = a * b;
	}

}

void floadAddAndMultiply(){
	float a = 3.1415926;
	float b = 2.71828;
	float c;
	int i;

	for(i = 0;i < NUM_OF_ITERATIONS;i++){
		//Add
		c = a + b;
		//Mult
		//c = a * b;
	}

}
