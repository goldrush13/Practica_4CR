#include "lib/include.h"

int main(void)
{
    Configurar_PLL();  //Confiuracion de velocidad de reloj (20MHz)
    Configura_Reg_ADC0();
    Configurar_UART5();

    // VARIABLES
    uint16_t Result[6]; // 6 canales
    uint16_t ch1;
    uint16_t ch2;
    uint16_t ch3;
    uint16_t ch4;
    uint16_t ch5;
    uint16_t ch6;

    while(1)
    {
        ADC0_InSeq1(Result);
        ADC1_InSeq0(Result);
        
        ch1 = (uint16_t)(Result[0]);
        ch2 = (uint16_t)(Result[1]);
        ch3 = (uint16_t)(Result[2]);
        ch4 = (uint16_t)(Result[3]);
        ch5 = (uint16_t)(Result[4]);
        ch6 = (uint16_t)(Result[5]);
        
        
        //valor=(float)(((Result[0]))*3.3)/4096;
        //valor1=(float)(((Result[1]))*3.3)/4096;
    }

}
