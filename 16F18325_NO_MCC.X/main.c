


// Configuration bits: selected in the GUI

// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINT1    // Power-up default value for COSC bits->HFINTOSC (1MHz)
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; I/O or oscillator function on OSC2
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit->MCLR/VPP pin function is MCLR; Weak pull-up enabled
#pragma config PWRTE = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config WDTE = OFF    // Watchdog Timer Enable bits->WDT disabled; SWDTEN is ignored
#pragma config LPBOREN = OFF    // Low-power BOR enable bit->ULPBOR disabled
#pragma config BOREN = ON    // Brown-out Reset Enable bits->Brown-out Reset enabled, SBOREN bit ignored
#pragma config BORV = LOW    // Brown-out Reset Voltage selection bit->Brown-out voltage (Vbor) set to 2.45V
#pragma config PPS1WAY = ON    // PPSLOCK bit One-Way Set Enable bit->The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a Reset
#pragma config DEBUG = OFF    // Debugger enable bit->Background debugger disabled

// CONFIG3
#pragma config WRT = OFF    // User NVM self-write protection bits->Write protection off
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.

// CONFIG4
#pragma config CP = OFF    // User NVM Program Memory Code Protection bit->User NVM code protection disabled
#pragma config CPD = OFF    // Data NVM Memory Code Protection bit->Data NVM code protection disabled

#include "xc.h"
#include "system.h"
#include "eusart.h"
#include "I2C1.h"
#include "BH1750.h"
//#include "bme280.h"
#include "SPI2.h"
#include "interupt.h"
#include <stdio.h>



void main()
{
    
    SYSTEM_Initialize();
    //I2C_Master_Init(100000);
   // spiInit(SPI_MASTER_OSC_DIV4,SPI_DATA_SAMPLE_MIDDLE,SPI_CLOCK_IDLE_LOW,SPI_IDLE_2_ACTIVE);
    uint16_t  lux ;
    float temperature ; 
    float humidity ;
    float pressure ; 
  /*  BME280_write8(BME280_REGISTER_CONTROLHUMID, 0x01); // regler avant  CONTROL !!!
    BME280_write8(BME280_REGISTER_CONTROL, 0b00100100);
    readSensorCoefficients(); */
    EnableGlobalinterupts();
    EnablePeripheralInterupts();
    EnableRXInterupts();
    TRISCbits.TRISC3 = 0 ;
	while(1)
	{
        
       PORTCbits.RC3 =1;
        __delay_ms(1000);
     //   BME280_goForceMode();
        PORTCbits.RC3 =0;
       // lux = LireLux(1);
         __delay_ms(1000);
       
        
    /*    temperature = BME280_readTemperature();
        humidity = BME280_readHumidity();
        pressure = BME280_readPressure();
        
         printf("temperature : %f \r\n",temperature);
         printf("humidite : %f \r\n",humidity);
         printf("pression : %f \r\n",pressure);
      
     //  spiWrite(0xAA); */
    }
}