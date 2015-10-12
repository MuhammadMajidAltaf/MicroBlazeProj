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
void addAndMultiply(); // Write this function


/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XGpio Gpio; /* The Instance of the GPIO Driver used for LED 0 */


unsigned char keyStatus;

// {RESET,START,STOP}
// 000
// 001
// 010
// 011
// 100
// 101
// 110
// 111


unsigned int decimalPart = 0; 
unsigned int integerPart = 0; 

int main()
{
	int numClockCycles = 0;
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	print("---Entering main---\n\r");
	print("---Trial Name \t Trial # \t Clock Cycles---\n\r");
	int i = 0;
	// Extra Method contains an interrupt routine which is set to go off at timed intervals
    	extra_method();

    initTimer();

	while(1) {

		if (stopWatchRefresh){ //
           	stopWatchRefresh = 0;
           	displayNumbers(integerPart, decimalPart);
       	}
       	
       	KeyDriver();


 	}

 	//	numClockCycles = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, 1);
 	//	xil_printf("print  \t%d\t%d\n", i,numClockCycles );
    // }
}


void initTimer(){
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
}

void KeyDriver(){
    unsigned char i;
    static unsigned char backup[4] = {1,1,1,1};
   
    for (i=0; i<3; i++){ // check keys
        if (backup[i] != KeySta[i]){ //检测按键动作
            if (backup[i] != 0){ //按键按下时执行动作
                if (i == 1){ //Esc 键复位秒表
                    StopwatchReset();
                }else if (i == 2){//回车键启停秒表
                    StopwatchAction();
                }
            }
            backup[i] = KeySta[i]; //刷新前一次的备份值
        }
    }
}

void KeyScan(){
    unsigned char i;
    static unsigned char keybuf[4] = { //按键扫描缓冲区
        0xFF, 0xFF, 0xFF, 0xFF
    };
   
    //按键值移入缓冲区
    keybuf[0] = (keybuf[0] << 1) | KEY1;
    keybuf[1] = (keybuf[1] << 1) | KEY2;
    keybuf[2] = (keybuf[2] << 1) | KEY3;
    keybuf[3] = (keybuf[3] << 1) | KEY4;
    //消抖后更新按键状态
    for (i=0; i<4; i++){
        if (keybuf[i] == 0x00){
            //连续 8 次扫描值为 0，即 16ms 内都是按下状态时，可认为按键已稳定的按下
            KeySta[i] = 0;
        }else if (keybuf[i] == 0xFF){
            //连续 8 次扫描值为 1，即 16ms 内都是弹起状态时，可认为按键已稳定的弹起
            KeySta[i] = 1;
        }
    }
}

void ConfigTimer0(unsigned int ms);
void StopwatchDisplay();
void KeyDriver();


void main(){
    Xil_ICacheEnable();
	Xil_DCacheEnable();

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


	EA = 1;  //开总中断
    ENLED = 0;  //使能选择数码管
    ADDR3 = 1;
    P2 = 0xFE; //P2.0 置 0，选择第 4 行按键作为独立按键
    ConfigTimer0(2); //配置 T0 定时 2ms
    while (1){
        if (StopwatchRefresh){ //需要刷新秒表示数时调用显示函数
            StopwatchRefresh = 0;
            StopwatchDisplay();
        }
        KeyDriver(); //调用按键驱动函数
    }
}

void KeyDriver(){
    unsigned char i;
    static unsigned char backup[4] = {1,1,1,1};
   
    for (i=0; i<4; i++){ //循环检测 4 个按键
        if (backup[i] != KeySta[i]){ //检测按键动作
            if (backup[i] != 0){ //按键按下时执行动作
                if (i == 1){ //Esc 键复位秒表
                    StopwatchReset();
                }else if (i == 2){//回车键启停秒表
                    StopwatchAction();
                }
            }
            backup[i] = KeySta[i]; //刷新前一次的备份值
        }
    }
}





bit KeySta = 1;  //当前按键状态

void main(){
    bit backup = 1;  //按键值备份，保存前一次的扫描值
    unsigned char cnt = 0;  //按键计数，记录按键按下的次数
    EA = 1;  //使能总中断
    ENLED = 0;  //选择数码管 DS1 进行显示
    ADDR3 = 1;
    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;
    TMOD = 0x01; //设置 T0 为模式 1
    TH0 = 0xF8; //为 T0 赋初值 0xF8CD，定时 2ms
    TL0 = 0xCD;
    ET0 = 1;  //使能 T0 中断
    TR0 = 1;  //启动 T0
    P2 = 0xF7;  //P2.3 置 0，即 KeyOut1 输出低电平
    P0 = LedChar[cnt];  //显示按键次数初值
   
    while (1){
        if (KeySta != backup){  //当前值与前次值不相等说明此时按键有动作
            if (backup == 0){  //如果前次值为 0，则说明当前是弹起动作
                cnt++;  //按键次数+1
                if (cnt >= 10){  //只用 1 个数码管显示，所以加到 10 就清零重新开始
                    cnt = 0;
                }
                P0 = LedChar[cnt]; //计数值显示到数码管上
            }
            //更新备份为当前值，以备进行下次比较
            backup = KeySta;
        }
    }
}
/* T0 中断服务函数，用于按键状态的扫描并消抖 */
void InterruptTimer0() interrupt 1{
    //扫描缓冲区，保存一段时间内的扫描值
    static unsigned char keybuf = 0xFF;
   
    TH0 = 0xF8; //重新加载初值
    TL0 = 0xCD;
    //缓冲区左移一位，并将当前扫描值移入最低位
    keybuf = (keybuf<<1) | KEY4;
    //连续 8 次扫描值都为 0，即 16ms 内都只检测到按下状态时，可认为按键已按下
    if (keybuf == 0x00){
        KeySta = 0;
    //连续 8 次扫描值都为 1，即 16ms 内都只检测到弹起状态时，可认为按键已弹起
    }else if (keybuf == 0xFF){
        KeySta = 1;
    }
    else{
        //其它情况则说明按键状态尚未稳定，则不对 KeySta 变量值进行更新
    }
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
}
void blinkLED(int numberOfBlinks) {
	// Fill in this function
}

