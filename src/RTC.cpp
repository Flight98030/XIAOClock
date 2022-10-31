#include "config.h"

RTC_PCF8563 pcf(DEV_8563_I2C_NUM);

// Time nowTime;



uint8_t Weather_time = 0;

void PCF8563_Task(void*arg)
{
	while (true)
	{
        // nowTime = pcf.getTime();//get current time
        pcf.getDate();
        pcf.getTime();

        printf("%d:%d:%d\r\n",pcf.hour, pcf.minute, pcf.sec);

        // if((nowTime.hour == 6) && (nowTime.minute == 0) && (nowTime.second == 00)) Lamp_auto = 1;
        if((pcf.minute % 2 ==0 ) && (pcf.sec == 0)) {
            Weather_update = 1;
            Weather_time = pcf.minute;
        }


        if((pcf.hour == 0) && (pcf.minute == 20) && (pcf.sec == 00)) {
            xTaskCreate(Set_Time,
                "Set_Time",
                1024*2,
                NULL,
                10,
                NULL); 
        }

		vTaskDelay(500/portTICK_RATE_MS);
	}
	

}