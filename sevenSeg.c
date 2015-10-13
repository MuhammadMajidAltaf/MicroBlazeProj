

/***************************** Include Files *******************************/
#include "sevenSeg.h"
#include "xparameters.h"
#include "xil_io.h"
/************************** Function Definitions ***************************/
unsigned int numPosition[] = {  // Position
    0xFE << 7, 0xFD << 7, 0xFB << 7, 0xF7 << 7,
    0xEF << 7, 0xDF << 7, 0xBF << 7, 0x7F << 7, 0xFF << 7
};

unsigned int numPattern[] = { // Character
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10 };

void displayNumbers(unsigned int pos, unsigned int integerPart, unsigned int decimalPart){
    unsigned char digits[8];

    digits[0] = decimalPart % 10;
    digits[1] = (decimalPart / 10) % 10;
    digits[2] = (decimalPart / 100) % 10;
    digits[3] = (decimalPart / 1000) % 10;

    digits[4] = integerPart % 10;
    digits[5] = (integerPart / 10) % 10;
    digits[6] = (integerPart / 100) % 10;
    digits[7] = (integerPart / 1000) % 10;


    SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, numPosition[pos] | numPattern[ digits[pos] ]);
}
