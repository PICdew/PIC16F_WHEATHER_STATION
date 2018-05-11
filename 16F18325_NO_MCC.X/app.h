
#include <xc.h> // include processor files 
#include "interupt.h"
#include "bme280.h"
#include "nrf24.h"

typedef struct { 
    char data[RX_BUFFER_SIZE];
    uint8_t len ;
} buffer;

typedef struct { 
    char data[PAYLOAD_LEN];
    uint8_t len ;
} Airbuffer;

union unpackFloat {
 float f;
 uint8_t b[3];
};

union unpackU16 {
 uint16_t u;
 uint8_t b[2];
};

void interpretation(void);
void Bme280_OneMeasure(float *temp,float * humi,float *press);
void InitBME280andPowerDown(void);
void Init_RF_andPowerDown(uint8_t channel,uint8_t payload_length,uint8_t tx_address[],uint8_t rx_address[]);
void AddTX_U16(uint16_t value );
void AddTXfloat(float the_float);
void AddTXAddr(uint8_t *addr);
void PushDataInTheAir(void);