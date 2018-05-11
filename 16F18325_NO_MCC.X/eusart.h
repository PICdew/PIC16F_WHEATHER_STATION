
/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define RX_BUFFER_SIZE 3

void SendUART1(unsigned int data);
void UART1_SendStr(const char * s);
void UART1_SendDec(unsigned int data);
void UART1_SendHex(unsigned int data);
unsigned char setHEX(unsigned char ch);
void printTable(uint8_t * table,uint8_t size); 


/**
  Section: Macro Declarations
*/

/**
  Section: EUSART APIs
*/

/**
 * @brief Initialization routine that takes inputs from the EUSART GUI.
 *
 */
void EUSART_Initialize(void);

/**
 * @brief Check if the EUSART transmitter is empty
 *
 * @return The status of EUSART TX empty checking.
 * @retval false the EUSART transmitter is not empty
 * @retval true the EUSART transmitter is empty
 */
bool EUSART_is_tx_ready(void);

/**
 * @brief Check if the EUSART receiver is not empty
 *
 * @return The status of EUSART RX empty checking.
 * @retval false the EUSART receiver is empty
 * @retval true the EUSART receiver is not empty
 */
bool EUSART_is_rx_ready(void);

/**
 * @brief Check if EUSART data is transmitted
 *
 * @return Receiver ready status
 * @retval false  Data is not completely shifted out of the shift register
 * @retval true   Data completely shifted out if the USART shift register
 */
bool EUSART_is_tx_done(void);

/**
 * @brief Read one character from EUSART
 *
 * @return Data read from the EUSART module
 */
uint8_t EUSART_Read(void);

/**
 * @brief Write one character on EUSART
 *
 * @param[in] data The character to write to the EUSART bus
 *
 * @return Nothing
 */
void EUSART_Write(uint8_t txData);
