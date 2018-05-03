#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define EUSART_DataReady  (EUSART_is_rx_ready())

extern volatile uint8_t eusartTxBufferRemaining;
extern volatile uint8_t eusartRxCount;



void EUSART_Initialize(void);

uint8_t EUSART_is_tx_ready(void);

uint8_t EUSART_is_rx_ready(void);

bool EUSART_is_tx_done(void);

uint8_t EUSART_Read(void);

void EUSART_Write(uint8_t txData);
