#include "config.h"

SGP30 SGP30_1;


void SGP30_Task(void*arg)
{
    SGP30_1.begin();
    while (1)
    {
        SGP30_1.readData();
        printf("\r\nCO2:%dppm\r\nTVOC:%dppd\r\n",SGP30_1.eCO2,SGP30_1.TVOC);

        vTaskDelay(5000/portTICK_RATE_MS);
    }
    
}