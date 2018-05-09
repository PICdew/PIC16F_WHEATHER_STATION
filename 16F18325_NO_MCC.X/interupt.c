
#include <pic16f18325.h>
#include "interupt.h"
#include "app.h"


unsigned char RX_byte ;

inline void EnableGlobalinterupts(void)
{
    INTCONbits.GIE = 1 ; 
}
inline void DisableGlobalinterupts(void)
{
    INTCONbits.GIE = 0 ; 
}

inline void EnablePeripheralInterupts(void)
{
    INTCONbits.PEIE = 1 ; 
}

inline void DisablePeripheralInterupts(void)
{
    INTCONbits.PEIE = 0 ; 
}

inline void EnableRXInterupts(void)
{
    PIE1bits.RCIE = 1 ; 
}

inline void DisableRXInterupts(void)
{
    PIE1bits.RCIE = 0 ; 
}



void interrupt interupt1(void)
{
    
    if(PIR1bits.RCIF) // si ils'agit d'une interuption RX
    {
       RX_byte = EUSART_Read();
       if(RX_byte == 0x0D) // si le caractere est "entrée"
       {
          
          interpretation(); 
       }
       else // sinon on le stock
       {
           //printf("stock");
           monBuffer.data[monBuffer.len] = RX_byte; //on stock
           monBuffer.len++; //on incremente la position du buffer
            if(monBuffer.len > RX_BUFFER_SIZE) // si le buffer est plein les caractere suivant ecraserons les autres 
            {
                monBuffer.len = 0 ;
            }
       }
        
       
     
    }

    //autres interuptions ici !
    
}