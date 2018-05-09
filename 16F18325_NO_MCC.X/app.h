
#include <xc.h> // include processor files 
#include "interupt.h"

void interpretation(void);
void Bme280_OneMeasure(float *temp,float * humi,float *press);
void InitBME280andPowerDown(void);
