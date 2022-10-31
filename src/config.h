// #pragma once

#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>
// #include <WiFi.h>
// #include <esp_wifi.h> 
// #include "OneButton.h"
// #define Button_Interrupt
// #include <Wire.h>




// #define SHOW_CHINESE
#define GuiTime 		1
#define GuiTempHum 		2
#define GuiWeather 		3
extern uint8_t GuiNum;
void OLED_Task(void *arg);

#define SUN 0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4



#include "PCF8563/PCF8563.h"
extern RTC_PCF8563 pcf;
void PCF8563_Task(void*arg);


#include "DHT20/DHT20.h"
extern DHT20 DHT20_1;
void DHT_Task(void*arg);


#include "SGP30/SGP30.h"
extern SGP30 SGP30_1;
void SGP30_Task(void*arg);

#define GET_O2_FUNCTION
#ifdef GET_O2_FUNCTION
#include "MIX8410/MIX8410.h"
extern MIX8410 MIX8410_1;
void MIX8410_Task(void*arg);
#endif



extern bool Connected_fl;
extern bool Connected_st;

/*心知天气*/
#include <ArduinoJson.h>

extern bool Weather_update;
#ifdef Weather_Library
#include <ESP8266_Seniverse.h>
// String reqUserKey = "SO7CGXFT-EAxYk7x0";                // 心知天气密钥
// String reqLocation = "ip";                         // 城市拼音
// String reqUnit = "c";                                   // 设置单位为 ℃
extern Forecast forecast;
//0为今天的数据，1为明天的数据，2为后天的数据
//         天气代码                最高气温                      最低气温                    湿度
extern String Weather_Code[3] , Weather_HighTemperature[3] , Weather_LowTemperature[3] , Weather_Humidity[3];
extern int count;      //用于控制显示页面
extern bool Weather_update;

#else
typedef struct 
{
    char weather[64];
    String City;
    char City1[64];
    int temp;
    int code;
}WetherData;
extern WetherData Weather;
#endif

void Set_Time(void*arg);
void Get_Weather_Task(void *arg);

#endif