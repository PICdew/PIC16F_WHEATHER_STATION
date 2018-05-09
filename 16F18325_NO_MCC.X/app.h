
#include <xc.h> // include processor files 
#include "interupt.h"
#include "bme280.h"
#include "nrf24.h"
void interpretation(void);
void Bme280_OneMeasure(float *temp,float * humi,float *press);
void InitBME280andPowerDown(void);
void Init_RF_andPowerDown(uint8_t channel,uint8_t payload_length,uint8_t tx_address[],uint8_t rx_address[]);