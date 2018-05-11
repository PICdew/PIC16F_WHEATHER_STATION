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
void UART1_SendStr(const char * s)
{
    while(*s)
            EUSART_Write(*s++);
}

void UART1_SendDec(unsigned int data) // 
{
    unsigned char temp;
    temp=data/1000;
    EUSART_Write(temp+'0');
    data=data-temp*1000;
    temp=data/100;
    EUSART_Write(temp+'0');
    data=data-temp*100;
    temp=data/10;
    EUSART_Write(temp+'0');
    data=data-temp*10;
    EUSART_Write(data+'0');
}

void UART1_SendHex(unsigned int data) // 
{
    unsigned char temp;
     EUSART_Write('0');
     EUSART_Write('x');
    temp = data>>4;
    EUSART_Write(setHEX(temp));
    temp = data&0x0F;
    EUSART_Write(setHEX(temp));
    
    
}

unsigned char setHEX(unsigned char ch)
{
    if(ch >= 0 && ch <= 9)
    {
        return (ch+'0');
    }
    else
    {
        switch(ch)
        {
            case 10:
                return 'A';
            break; 
            case 11:
                return 'B';
            break; 
            case 12:
                return 'C';
            break; 
            case 13:
                return 'D';
            break; 
            case 14:
                return 'E';
            break; 
            case 15:
                return 'F';
            break; 
        }
    }
}


void printTable(uint8_t * table,uint8_t size)
{
    uint8_t i ;
    for(i = 0; i<size;i++)
    {
        if (table[i] != NULL)
        {
            UART1_SendHex(table[i]);
            UART1_SendStr(" ");
        }
    }
}     