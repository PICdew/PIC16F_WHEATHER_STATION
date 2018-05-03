
#include <pic16f18325.h>

#include "SPI2.h"


void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISCbits.TRISC2 = 0; //SDO sortie 
    TRISCbits.TRISC1 = 1; //SDI entrée
    if(sType & 0b00000100) //If Slave Mode
    {
        SSP2STAT = sTransmitEdge;
        TRISCbits.TRISC0 = 1;
    }
    else              //If Master Mode
    {
        SSP2STAT = sDataSample | sTransmitEdge;
        TRISCbits.TRISC0 = 0;
    }
    
    SSP2CON1 = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSP2STATbits.BF ); // Wait for Data Receive complete
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSP2BUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSP2STATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
    spiReceiveWait();        // wait until the all bits receive
    return(SSP2BUF); // read the received data from the buffer
}