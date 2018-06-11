

#include "app.h"

 buffer monBuffer;

 Airbuffer TXbuffer;
 

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
     __delay_ms(200);
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


void AddTXfloat(float the_float)
{
    uint8_t i ;
    union unpackFloat pack1;
    pack1.f = the_float ; 
    for(i=0;i<3;i++)
    {
        TXbuffer.data[TXbuffer.len] = pack1.b[i];
        TXbuffer.len++;
    }
}

void AddTX_U16(uint16_t value )
{
    uint8_t i ;
    union unpackU16 pack1;
    pack1.u = value ; 
    for(i=0;i<2;i++)
    {
        TXbuffer.data[TXbuffer.len] = pack1.b[i];
        TXbuffer.len++;
    }
    
}

void AddTXAddr(uint8_t *addr)
{
    uint8_t i ;
    for(i=0;i<nrf24_ADDR_LEN;i++)
    {
      TXbuffer.data[TXbuffer.len] = addr[i];
      TXbuffer.len++;  
    }
    
}

void PushDataInTheAir(void)
{
    nrf24_send(TXbuffer.data,1);        
    TXbuffer.len = 0 ;   
        uint8_t temp ;
        while(nrf24_isSending(1));

   
        temp = nrf24_lastMessageStatus(1);

        if(temp == NRF24_TRANSMISSON_OK)
        {                    
           UART1_SendStr("> Tranmission went OK\r\n");
        }
        else if(temp == NRF24_MESSAGE_LOST)
        {                    
            UART1_SendStr("> Message is lost ...\r\n");    
        }
        
        temp = nrf24_retransmissionCount(1);
		UART1_SendStr("> Retranmission count:");
        UART1_SendDec(temp);
		UART1_SendStr("\r\n"); 
        __delay_ms(1);
        nrf24_powerDown(1);
        
}

