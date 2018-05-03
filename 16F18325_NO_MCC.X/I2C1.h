
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
