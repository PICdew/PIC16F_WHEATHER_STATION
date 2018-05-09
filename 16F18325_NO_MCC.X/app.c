

#include "app.h"

extern RXbuffer monBuffer ;

void interpretation(void)
{
    uint8_t i ;
    monBuffer.len = 0; 
    printf("la commande est : \r\n");
    for(i=0;i<3;i++)
    {
        printf(" %c",monBuffer.data[i]);
    }
}

void InitBME280andPowerDown(void)
{
	BME280_write8(BME280_REGISTER_CONTROLHUMID, 0x01); // regler avant  CONTROL !!!
    BME280_write8(BME280_REGISTER_CONTROL, 0b00100100);
    readSensorCoefficients(); 
}

void Bme280_OneMeasure(float *temp,float * humi,float *press)
{
	 BME280_goForceMode();
     *temp = BME280_readTemperature();
     *humi = BME280_readHumidity();
     *press = BME280_readPressure();
}

void Init_RF_andPowerDown(uint8_t channel,uint8_t payload_length,uint8_t tx_address[],uint8_t rx_address[])
{
    nrf24_setPayload(payload_length);
    nrf24_config(channel,1);
    nrf24_tx_address(tx_address,1);
    nrf24_rx_address(rx_address,1); 
    nrf24_powerDown(1);
}
