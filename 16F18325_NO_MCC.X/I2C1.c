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

 void I2C_Master_ReStart()
{
  I2C_Master_Wait();    
  SSP1CON2bits.RSEN = 1;             //Initiate Restart condition
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
  return SSP1CON2bits.ACKSTAT ;

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

int8_t I2C_Write_data(uint8_t I2C_Slave_8bitAddr,uint8_t *data,uint8_t data_len)
{
  uint8_t i ;
  I2C_Master_Start();  // start condtion 
  if(I2C_Master_Write_V2(I2C_Slave_8bitAddr)) // si 1 --> Acknowledge was not received (datasheet p362)
  {
    I2C_Master_Stop(); // on stop
    return -1 ; // on exit avec erreur 
  }
  for(i=0;i<data_len;i++) // on envoie les données en fonction de la longeur et on verifie que l'esclave acquitte chaque octet 
  {
    if(I2C_Master_Write_V2(data[i])) // si 1 --> Acknowledge was not received (datasheet p362)
    {
      I2C_Master_Stop(); // on stop
      return -1 ; // on exit avec erreur 
    }
  }
  
 I2C_Master_Stop();
 return 0 ; //on exit avec succes 
}


int8_t I2C_Write_data_then_Read(uint8_t I2C_Slave_7bitAddr,uint8_t *dataIn,uint8_t dataIn_len,uint8_t *dataOut,uint8_t dataOut_len)
{
  uint8_t i ;
  I2C_Master_Start();  // start condtion 
  if(I2C_Master_Write_V2((I2C_Slave_7bitAddr<<1))) // adresse en ecriture 
  {
    I2C_Master_Stop(); // on stop
    return -1 ; // on exit avec erreur 
  }
  for(i=0;i<dataIn_len;i++) // on envoie les données en fonction de la longeur et on verifie que l'esclave acquitte chaque octet 
  {
    if(I2C_Master_Write_V2(*dataIn[i])) // si 1 --> Acknowledge was not received (datasheet p362)
    {
      I2C_Master_Stop(); // on stop
      return -1 ; // on exit avec erreur 
    }
  }
  
 I2C_Master_ReStart() // restart 

 if(I2C_Master_Write_V2(((I2C_Slave_7bitAddr<<1)|0x01))) // addresse en lecture 
  {
    I2C_Master_Stop(); // on stop
    return -1 ; // on exit avec erreur 
  }
  for(i=0;i<dataOut_len;i++) // on recoit les données et on acquite sauf la derniere qu'on marque d'un >NACK pour dire qu'on a fini 
  {
    I2C_Master_Wait();
    SSP1CON2bits.RCEN = 1;
    I2C_Master_Wait();
    *dataOut[i] = SSPBUF;      //Read data from SSPBUF
    I2C_Master_Wait();
    if(i == (dataOut_len-1)) // si on recoit le dernier on NACK
    {
       SSP1CON2bits.ACKDT = 1;    //Acknowledge bit
       SSP1CON2bits.ACKEN = 1; 
    }
    else // sinon ACk 
    {
       SSP1CON2bits.ACKDT = 0   //Acknowledge bit
       SSP1CON2bits.ACKEN = 1; 
    }
   
  }
  I2C_Master_Stop();
  return 0 ;


}
