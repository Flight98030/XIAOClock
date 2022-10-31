#include "config.h"


#ifdef GET_O2_FUNCTION
MIX8410 MIX8410_1;


void MIX8410_Task(void*arg)
{

    while (1)
    {
        MIX8410_1.readData();
        printf("O2: %4.2f\n",MIX8410_1.O2results);
        vTaskDelay(3000/portTICK_RATE_MS);
    }
    
}




#endif