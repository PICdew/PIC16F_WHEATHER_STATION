
#include "nrf24.h"
#ifndef SOFT

uint8_t spi_transfer(uint8_t tx)
{
    return SPI2_Exchange8bit(tx);
}
#else

uint8_t spi_transfer(uint8_t tx)
{
    uint8_t i = 0;
    uint8_t rx = 0;    

    set_sck(0);

    for(i=0;i<8;i++)
    {

        if(tx & (1<<(7-i)))
        {
            set_mosi(1);            
        }
        else
        {
            set_mosi(0);
        }

        set_sck(1);        

        rx = rx << 1;
        if(get_miso())
        {
            rx |= 0x01;
        }

        set_sck(0);                

    }

    return rx;
}
#endif
void set_ce(uint8_t state,uint8_t radioNb)
{
    switch(radioNb)
    {
        case 1:
            if(state) cePin1=1; else cePin1=0;
        break;
        case 2:
            if(state) cePin2=1; else cePin2=0;
        break;
      
    }
}
/* ------------------------------------------------------------------------- */
void set_csn(uint8_t state,uint8_t bus)
{
    switch(bus)
    {
        case 1:
            if(state) csnPin1=1; else csnPin1=0;
        break;
        case 2:
            if(state) csnPin2=1; else csnPin2=0;
        break;
       
        
    }
    
}
#ifdef SOFT
/* ------------------------------------------------------------------------- */
void set_sck(uint8_t state)
{
    if(state) sckPin=1; else sckPin=0;
}
/* ------------------------------------------------------------------------- */
void set_mosi(uint8_t state)
{
    if(state) mosiPin=1; else mosiPin=0;
}
/* ------------------------------------------------------------------------- */
uint8_t get_miso()
{
    return misoPin;
}

#endif



/* send and receive multiple bytes over SPI */
void nrf24_transferSync(uint8_t* dataout,uint8_t* datain,uint8_t len)
{
    uint8_t i;

    for(i=0;i<len;i++)
    {
        datain[i] = spi_transfer(dataout[i]);
    }

}

/* send multiple bytes over SPI */
void nrf24_transmitSync(uint8_t* dataout,uint8_t len)
{
    uint8_t i;
    
    for(i=0;i<len;i++)
    {
        spi_transfer(dataout[i]);
    }

}

/* Clocks only one byte into the given nrf24 register */
void nrf24_configRegister(uint8_t reg, uint8_t value,uint8_t bus)
{
    set_csn(0,bus);
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    spi_transfer(value);
    set_csn(1,bus);
}

/* Read single register from nrf24 */
void nrf24_readRegister(uint8_t reg, uint8_t* value, uint8_t len,uint8_t bus)
{
    set_csn(0,bus);
    spi_transfer(R_REGISTER | (REGISTER_MASK & reg));
    nrf24_transferSync(&value,&value,len);
    set_csn(1,bus);
}

/* Write to a single register of nrf24 */
void nrf24_writeRegister(uint8_t reg, uint8_t* value, uint8_t len,uint8_t bus) 
{
    set_csn(0,bus);
    spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
    nrf24_transmitSync(value,len);
    set_csn(1,bus);
}

void nrf24_config(uint8_t channel, uint8_t pay_length,uint8_t radioNb)
{
    set_csn(1,1);
    set_ce(0,1);
    set_csn(1,2);
    set_ce(0,2);

    // Set RF channel
    nrf24_configRegister(RF_CH,channel,radioNb);

    // Set length of incoming payload 
    nrf24_configRegister(RX_PW_P0, 0x00,radioNb); // Auto-ACK pipe ...
    nrf24_configRegister(RX_PW_P1, pay_length,radioNb); // Data payload pipe
    nrf24_configRegister(RX_PW_P2, 0x00,radioNb); // Pipe not used 
    nrf24_configRegister(RX_PW_P3, 0x00,radioNb); // Pipe not used 
    nrf24_configRegister(RX_PW_P4, 0x00,radioNb); // Pipe not used 
    nrf24_configRegister(RX_PW_P5, 0x00,radioNb); // Pipe not used 

    // 1 Mbps, TX gain: 0dbm
    //nrf24_configRegister(RF_SETUP, (0<<RF_DR)|((0x03)<<RF_PWR));
	// 250Kbps, TXgain 0dbm 
	nrf24_configRegister(RF_SETUP,0b00100110,radioNb);
	

    // CRC enable, 1 byte CRC length
    nrf24_configRegister(CONFIG,((1<<EN_CRC)|(0<<CRCO)),radioNb);

    // Auto Acknowledgment
    nrf24_configRegister(EN_AA,(1<<ENAA_P0)|(1<<ENAA_P1)|(0<<ENAA_P2)|(0<<ENAA_P3)|(0<<ENAA_P4)|(0<<ENAA_P5),radioNb);

    // Enable RX addresses
    nrf24_configRegister(EN_RXADDR,(1<<ERX_P0)|(1<<ERX_P1)|(0<<ERX_P2)|(0<<ERX_P3)|(0<<ERX_P4)|(0<<ERX_P5),radioNb);

    // Auto retransmit delay: 1000 us and Up to 15 retransmit trials
    nrf24_configRegister(SETUP_RETR,(0xFF<<ARD)|(0x0F<<ARC),radioNb);

    // Dynamic length configurations: No dynamic length
    nrf24_configRegister(DYNPD,(0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5),radioNb);

    // Start listening
    nrf24_powerUpRx(radioNb);
}

void nrf24_powerUpRx(uint8_t radioNb)
{     
    set_csn(0,radioNb);
    spi_transfer(FLUSH_RX);
    set_csn(1,radioNb);

    nrf24_configRegister(STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT),radioNb); 

    set_ce(0,radioNb);    
    nrf24_configRegister(CONFIG,((1<<EN_CRC)|(0<<CRCO))|((1<<PWR_UP)|(1<<PRIM_RX)),radioNb);    
    set_ce(1,radioNb);
}

void nrf24_rx_address(uint8_t * adr,uint8_t RadioNb) 
{
    set_ce(0,RadioNb);
    nrf24_writeRegister(RX_ADDR_P1,adr,nrf24_ADDR_LEN,RadioNb);
    set_ce(1,RadioNb);
}

void nrf24_tx_address(uint8_t* adr,uint8_t RadioNb)
{
  
    nrf24_writeRegister(TX_ADDR,adr,nrf24_ADDR_LEN,RadioNb);
}

uint8_t nrf24_dataReady(uint8_t RadioNb) 
{
    // See note in getData() function - just checking RX_DR isn't good enough
    uint8_t status = nrf24_getStatus(RadioNb);

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if ( status & (1 << RX_DR) ) 
    {
        return 1;
    }

    return !nrf24_rxFifoEmpty(RadioNb);;
}

/* Checks if receive FIFO is empty or not */
uint8_t nrf24_rxFifoEmpty(uint8_t RadioNb)
{
    uint8_t fifoStatus;

    nrf24_readRegister(FIFO_STATUS,&fifoStatus,1,RadioNb);
    
    return (fifoStatus & (1 << RX_EMPTY));
}

/* Returns the length of data waiting in the RX fifo */
uint8_t nrf24_payloadLength(uint8_t RadioNb)
{
    uint8_t status;
    set_csn(0,RadioNb);
    spi_transfer(R_RX_PL_WID);
    status = spi_transfer(0x00);
    set_csn(1,RadioNb);
    return status;
}


void nrf24_getData(uint8_t* dta,uint8_t RadioNb) 
{
    /* Pull down chip select */
    set_csn(0,RadioNb);                               

    /* Send cmd to read rx payload */
    spi_transfer( R_RX_PAYLOAD );
    
    /* Read payload */
    nrf24_transferSync(dta,dta,PAYLOAD_LEN);
    
    /* Pull up chip select */
    set_csn(1,RadioNb);

    /* Reset status register */
    nrf24_configRegister(STATUS,(1<<RX_DR),RadioNb);   
}

/* Returns the number of retransmissions occured for the last message */
uint8_t nrf24_retransmissionCount(uint8_t RadioNb)
{
    uint8_t rv;
    nrf24_readRegister(OBSERVE_TX,&rv,1,RadioNb);
    rv = rv & 0x0F;
    return rv;
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
void nrf24_send(uint8_t* value,uint8_t RadioNb) 
{    
    /* Go to Standby-I first */
    set_ce(0,RadioNb);
     
    /* Set to transmitter mode , Power up if needed */
    nrf24_powerUpTx(RadioNb);

    /* Do we really need to flush TX fifo each time ? */
    #if 1
        /* Pull down chip select */
        set_csn(0,RadioNb);           

        /* Write cmd to flush transmit FIFO */
        spi_transfer(FLUSH_TX);     

        /* Pull up chip select */
        set_csn(1,RadioNb);                    
    #endif 

    /* Pull down chip select */
    set_csn(0,RadioNb);

    /* Write cmd to write payload */
    spi_transfer(W_TX_PAYLOAD);

    /* Write payload */
    nrf24_transmitSync(value,PAYLOAD_LEN);   

    /* Pull up chip select */
    set_csn(1,RadioNb);

    /* Start the transmission */
    set_ce(1,RadioNb);    
}

uint8_t nrf24_isSending(uint8_t RadioNb)
{
    uint8_t status;

    /* read the current status */
    status = nrf24_getStatus(RadioNb);
                
    /* if sending successful (TX_DS) or max retries exceded (MAX_RT). */
    if((status & ((1 << TX_DS)  | (1 << MAX_RT))))
    {        
        return 0; /* false */
    }

    return 1; /* true */

}

uint8_t nrf24_getStatus(uint8_t RadioNb)
{
    uint8_t rv;
    set_csn(0,RadioNb);
    rv = spi_transfer(NOP);
    set_csn(1,RadioNb);
    return rv;
}

uint8_t nrf24_lastMessageStatus(uint8_t RadioNb)
{
    uint8_t rv;

    rv = nrf24_getStatus(RadioNb);

    /* Transmission went OK */
    if((rv & ((1 << TX_DS))))
    {
        return NRF24_TRANSMISSON_OK;
    }
    /* Maximum retransmission count is reached */
    /* Last message probably went missing ... */
    else if((rv & ((1 << MAX_RT))))
    {
        return NRF24_MESSAGE_LOST;
    }  
    /* Probably still sending ... */
    else
    {
        return 0xFF;
    }
}

void nrf24_powerUpTx(uint8_t RadioNb)
{
    nrf24_configRegister(STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT),RadioNb); 

    nrf24_configRegister(CONFIG,nrf24_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)),RadioNb);
}

void nrf24_powerDown(uint8_t RadioNb)
{
    set_ce(0,RadioNb);
    nrf24_configRegister(CONFIG,nrf24_CONFIG,RadioNb);
}

/*void nrf24_displayConfiguration(uint8_t radioNb)

{

               uint8_t tab[5] = { NULL };

               nrf24_readRegister(CONFIG,tab,1,radioNb);

               UART1_SendStr("CONFIG REGISTER(Configuration Register) :  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(EN_AA,tab,1,radioNb);

               UART1_SendStr("EN_AA REGISTER (Enable Auto Acknowledgment):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(EN_RXADDR,tab,1,radioNb);

               UART1_SendStr("EN_RXADDR (Enabled RX pipes):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(SETUP_AW,tab,1,radioNb);

               UART1_SendStr("SETUP_AW (Setup of Address Width) \r\n");

               UART1_SendStr(" (common for all data pipes) : ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(SETUP_RETR,tab,1,radioNb);

               UART1_SendStr("SETUP_RETR (Setup of Automatic Retransmission):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RF_CH,tab,1,radioNb);

               UART1_SendStr("RF_CH (RF Channel):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RF_SETUP,tab,1,radioNb);

               UART1_SendStr("RF_SETUP (RF Setup Register):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(STATUS,tab,1,radioNb);

               UART1_SendStr("STATUS (Status Register):  ");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P0,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P0 (Receive address data pipe 0):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P1,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P1 (Receive address data pipe 1):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P2,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P2 (Receive address data pipe 2):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P3,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P3 (Receive address data pipe 3):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P4,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P4 (Receive address data pipe 4):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_ADDR_P5,tab,5,radioNb);

               UART1_SendStr("RX_ADDR_P5 (Receive address data pipe 5):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(TX_ADDR,tab,5,radioNb);

               UART1_SendStr("TX_ADDR (Transmit address):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

              

               nrf24_readRegister(RX_PW_P0,tab,1,radioNb);

               UART1_SendStr("RX_PW_P0 (Number of bytes in RX payload in data pipe 0):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_PW_P1,tab,1,radioNb);

               UART1_SendStr("RX_PW_P1 (Number of bytes in RX payload in data pipe 1):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_PW_P2,tab,1,radioNb);

               UART1_SendStr("RX_PW_P2 (Number of bytes in RX payload in data pipe 2):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_PW_P3,tab,1,radioNb);

               UART1_SendStr("RX_PW_P3 (Number of bytes in RX payload in data pipe 3):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_PW_P4,tab,1,radioNb);

               UART1_SendStr("RX_PW_P4 (Number of bytes in RX payload in data pipe 4):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(RX_PW_P5,tab,1,radioNb);

               UART1_SendStr("RX_PW_P5 (Number of bytes in RX payload in data pipe 5):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

              

               nrf24_readRegister(DYNPD,tab,1,radioNb);

               UART1_SendStr("DYNPD (Enable dynamic payload length):");

               printTable(tab,5);

               UART1_SendStr("\r\n");

               clearTable(tab,5);

       
              

              

}

void clearTable(uint8_t  *tab,uint8_t size)
{
    uint8_t i ;
    for(i = 0; i<size;i++)
    {
        tab[i] = NULL ;
    }
}

*/
