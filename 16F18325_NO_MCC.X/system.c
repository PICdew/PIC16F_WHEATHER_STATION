

#include "system.h"
#include "I2C1.h"
#include "SPI2.h"

void SYSTEM_Initialize(void)
{

    PMD_Initialize();
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    WDT_Initialize();
    EUSART_Initialize();
   // I2C_Master_Init(100000);
#ifndef SOFT
  spiInit(SPI_MASTER_OSC_DIV4,SPI_DATA_SAMPLE_END,SPI_CLOCK_IDLE_HIGH,SPI_IDLE_2_ACTIVE);
#endif
}

void OSCILLATOR_Initialize(void)
{
    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; SOSCBE crystal oscillator; 
    OSCCON3 = 0x00;
    // LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 2_MHz; 
    OSCFRQ = 0x01;
    // HFTUN 0; 
    OSCTUNE = 0x00;
}

void WDT_Initialize(void)
{
    // WDTPS 1:65536; SWDTEN OFF; 
    WDTCON = 0x16;
}

void PMD_Initialize(void)
{
    // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; FVRMD FVR enabled; IOCMD IOC enabled; NVMMD NVM enabled; 
    PMD0 = 0x00;
    // TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR4MD TMR4 enabled; TMR5MD TMR5 enabled; TMR2MD TMR2 enabled; TMR3MD TMR3 enabled; NCOMD DDS(NCO) enabled; TMR6MD TMR6 enabled; 
    PMD1 = 0x00;
    // DACMD DAC enabled; CMP1MD CMP1 enabled; ADCMD ADC enabled; CMP2MD CMP2 enabled; 
    PMD2 = 0x00;
    // CCP2MD CCP2 enabled; CCP1MD CCP1 enabled; CCP4MD CCP4 enabled; CCP3MD CCP3 enabled; PWM6MD PWM6 enabled; PWM5MD PWM5 enabled; CWG2MD CWG2 enabled; CWG1MD CWG1 enabled; 
    PMD3 = 0x00;
    // MSSP1MD MSSP1 enabled; UART1MD EUSART enabled; MSSP2MD MSSP2 enabled; 
    PMD4 = 0x00;
    // DSMMD DSM enabled; CLC3MD CLC3 enabled; CLC4MD CLC4 enabled; CLC1MD CLC1 enabled; CLC2MD CLC2 enabled; 
    PMD5 = 0x00;
}

void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISA = 0x37;
    TRISC = 0x2A;
    TRISAbits.TRISA1 = 0 ;
    TRISAbits.TRISA2 = 0 ;
    #ifdef SOFT
    TRISCbits.TRISC0 = 0 ;
    TRISCbits.TRISC1 = 1 ;
    TRISCbits.TRISC2 = 0 ;   
    #endif
    /**
    ANSELx registers
    */
    ANSELC = 0x00;
    ANSELA = 0x00;
    

    /**
    WPUx registers
    */
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONA = 0x00;
    ODCONC = 0x00;
	SSP1DATPPSbits.SSP1DATPPS = 0x04;   //RA4->MSSP1:SDA1;    
    SSP1CLKPPSbits.SSP1CLKPPS = 0x05;   //RA5->MSSP1:SCL1;    
    RXPPSbits.RXPPS = 0x15;   //RC5->EUSART:RX;    
    RA4PPSbits.RA4PPS = 0x19;   //RA4->MSSP1:SDA1;    
    RC4PPSbits.RC4PPS = 0x14;   //RC4->EUSART:TX;    
    #ifndef SOFT
    RC0PPSbits.RC0PPS = 0x1A;   //RC0->MSSP2:SCK2;    
    RC2PPSbits.RC2PPS = 0x1B;   //RC2->MSSP2:SDO2;    
    SSP2CLKPPSbits.SSP2CLKPPS = 0x10;   //RC0->MSSP2:SCK2;    
    SSP2DATPPSbits.SSP2DATPPS = 0x11;   //RC1->MSSP2:SDI2;    
    RA5PPSbits.RA5PPS = 0x18;   //RA5->MSSP1:SCL1;    
    #endif
}