#include "BH1750.h"
#include "app.h"

void BH1750_Write(unsigned char data1) // // deprecated
{
  	
	I2C_Master_Start();
	I2C_Master_Write(BH1750_address_w);
	I2C_Master_Write(data1);
    I2C_Master_Stop();
}

uint16_t BH1750_Read()  // deprecated
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
 
  
   union unpackU16 Word ;
   Word.b[0] = power_up;
   I2C_Write_data(BH1750_address_w,&Word.b,1);
   switch(resolution)
  {
    case 0:  // 1 mesure haute precision 
     Word.b[0] = one_time_L_res_mode;
    break;
    case 1:  // 1 mesure haute precision 
      Word.b[0] = one_time_H_res_mode1;
    break;
    case 2:  // 1 mesure haute precision 
      Word.b[0] = one_time_H_res_mode2;
    break;
    default: // par defaut haute precison 
     Word.b[0] = one_time_H_res_mode1;
    break;
  }
  
   I2C_Write_data_then_Read(bh1750_address_7bit,&Word.b,1,&Word.b,2,185);
   Word.u = (Word.u >> 8) | (Word.u << 8);
   return (((uint16_t)Word.u/1.20)); 
  
  
     
}
