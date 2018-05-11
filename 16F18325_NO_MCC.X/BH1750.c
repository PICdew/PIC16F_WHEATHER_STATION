#include "BH1750.h"

void BH1750_Write(unsigned char data1)
{
  	
	I2C_Master_Start();
	I2C_Master_Write(BH1750_address_w);
	I2C_Master_Write(data1);
  I2C_Master_Stop();
}

uint16_t BH1750_Read()
{
	uint16_t res ;
  I2C_Master_Start();
	I2C_Master_Write(BH1750_address_r);
    res =  I2C_Master_Read(I2C_ACK);
    res <<= 8;
    res |=  I2C_Master_Read(I2C_NACK);
    I2C_Master_Wait();
    I2C_Master_Stop();
    return res;
     
}



uint16_t LireLux(int8_t resolution)
{
   uint16_t lux ;
  BH1750_Write(power_up);
  switch(resolution)
  {
    case 0:  // 1 mesure haute precision 
      BH1750_Write(one_time_L_res_mode);
    break;
    case 1:  // 1 mesure haute precision 
      BH1750_Write(one_time_H_res_mode1);
    break;
    case 2:  // 1 mesure haute precision 
      BH1750_Write(one_time_H_res_mode2);
    break;
    default: // par defaut haute precison 
      BH1750_Write(one_time_H_res_mode1);
    break;
  }
  
  __delay_ms(200);
  lux = BH1750_Read();
  lux = lux/1.20 ;
   return lux;
  
     
}
