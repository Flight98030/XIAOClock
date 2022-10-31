/**
 * @file    PCF8563.h
 * @author  彬_Linus (you@domain.com)
 * @brief   基于:Joe Robertson 的 Rtc_Pcf8563(Arduino) 修改
 *          github : https://github.com/orbitalair/Rtc_Pcf8563
 *          此组件仅适用于ESP32
 *          需要My_i2c组件支持
 * 
 * @brief   based on Rtc_Pcf8563 by Joe Robertson
 *          github : https://github.com/orbitalair/Rtc_Pcf8563
 *          This component only supports ESP32
 *          This component requires a component named My_i2c
 * @version 1.0
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once

#include <stdio.h>
#include "stdlib.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "My_i2c/My_i2c.h"


#define DEV_8563_Read_add      0xa2         /*!< PCF8563 I2C Address 	*** PCF8563地址 */
#define DEV_8563_SCL_NUM       7            /*!< gpio number for SCL 	*** SCL的IO口*/   
#define DEV_8563_SDA_NUM       6            /*!< gpio number for SDA 	*** SDA的IO口 */  
#define DEV_8563_I2C_NUM       (i2c_port_t)0            /*!< I2C port number  		*** I2C编号*/   


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) 

/* 寄存器地址 */
#define PCF8563_STAT1_ADDR         0x00
#define PCF8563_STAT2_ADDR         0x01
#define PCF8563_SEC_ADDR           0x02
#define PCF8563_MIN_ADDR           0x03
#define PCF8563_HR_ADDR            0x04
#define PCF8563_DAY_ADDR           0x05
#define PCF8563_WEEKDAY_ADDR       0x06
#define PCF8563_MONTH_ADDR         0x07
#define PCF8563_YEAR_ADDR          0x08
#define PCF8563_ALRM_MIN_ADDR      0x09
#define PCF8563_SQW_ADDR           0x0D

/* setting the alarm flag to 0 enables the alarm.
 * set it to 1 to disable the alarm for that value.
 */
#define PCF8563_ALARM              0x80
#define PCF8563_ALARM_AIE          0x02
#define PCF8563_ALARM_AF           0x08 // 0x08 : not 0x04!!!!
/* optional val for no alarm setting */
#define PCF8563_NO_ALARM               99

#define PCF8563_CENTURY_MASK   0x80

/* date format flags */
#define PCF8563_DATE_WORLD         0x01
#define PCF8563_DATE_ASIA          0x02
#define PCF8563_DATE_US            0x04
/* time format flags */
#define PCF8563_TIME_HMS           0x01
#define PCF8563_TIME_HM            0x02

/* square wave contants */
#define SQW_DISABLE     0B00000000
#define SQW_32KHZ       0B10000000
#define SQW_1024HZ      0B10000001
#define SQW_32HZ        0B10000010
#define SQW_1HZ         0B10000011




class RTC_PCF8563{
    public:

		RTC_PCF8563(i2c_port_t);
        void initClock();
		void clearStatus();

		void getDate();		/* get date vals to local vars */
		void setDate(uint8_t day, uint8_t weekday, uint8_t month, uint8_t century, uint8_t year);
        void getTime();    /* get time vars + 2 status bytes to local vars */
        void getAlarm();
        void setTime(uint8_t hour, uint8_t minute, uint8_t sec);
        uint8_t readStatus2();
        uint8_t alarmEnabled();
        uint8_t alarmActive();

        void enableAlarm(); /* activate alarm flag and interrupt */
        void setAlarm(uint8_t min, uint8_t hour, uint8_t day, uint8_t weekday); /* set alarm vals, 99=ignore */
        void clearAlarm();  /* clear alarm flag and interrupt */
        void resetAlarm();  /* clear alarm flag but leave interrupt unchanged */
        void setSquareWave(uint8_t frequency);
        void clearSquareWave();

        // uint8_t getSecond();
        // uint8_t getMinute();
        // uint8_t getHour();
        // uint8_t getDay();
        // uint8_t getMonth();
        // uint8_t getYear();
        // uint8_t getWeekday();

        // /*get a output string, these call getTime/getDate for latest vals */
        // char *formatTime(uint8_t style = PCF8563_TIME_HMS);
        // /* date supports 3 styles as listed in the wikipedia page about world date/time. */
        // char *formatDate(uint8_t style = PCF8563_DATE_US);

        // char *version();

        /* time variables */
        uint8_t hour;
        uint8_t minute;
        uint8_t sec;
        uint8_t day;
        uint8_t weekday;
        uint8_t month;
        uint8_t year;
        uint8_t century;



    private:
        uint8_t _address;
        i2c_port_t _i2c_num;
        // int i2c_sda_IO;
        // int i2c_scl_IO,
		/* methods */
        uint8_t decToBcd(uint8_t value);
        uint8_t bcdToDec(uint8_t value);
		 /* time variables */
        // uint8_t hour;
        // uint8_t minute;
        // uint8_t sec;
        // uint8_t day;
        // uint8_t weekday;
        // uint8_t month;
        // uint8_t year;
        /* alarm */
        uint8_t alarm_hour;
        uint8_t alarm_minute;
        uint8_t alarm_weekday;
        uint8_t alarm_day;
        /* support */
        uint8_t status1;
        uint8_t status2;

        char strOut[9];
        char strDate[11];
};

