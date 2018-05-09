

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
    I2C_Master_Init(100000);
#ifndef SOFT
  spiInit(SPI_MASTER_OSC_DIV4,SPI_DATA_SAMPLE_MIDDLE,SPI_CLOCK_IDLE_LOW,SPI_ACTIVE_2_IDLE);
#endif
}

void OSCILLATOR_Initialize(void)
{
    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; SOSCBE crystal oscillator; 
    OSCCON3 = 0x00;
    // LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled;  // peut etre activer LFOEN pour le WDT a verifier ...
    OSCEN = 0x00;
    // HFFRQ 2_MHz; 
    OSCFRQ = 0x01;
    // HFTUN 0; 
    OSCTUNE = 0x00;
}

void WDT_Initialize(void)
{
    // WDTPS 1:65536; SWDTEN OFF; 
    CLRWDT();
    WDTCON = 0x1B;
}

void PMD_Initialize(void)
{
    //ICI on desactive tout ce qui n'est pas utile !!

    // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; FVRMD FVR disabled; IOCMD IOC disabled; NVMMD NVM disabled; 
    PMD0 = 0x07;
    // TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR4MD TMR4 disabled; TMR5MD TMR5 disabled; TMR2MD TMR2 disabled; TMR3MD TMR3 disabled; NCOMD DDS(NCO) disabled; TMR6MD TMR6 disabled; 
    PMD1 = 0xFC;
    // DACMD DAC disabled; CMP1MD CMP1 disabled; ADCMD ADC disabled; CMP2MD CMP2 disabled; 
    PMD2 = 0x66;
    // CCP2MD CCP2 disabled; CCP1MD CCP1 disabled; CCP4MD CCP4 disabled; CCP3MD CCP3 disabled; PWM6MD PWM6 disabled; PWM5MD PWM5 disabled; CWG2MD CWG2 disabled; CWG1MD CWG1 disabled; 
    PMD3 = 0xFF;
    // MSSP1MD MSSP1 enabled; UART1MD EUSART enabled; MSSP2MD MSSP2 enabled; 
    PMD4 = 0x00;
    // DSMMD DSM disabled; CLC3MD CLC3 disabled; CLC4MD CLC4 disabled; CLC1MD CLC1 disabled; CLC2MD CLC2 disabled; 
    PMD5 = 0x1F;
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
    TRISCbits.TRISC3 = 0 ;  // A revoir ! 
    #ifdef SOFT
    TRISCbits.TRISC0 = 0 ;
    TRISCbits.TRISC1 = 1 ;
    TRISCbits.TRISC2 = 0 ;   
    #endif
    /**
    ANSELx registers
    */                //pas d'analogique !
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