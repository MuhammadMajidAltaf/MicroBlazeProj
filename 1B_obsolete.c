/****************By Dongqiao Ma&Zhiang Li************************/
#include "all_method.h"


XGpio Btn;
XIntc intCtrl_Btn;
XGpio Led;
XIntc intCtrl_Timer;
XTmrCtr Timer_tmrctr;

unsigned int count=0;


int btn_initialize(){
	 XStatus Status= XST_SUCCESS;
	 Status = XGpio_Initialize(&Btn, BTN_DEVICE_ID);
	 if (Status != XST_SUCCESS)
	 {
		 if( Status == XST_DEVICE_NOT_FOUND )
		 {
			xil_printf("GPIO_Btn: XST_DEVICE_NOT_FOUND...\r\n");
		 }
		 else
		 {
			xil_printf("GPIO_Btn: A different error from XST_DEVICE_NOT_FOUND...\r\n");
		 }
		 xil_printf("GPIO_Btn: driver failed to be initialized...\r\n");
		 return XST_FAILURE;
	  }
	 print("GPIO_Btn: driver Initialized!\n\r ");
	 return XST_SUCCESS;
}

int btn_read(){
	return XGpio_DiscreteRead(&Btn, BUTTON_CHANNEL);
}

void btn_handler(void *pshButton)
{
  XGpio* PushButton = (XGpio*) pshButton;
//  u32 btnState = XGpio_DiscreteRead(PushButton, 1);

	counter_initialize();
  count=0;
  onLED(count);
  XGpio_InterruptClear(PushButton, 0xff);
}



int btn_method(){

  XStatus Status= XST_SUCCESS;

  if(btn_initialize()!=XST_SUCCESS)
	  return XST_FAILURE;

  Status = XIntc_Initialize(&intCtrl_Btn, INTC_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		if( Status == XST_DEVICE_NOT_FOUND )
		{
			xil_printf("Interrupt Controller: XST_DEVICE_NOT_FOUND...\r\n");
		}
		else
		{
			xil_printf("Interrupt Controller: A different error from XST_DEVICE_NOT_FOUND...\r\n");
		}
		xil_printf("Interrupt controller: driver failed to be initialized...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Interrupt controller: driver initialized!\r\n");

  Status =  XIntc_Connect(&intCtrl_Btn, BTN_IR_ID, btn_handler, &Btn);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected to Interrupt Controller!\r\n");

  XIntc_Enable(&intCtrl_Btn, BTN_IR_ID);

  microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,INTC_DEVICE_ID);//Because it is 0?

  microblaze_enable_interrupts();

  XGpio_InterruptEnable(&Btn,BTN_IR_MASK);

  XGpio_InterruptGlobalEnable(&Btn);


  Status = XIntc_Start(&intCtrl_Btn, XIN_REAL_MODE);
  if (Status != XST_SUCCESS) {
		xil_printf("Failed to start the interrupt controller...\r\n");
		return XST_FAILURE;
  }
  print("Interrupt controller Started!\n\r ");

 return XST_SUCCESS;
}

/****************************************************************/
void timer_handler()
{
	Xuint32 ControlStatusReg;

	ControlStatusReg = XTimerCtr_ReadReg(Timer_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET);


	count++;
	onLED(count);
	XTmrCtr_WriteReg(Timer_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET, ControlStatusReg |XTC_CSR_INT_OCCURED_MASK);
}

void timer_disable()
{
	XIntc_Disable(&intCtrl_Timer, TIMER_IR_ID);
}

void timer_enable()
{
	XIntc_Enable(&intCtrl_Timer, TIMER_IR_ID);
}

int timer_method()
{

	XStatus Status;

	Status = XST_SUCCESS;

	Status = XIntc_Initialize(&intCtrl_Timer, TIMER_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		if( Status == XST_DEVICE_NOT_FOUND )
		{
			xil_printf("Interrupt Controller: XST_DEVICE_NOT_FOUND...\r\n");
		}
		else
		{
			xil_printf("Interrupt Controller: A different error from XST_DEVICE_NOT_FOUND...\r\n");
		}
		xil_printf("Interrupt controller: driver failed to be initialized...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Interrupt controller: driver initialized!\r\n");

	Status = XIntc_Connect(&intCtrl_Timer,TIMER_IR_ID,(XInterruptHandler)timer_handler, &Timer_tmrctr);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected to Interrupt Controller!\r\n");


	Status = XIntc_Start(&intCtrl_Timer, XIN_REAL_MODE);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to start Interrupt Controller: ...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Interrupt Controller Started!\r\n");

	XIntc_Enable(&intCtrl_Timer, TIMER_IR_ID );

	Status = XTmrCtr_Initialize(&Timer_tmrctr, INTC_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Timer initialization failed...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Timer Initialized !\r\n");
	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&Timer_tmrctr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(&Timer_tmrctr, 0, 0xFFFFFFFF-RESET_VALUE);		// 0x17D7840 = 25*10^6 clk cycles @ 50MHz = 500ms
	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */
	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,(void*)TIMER_DEVICE_ID);
	microblaze_enable_interrupts();
	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&Timer_tmrctr, 0);
	xil_printf("Timer Started!\r\n");
	return XST_SUCCESS;
}


/**************************** Counter *******************************************/
void counter_initialize(){
 	//TIMER RESET CODE
	//Turn off the timer
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
	//Put a zero in the load register
	XTmrCtr_SetLoadReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
	//Copy the load resister into the counter register
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, XTC_CSR_LOAD_MASK);
	//Enable (start) the timer
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1,	XTC_CSR_ENABLE_TMR_MASK);
	//END TIMER RESET CODE
}

int counter_method(){
	return XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, 1);
}



/************************Seven Segment************************************************/
void sevenSegment() {
	// The last column is for controlling the anode and cathodes of the seven segment displays
	// The Last seven digits 0b....7 control the lighted segments. 1 is off, 0 is on
	// SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, 0b1111111101111001); // Prints 1
	SEVENSEG_mWriteReg(SEVEN_BASEADDR, 0, 0b1111111101000000); // Prints 0
	//SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, 0xF731);
}


/************  L  ********************  E  ********************  D  *******************/

int led_initialize(){
	XStatus Status = XST_SUCCESS;
	Status =  XGpio_Initialize(&Led, LED_DEVICE_ID);
	if ( Status != XST_SUCCESS )
		{
			if( Status == XST_DEVICE_NOT_FOUND )
			{
				xil_printf("GPIO_LED: driver XST_DEVICE_NOT_FOUND...\r\n");
			}
			else
			{
				xil_printf("GPIO_LED: driver: A different error from XST_DEVICE_NOT_FOUND...\r\n");
			}
			xil_printf("GPIO_LED: driver failed to be initialized...\r\n");
			return XST_FAILURE;
		}
		xil_printf("GPIO_LED: driver initialized!\r\n");
	return  XST_SUCCESS;
}

void onLED(int num) {
	XGpio_DiscreteWrite(&Led, LED_CHANNEL, num);
}
void offLED() {
	XGpio_DiscreteWrite(&Led, LED_CHANNEL, 0x00);
}


