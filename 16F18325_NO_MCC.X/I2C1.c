/*
 * File:   I2C_Custom.c
 * Author: pierre
 *
 * Created on 25 avril 2018, 20:21
 */

#include "I2C1.h"

void I2C_Master_Init(const unsigned long c)
{
  SSP1CON1 = 0b00101000;            //SSP Module as Master
  SSP1CON2 = 0;
  SSP1ADD = (_XTAL_FREQ/(4*c))-1; //Setting Clock Speed
  SSP1STAT = 0;
  TRISAbits.TRISA5 = 1;                   //Setting as input as given in datasheet
  TRISAbits.TRISA4 = 1;                   //Setting as input as given in datasheet
}
void I2C_Master_Wait()
{
  while ((SSP1STAT & 0x04) || (SSP1CON2 & 0x1F)); //Transmit is in progress
}

 void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SSP1CON2bits.SEN = 1;             //Initiate start condition
}

 void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSP1CON2bits.PEN = 1;           //Initiate stop condition
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
}

int8_t I2C_Master_Write_V2(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
  I2C_Master_Wait();
  return SSP1CON2bits.ACKDT ;

}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  SSP1CON2bits.RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  SSP1CON2bits.ACKDT = (a)?0:1;    //Acknowledge bit
  SSP1CON2bits.ACKEN = 1;          //Acknowledge sequence
  return temp;
}
