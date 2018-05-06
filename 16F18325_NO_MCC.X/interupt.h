
#include <xc.h>
#include "eusart.h"
#include <stdio.h>

typedef struct { 
    char data[RX_BUFFER_SIZE];
    uint8_t len ;
} RXbuffer;
RXbuffer monBuffer ;



inline void EnableGlobalinterupts(void);
inline void DisableGlobalinterupts(void);
inline void EnablePeripheralInterupts(void);
inline void DisablePeripheralInterupts(void);
inline void EnableRXInterupts(void);
inline void DisableRXInterupts(void);
