
/* 
 * File:   
 * Author: Pierre jeanne
 * Comments: library for use MSSP1 as I2C master 
 * Revision history: 
 */



#include <xc.h> // include processor files - each processor file is guarded. 
#include "system.h"

#define I2C_ACK 1
#define I2C_NACK 0
void I2C_Master_Init(const unsigned long c);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_Stop();
void I2C_Master_Write(unsigned d);
int8_t I2C_Master_Write_V2(unsigned d);
unsigned short I2C_Master_Read(unsigned short a);
void I2C_Master_ReStart();
int8_t I2C_Write_data(uint8_t I2C_Slave_8bitAddr,uint8_t *data,uint8_t data_len);
int8_t I2C_Write_data_then_Read(uint8_t I2C_Slave_7bitAddr,uint8_t *dataIn,uint8_t dataIn_len,uint8_t *dataOut,uint8_t dataOut_len);


