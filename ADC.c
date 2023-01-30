/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
/*Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada,
    * para adquirir 6 señales analógicas a una velocidad de 1 millón de muestras 
    * por segundo, por los canales asignados  y guardar los valores en un arreglo 
    * para ser enviados con un botones  externos asociado al gpio D a través del protocolo de 
    * comunicación asíncrona a una velocidad todo esto usando interrupciones. 
    * CECILIA: canales 2,3,4,5,7,8
    * 
    * */
    
    // canal A1N2: PE1 (14)
    // canal A1N3: PE0 (15)
    // canal A1N4: PD7 (128)
    // canal A1N5: PD6 (127)
    // canal A1N7: PD4 (125)
    // canal A1N8: PE5 (124)

    // SS3 - 1
    // SS2 - 4
    // SS1 - 4 - modulo 0 (canales 2,3,4)
    // SS0 - 8 - modulo 1 (canales 5,7,8)

    //Pag 396 para inicializar el modulo 0 Y 1 de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0) | (1<<1); 
    
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                    E      D   
    SYSCTL->RCGCGPIO |= (1<<4)|(0<<3);

    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR &= (0<<0) | (0<<1) | (0<<5); //PE0, PE1, PE5
    GPIOD_AHB->DIR &= (0<<4) | (0<<6) | (0<<7); //PD4 PD6 PD7
    
    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines
    GPIOE_AHB->AFSEL =  (1<<0) | (1<<1) | (1<<5); //PE0, PE1, PE5
    GPIOD_AHB->AFSEL =  (1<<4) | (1<<6) | (1<<7); //PD4 PD6 PD7

    //(GPIODEN) pag.781 desabilita el modo digital
    GPIOE_AHB->DEN = (0<<0) | (0<<1) | (0<<5);
    GPIOD_AHB->DEN = (0<<4) | (0<<6) | (0<<7);

    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FFF00);
    GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0x00F0FFFF);

    //(GPIOAMSEL) pag.786 habilitar analogico
    GPIOE_AHB->AMSEL = (1<<0) | (1<<1) | (1<<5);
    GPIOD_AHB->AMSEL = (1<<4) | (1<<6) | (1<<7);

    ///////////////////// CONFIGURACION ADC ///////////////////////////
    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = (1<<2)|(1<<1)|(1<<0); //para config. a 1 millones de muestra/s
    ADC1->PC = (1<<2)|(1<<1)|(1<<0); 
    
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    //ADC0->SSPRI = 0x3210;
    ADC0->SSPRI = 0x1023;
    ADC1->SSPRI = 0x0213;
    
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  = (0<<3) | (0<<2) | (0<<1) | (0<<0);
    ADC1->ACTSS  = (0<<3) | (0<<2) | (0<<1) | (0<<0);
    
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX = (0x0000);
    ADC1->EMUX = (0x0000);
    
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    //ADC0->SSMUX2 = 0x0089;
    ADC0->SSMUX1 = 0x0234; //canal 2, 3, 4 ->modulo 0
    ADC1->SSMUX0 = 0x0578; //canal 5, 7, 8 ->modulo 1
    
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSCTL1 = ();
    ADC1->SSCTL0 = ();

    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<1); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    ADC1->IM |= (1<<0); // MODULO 1, SS0
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    
    ADC0->ACTSS |= (0<<3) | (0<<2) | (1<<1) | (0<<0); //activar secuenciadores p. 821
    ADC1->ACTSS |= (0<<3) | (0<<2) | (0<<1) | (1<<0);

    ADC0->PSSI |= (0<<1);
    ADC1->PSSI |= (0<<0);
}

extern void ADC0_InSeq1(uint16_t Result[6])
{
       ADC0->PSSI = 0x00000002;
       while((ADC0->RIS&0x02)==0){};
       Result[0] = ADC0->SSFIFO1&0xFFF;
       Result[1] = ADC0->SSFIFO1&0xFFF;
       Result[2] = ADC0->SSFIFO1&0xFFF;
       ADC0->ISC = 0x0002; 
}

extern void ADC1_InSeq0(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000004;
       while((ADC1->RIS&0x04)==0){};
       Result[3] = ADC1->SSFIFO2&0xFFF; 
       Result[4] = ADC1->SSFIFO2&0xFFF;
       ADC1->ISC = 0x0004;
}
