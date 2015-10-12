#include "xtmrctr.h"
#include "xintc.h"
#include "xparameters.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"
#include "mb_interface.h"
#include <xbasic_types.h>
#include <xio.h>

#define PUSHBUTTON_DEVICE_ID XPAR_XPS_GPIO_0_DEVICE_ID
#define PUSHBUTTON_INTERRUPT_ID XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
#define PUSHBUTTON_MASK XPAR_XPS_GPIO_0_IP2INTC_IRPT_MASK

#define GPIO_DEVICE_ID_BUTTON          XPAR_XPS_INTC_0_DEVICE_ID
//#define GPIO_DEVICE_ID_ROTARY          XPAR_XPS_GPIO_1_DEVICE_ID

#define INTC_DEVICE_ID                 XPAR_INTC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID_BUTTON  XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
#define BUTTON_INTERRUPT XGPIO_IR_CH1_MASK  /* Channel 1 Interrupt Mask */
#define RESET_VALUE                    0xC00 //0x10000 //0x017D7840

//#define TMRCTR_INTERRUPT_ID         XPAR_INTC_0_TMRCTR_0_VEC_ID

void extra_handler();
void extra_disable();
void extra_enable();
int extra_method();
