#include "config.h"

// DHT dht(DHTTYPE);



DHT20 DHT20_1;


void DHT_Task(void*arg)
{
    DHT20_1.begin();
    while(1){  
        DHT20_1.readData();

    vTaskDelay(2000/portTICK_RATE_MS);

    }
}