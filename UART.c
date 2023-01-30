#include "lib/include.h"

// MI configuración UART5 que corresponde al PE4(59Rx) y PE5(60Tx)

extern void Configurar_UART5(void)
{
    ///////////////////  CONFIGURACIÓN PINES /////////////////////
    // Habilitar RELOJES <UART> y <GPIO> con REGISTROS
    SYSCTL->RCGCUART  = (1<<5);     //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port E
    
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOE_AHB->AFSEL = (1<<4) | (1<<5); // Habilita función alternativa en el pin E4 y E5
    
    //GPIO Port Control (GPIOPCTL) to assign the UART signals to the appropriate pins
    //PE4-> U5Rx PE5-> U5Tx                                 
    //GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011

    GPIOE_AHB->PCTL =  0x00110000;// (1<<5) | (1<<4);//0x00000011
    
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE_AHB->DEN = (1<<4) | (1<<5);  //PE4 PE5
    

    /////////////////// CONFIGURACIÓN UART /////////////////////
    //UART Control (UARTCTL) pag.918 DISABLE!!
    UART5->CTL = (0<<9) | (0<<8) | (0<<0);
    //            RXE       TXE    UARTEN (UARTENable)

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
        = 40,000,000 / (16 * 28800) = 86.805

    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
                               (0.805 * 64 + 0.5) = 52.02

    */
    UART5->IBRD = 86;
    
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART5->FBRD = 52;
    
    //  UART Line Control (UARTLCRH) pag. 916 Serial parameters such as data length, parity, and stop bit selection are implemented in this register.
    UART5->LCRH = (0x3<<5);      //PARA TENER 8 bits(0x3)
    //UART5->LCRH = (0x3<<5)|(1<<4);  |(1<<4) activar el bit de paridad pero solo se emplea con el uart0
    
    //  UART Clock Configuration(UARTCC) pag.939
    UART5->CC =(0<<0);      //usaremos el reloj PRINCIPAL
    
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART5->CTL = (1<<0) | (1<<8) | (1<<9);
    //            RXE       TXE      UARTEN



}

/*extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 );
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}*/

extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}

extern void printString(char *string) //enviar una cadena
{
    while(*string)
    {
        printChar(*(string++));
        
    }
}

/*extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}*/

//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

