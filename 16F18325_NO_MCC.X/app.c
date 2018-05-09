

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
     *temperature = BME280_readTemperature();
     *humidity = BME280_readHumidity();
     *pressure = BME280_readPressure();
}
