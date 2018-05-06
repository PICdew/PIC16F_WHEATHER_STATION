

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
