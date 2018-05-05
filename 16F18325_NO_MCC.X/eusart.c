/*
    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "eusart.h"

/**
  Section: EUSART APIs
*/

void EUSART_Initialize(void)
{
    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x0A;
    //0000 1010

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;
    //1001 0000
    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;
    //0010 0100
    // Baud Rate = 9600; SP1BRGL 51; 
    SP1BRGL = 0x33;
   
    // Baud Rate = 9600; SP1BRGH 0; 
    SP1BRGH = 0x00;

}


bool EUSART_is_tx_ready(void)
{
    return (PIR1bits.TXIF && TX1STAbits.TXEN);
}

bool EUSART_is_rx_ready(void)
{
    return PIR1bits.RCIF;
}

bool EUSART_is_tx_done(void)
{
    return TX1STAbits.TRMT;
}

uint8_t EUSART_Read(void)
{

    RC1STAbits.SREN = 1;	
    while(!PIR1bits.RCIF)
    {
    }

    
    if(1 == RC1STAbits.OERR)
    {
        // EUSART error - restart

        RC1STAbits.SPEN = 0; 
        RC1STAbits.SPEN = 1; 
    }

    return RC1REG;
}

void EUSART_Write(uint8_t txData)
{
    while(0 == PIR1bits.TXIF)
    {
    }

    TX1REG = txData;    // Write the data byte to the USART.
}

char getch(void)
{
    return EUSART_Read();
}

void putch(char txData)
{
    EUSART_Write(txData);
}
/**
  End of File
*/
