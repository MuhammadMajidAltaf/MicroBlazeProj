

/***************************** include Files *******************************/
#include "sevenSeg.h"

/************************** Function Definitions ***************************/
unsigned int numPosition[] = {  // Position
    0xFE << 7, 0xFD << 7, 0xFB << 7, 0xF7 << 7,
    0xEF << 7, 0xDF << 7, 0xBF << 7, 0x7F << 7, 0xFF << 7
};

unsigned int numPattern[] = { // Character
    0x40, 0x79 /*0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E*/
};

void displayNumbers(unsigned int integerPart, unsigned int decimalPart){
    unsigned char digits[8];
    unsigned int i;


    digits[0] = decimalPart % 10;
    digits[1] = (decimalPart / 10) % 10;
    digits[2] = (decimalPart / 100) % 10;
    digits[3] = (decimalPart / 1000) % 10;

    digits[4] = integerPart % 10;
    digits[5] = (integerPart / 10) % 10;
    digits[6] = (integerPart / 100) % 10;
    digits[7] = (integerPart / 1000) % 10;

    for(i=0; i<8; i++){
        SEVENSEG_mWriteReg(XPAR_SEVENSEG_0_S00_AXI_BASEADDR, 0, numPosition[i] | numPattern[ digit[i] ]);
    }

   
}
