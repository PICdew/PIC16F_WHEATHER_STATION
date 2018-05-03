
#include "bme280.h"

struct BME280_Calibration_Data cal_data ; 
int32_t   t_fine;

inline uint8_t BME280_read8(uint8_t reg)
{
    uint8_t temp ; 
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_W); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Write(reg); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_R); //lecture
    I2C_Master_Wait();
    temp = I2C_Master_Read(I2C_NACK);
    I2C_Master_Wait();
    I2C_Master_Stop();
    
    return temp;
    
}

inline uint16_t BME280_read16(uint8_t reg)
{
    uint16_t temp ; 
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_W); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Write(reg); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_R); //lecture
    I2C_Master_Wait();
    temp = I2C_Master_Read(I2C_ACK);
    temp<<=8;
    temp |= I2C_Master_Read(I2C_NACK);
    I2C_Master_Wait();
    I2C_Master_Stop();
    
    return temp;
    
}

inline uint32_t BME280_read24(uint8_t reg)
{
    uint32_t temp ; 
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_W); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Write(reg); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_R); //lecture
    I2C_Master_Wait();
    temp = I2C_Master_Read(I2C_ACK);
    temp<<=8;
    I2C_Master_Wait();
    temp |= I2C_Master_Read(I2C_ACK);
    temp<<=8;
    I2C_Master_Wait();
    temp |= I2C_Master_Read(I2C_NACK);
   // I2C_Master_Wait();
    I2C_Master_Stop();
    
    return temp;
    
}

int16_t BME280_readS16(uint8_t reg)
{
    return (int16_t)BME280_read16(reg);
}

 uint16_t BME280_read16_LE(uint8_t reg) {
    
    uint16_t temp =  BME280_read16(reg);
    
    return (temp >> 8) | (temp << 8);
    
}

int16_t BME280_readS16_LE(uint8_t reg)
{
    return (int16_t)BME280_read16_LE(reg);
}

inline int8_t BME280_write8(uint8_t reg,uint8_t value)
{
   I2C_Master_Start();
    I2C_Master_Write(BME280_ADDRESS_W); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Write(reg); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Write(value); //selection egistre
    I2C_Master_Wait();
    I2C_Master_Stop();
    
}

void readSensorCoefficients(void)
{
    
    cal_data.dig_T1 = BME280_read16_LE(BME280_DIG_T1_REG);
    
    cal_data.dig_T2 = BME280_readS16_LE(BME280_DIG_T2_REG);
    
    cal_data.dig_T3 = BME280_readS16_LE(BME280_DIG_T3_REG);
    
    cal_data.dig_P1 = BME280_read16_LE(BME280_DIG_P1_REG);
    
    cal_data.dig_P2 = BME280_readS16_LE(BME280_DIG_P2_REG);
    
    cal_data.dig_P3 = BME280_readS16_LE(BME280_DIG_P3_REG);
    
    cal_data.dig_P4 = BME280_readS16_LE(BME280_DIG_P4_REG);
    
    cal_data.dig_P5 = BME280_readS16_LE(BME280_DIG_P5_REG);
    
    cal_data.dig_P6 = BME280_readS16_LE(BME280_DIG_P6_REG);
    
    cal_data.dig_P7 = BME280_readS16_LE(BME280_DIG_P7_REG);
    
    cal_data.dig_P8 = BME280_readS16_LE(BME280_DIG_P8_REG);
    
    cal_data.dig_P9 = BME280_readS16_LE(BME280_DIG_P9_REG);
    
    cal_data.dig_H1 = BME280_read8(BME280_DIG_H1_REG);
    
    cal_data.dig_H2 = BME280_readS16_LE(BME280_DIG_H2_REG);
    
    cal_data.dig_H3 = BME280_read8(BME280_DIG_H3_REG);
    
    cal_data.dig_H4 = (BME280_read8(BME280_DIG_H4_REG) << 4) | (BME280_read8(BME280_DIG_H4_REG+1) & 0xF);
    
    cal_data.dig_H5 = (BME280_read8(BME280_DIG_H5_REG+1) << 4) | (BME280_read8(BME280_DIG_H5_REG) >> 4);
    
    cal_data.dig_H6 = (int8_t)BME280_read8(BME280_DIG_H6_REG);


}

/*void displayCoefficients(void)
{
    printf("dig_T1 : %u \r\n",cal_data.dig_T1);
    printf("dig_T2 : %d \r\n",cal_data.dig_T2);
    printf("dig_T3 : %d \r\n",cal_data.dig_T3);
    printf("dig_P1 : %u \r\n",cal_data.dig_P1);
    printf("dig_P2 : %d \r\n",cal_data.dig_P2);
    printf("dig_P3 : %d \r\n",cal_data.dig_P3);
    printf("dig_P4 : %d \r\n",cal_data.dig_P4);
    printf("dig_P5 : %d \r\n",cal_data.dig_P5);
    printf("dig_P6 : %d \r\n",cal_data.dig_P6);
    printf("dig_P7 : %d \r\n",cal_data.dig_P7);
    printf("dig_P8 : %d \r\n",cal_data.dig_P8);
    printf("dig_P9 : %d \r\n",cal_data.dig_P9);
    printf("dig_H1 : %u \r\n",cal_data.dig_H1);
    printf("dig_H2 : %d \r\n",cal_data.dig_H2);
    printf("dig_H3 : %u \r\n",cal_data.dig_H3);
    printf("dig_H4 : %u \r\n",cal_data.dig_H4);
    printf("dig_H5 : %u \r\n",cal_data.dig_H5);
    printf("dig_H6 : %d \r\n",cal_data.dig_H6);
   
} */


void BME280_goForceMode(void)
{
    uint8_t mode ; 
    mode = BME280_read8(BME280_REGISTER_CONTROL);
    mode = (mode & 0xFC) + 0x01;
    BME280_write8(BME280_REGISTER_CONTROL,mode);
}

float BME280_readTemperature(void)
{
    float temperature ;
    int32_t var1, var2;
    
    int32_t adc_T = BME280_read24(BME280_REGISTER_TEMPDATA);
        
    adc_T >>= 4;
    
    var1  = ((((adc_T>>3) - ((int32_t)cal_data.dig_T1 <<1))) *
             
             ((int32_t)cal_data.dig_T2)) >> 11;
    
    var2  = (((((adc_T>>4) - ((int32_t)cal_data.dig_T1)) *
               
               ((adc_T>>4) - ((int32_t)cal_data.dig_T1))) >> 12) *
             
             ((int32_t)cal_data.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    
    
    temperature  = (t_fine * 5 + 128) >> 8;
    
    temperature = temperature / 100;
    
    return temperature ;
    
    
    
}




float BME280_readHumidity(void) {
    
    int32_t adc_H = BME280_read16(BME280_REGISTER_HUMIDDATA);
    
    int32_t v_x1_u32r;
    
    v_x1_u32r = (t_fine - ((int32_t)76800));
    
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)cal_data.dig_H4) << 20) -
                    
                    (((int32_t)cal_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 
                 (((((((v_x1_u32r * ((int32_t)cal_data.dig_H6)) >> 10) *
                      
                      (((v_x1_u32r * ((int32_t)cal_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    
                    ((int32_t)2097152)) * ((int32_t)cal_data.dig_H2) + 8192) >> 14));
    
    
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               
                               ((int32_t)cal_data.dig_H1)) >> 4));
    
    
 //   v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    
  //  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    
    float h = (v_x1_u32r>>12);
    
    return  (h / 1024.0);
    
}
// Returns pressure in Pa as unsigned 32 bit integer. Output value of ?96386? equals 96386 Pa = 963.86 hPa
float BME280_readPressure(void)
{
    int32_t adc_P = BME280_read24(BME280_REGISTER_PRESSUREDATA);
    int32_t var1,var2 ;
    uint32_t p ;
    
    
    
  var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
  var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)cal_data.dig_P6);
  var2 = var2 + ((var1*((int32_t)cal_data.dig_P5))<<1);
  var2 = (var2>>2)+(((int32_t)cal_data.dig_P4)<<16);
  var1 = (((cal_data.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)cal_data.dig_P2) * var1)>>1))>>18;
  var1 =((((32768+var1))*((int32_t)cal_data.dig_P1))>>15);
  if (var1 == 0) {
    return 0; // avoid exception caused by division by zero
  }
  p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
  if (p < 0x80000000) {
    p = (p << 1) / ((uint32_t)var1);
  } else {
    p = (p / (uint32_t)var1) * 2;
  }
  var1 = (((int32_t)cal_data.dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
  var2 = (((int32_t)(p>>2)) * ((int32_t)cal_data.dig_P8))>>13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + cal_data.dig_P7) >> 4));
  return (p/100.0);
    
    
    
    
}




