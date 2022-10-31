#include "PCF8563/PCF8563.h"

static const char *TAG = "PCF8563.CPP";

RTC_PCF8563::RTC_PCF8563(i2c_port_t i2c_num)
{
	_address = DEV_8563_Read_add >> 1;
	_i2c_num = i2c_num;
}

/**
 * @brief 初始化RTC,所有寄存器清零
 * 
 */
void RTC_PCF8563::initClock()
{
	uint8_t buf[16] = {0};
	buf[0]  = 0x00;	    //寄存器地址
	buf[1]  = 0x00;	    //REG1
	buf[2]  = 0x00;	    //REG2
    /*时间*/
	buf[3]  = 0x01;     //设置秒
  	buf[4]  = 0x01;     //设置分
  	buf[5]  = 0x01;     //设置时
    /*日期*/
  	buf[6]  = 0x01;     //设置日
  	buf[7]  = 0x01;     //设置星期
  	buf[8]  = 0x01;     //设置月，世纪
  	buf[9] 	= 0x01;     //设置年
    /*闹钟*/
  	buf[10] = 0x80;     //设置分钟为 00
  	buf[11] = 0x80;     //设置小时为 00
  	buf[12] = 0x80;     //设置 日 为 00
  	buf[13] = 0x80;     //设置星期为 00
    
  	buf[14] = 0x00;     //clockout设置为0
  	buf[15] = 0x00;     //定时器REG设置为0
	
	esp_err_t ret = i2c_master_WriteData(_i2c_num,
										_address<<1 | WRITE_BIT,
										buf,16,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"RTC Init ERROR!\r\n");					
}

/**
 * @brief   10进制转Bcd编码
 * 
 * @param   val 
 * @return  uint8_t 
 */
uint8_t RTC_PCF8563::decToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}

/**
 * @brief Bcd编码转10进制
 * 
 * @param val 
 * @return uint8_t 
 */
uint8_t RTC_PCF8563::bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

//=====================================================================

/**
 * @brief 清除REG1,REG2
 * 
 */
void RTC_PCF8563::clearStatus()
{
	uint8_t buf[3] = {0};
	buf[0] = 0x00;	//REG1地址
	buf[1] = 0x00;	//REG1数据
	buf[2] = 0x00;	//REG2数据

	esp_err_t ret = i2c_master_WriteData(_i2c_num,
										_address<<1 | WRITE_BIT,
										buf,3,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"Clear Status ERROR!\r\n");

}


/**
 * @brief 设置时间
 * 
 * @param hour      小时
 * @param minute    分钟
 * @param sec       秒
 */
void RTC_PCF8563::setTime(uint8_t hour,uint8_t minute,uint8_t sec)
{
	uint8_t buf[4] = {0};
	buf[0] = PCF8563_SEC_ADDR;		//秒寄存器地址
	buf[1] = decToBcd(sec);			//秒
	buf[2] = decToBcd(minute);		//分
	buf[3] = decToBcd(hour);		//时

	esp_err_t ret = i2c_master_WriteData(_i2c_num,
										_address<<1 | WRITE_BIT,
										buf,4,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"Set Time ERROR!\r\n");
}


/**
 * @brief 设置日期
 * 
 * @param day 
 * @param weekday 
 * @param mon 
 * @param century 
 * @param year 
 */
void RTC_PCF8563::setDate(uint8_t day, uint8_t weekday, uint8_t mon, uint8_t century, uint8_t year)
{
	/*  年 = 00 ~ 99,
        月的最高位为世纪位
        世纪位 : 0=20xx, 1=19xx
        */
	month = decToBcd(mon);
	if(century ==1)	{
		month |= PCF8563_CENTURY_MASK;
	}
	else {
		month &= ~PCF8563_CENTURY_MASK;
	}

	uint8_t buf[5] = {0};
	buf[0] = PCF8563_DAY_ADDR;
	buf[1] = decToBcd(day);
	buf[2] = decToBcd(weekday);
	buf[3] = month;
	buf[4] = decToBcd(year);

	esp_err_t ret = i2c_master_WriteData(_i2c_num,
										_address<<1 | WRITE_BIT,
										buf,5,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"Set Day ERROR!\r\n");
}

/* 使能闹钟中断
 * whenever the clock matches these values an int will
 * 芯片的第3引脚会输出中断(开漏)
 */
void RTC_PCF8563::enableAlarm()
{

    //设置REG2的AF为0
    status2 &= ~PCF8563_ALARM_AF;
    //使能中断
    status2 |= PCF8563_ALARM_AIE;

	esp_err_t ret = i2c_WriteCommand(_i2c_num,_address,
									PCF8563_STAT2_ADDR,status2,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"En AlaeRm ERROR!\r\n");

}

/*
* 读取寄存器数据
*/
uint8_t RTC_PCF8563::readStatus2()
{
    uint8_t buf[1] = {0};

	esp_err_t ret = i2c_Read_nbyte(_i2c_num,_address,
									PCF8563_STAT2_ADDR,buf,1);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"Read Status2 ERROR!\r\n");

	return buf[0];

}


/*
* 返回AIE位的结果
*/
uint8_t RTC_PCF8563::alarmEnabled()
{
    return RTC_PCF8563::readStatus2() & PCF8563_ALARM_AIE;
}

/*
* 返回AF结果
*/
uint8_t RTC_PCF8563::alarmActive()
{
    return RTC_PCF8563::readStatus2() & PCF8563_ALARM_AF;
}


/**
 * @brief 设置闹钟
 * 
 * @param min       分钟
 * @param hour      小时
 * @param day       日期
 * @param weekday   星期
 */
void RTC_PCF8563::setAlarm(uint8_t min, uint8_t hour, uint8_t day, uint8_t weekday)
{
    if (min <99) {
        min = constrain(min, 0, 59);
        min = decToBcd(min);
        min &= ~PCF8563_ALARM;
    } else {
        min = 0x0; min |= PCF8563_ALARM;
    }

    if (hour <99) {
        hour = constrain(hour, 0, 23);
        hour = decToBcd(hour);
        hour &= ~PCF8563_ALARM;
    } else {
        hour = 0x0; hour |= PCF8563_ALARM;
    }

    if (day <99) {
        day = constrain(day, 1, 31);
        day = decToBcd(day); day &= ~PCF8563_ALARM;
    } else {
        day = 0x0; day |= PCF8563_ALARM;
    }

    if (weekday <99) {
        weekday = constrain(weekday, 0, 6);
        weekday = decToBcd(weekday);
        weekday &= ~PCF8563_ALARM;
    } else {
        weekday = 0x0; weekday |= PCF8563_ALARM;
    }

    RTC_PCF8563::enableAlarm();

    uint8_t buf[5] = {0};
	buf[0] = PCF8563_ALRM_MIN_ADDR;
	buf[1] = min;
	buf[2] = hour;
	buf[3] = day;
	buf[4] = weekday;

	esp_err_t ret = i2c_master_WriteData(_i2c_num,
										_address<<1 | WRITE_BIT,
										buf,5,true);
	if(ret != ESP_OK)	ESP_LOGW(TAG,"Set Alarm ERROR!\r\n");
}


/**
 * @brief 获取闹钟的值
 * 
 */
void RTC_PCF8563::getAlarm()
{
	uint8_t buf[4] = {0};
	esp_err_t ret = i2c_Read_nbyte(_i2c_num,_address,
					            PCF8563_ALRM_MIN_ADDR,buf,4);
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Get Alarm ERROR!\r\n");
    
    alarm_minute = buf[0];
    if(0B10000000 & alarm_minute){
        alarm_minute = PCF8563_NO_ALARM;
    } else {
        alarm_minute = bcdToDec(alarm_minute & 0B01111111);
    }
    alarm_hour = buf[1];
    if(0B10000000 & alarm_hour){
        alarm_hour = PCF8563_NO_ALARM;
    } else {
        alarm_hour = bcdToDec(alarm_hour & 0B00111111);
    }
    alarm_day = buf[2];
    if(0B10000000 & alarm_day){
        alarm_day = PCF8563_NO_ALARM;
    } else {
        alarm_day = bcdToDec(alarm_day  & 0B00111111);
    }
    alarm_weekday = buf[3];
    if(0B10000000 & alarm_weekday){
        alarm_weekday = PCF8563_NO_ALARM;
    } else {
        alarm_weekday = bcdToDec(alarm_weekday  & 0B00000111);
    }
}


/**
 * @brief 设置CLKOUT输出
 * 
 * @param frequency     设置输出的频率
 */
void RTC_PCF8563::setSquareWave(uint8_t frequency)
{
	esp_err_t ret = i2c_WriteCommand(_i2c_num,_address,
					                PCF8563_SQW_ADDR,frequency,true);
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Set Clkout ERROR!\r\n");
}

/**
 * @brief 禁用CLKOUT输出
 * 
 */
void RTC_PCF8563::clearSquareWave()
{
    RTC_PCF8563::setSquareWave(SQW_DISABLE);
}


/**
 * @brief 重置闹钟,清除中断
 * 
 */
void RTC_PCF8563::resetAlarm()
{
    //set status2 AF val to zero to reset alarm
    status2 &= ~PCF8563_ALARM_AF;
    esp_err_t ret = i2c_WriteCommand(_i2c_num,_address,
					    PCF8563_STAT2_ADDR,status2,true);
    
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Reset Alarm ERROR!\r\n");
}


/**
 * @brief 清除闹钟
 * 
 */
void RTC_PCF8563::clearAlarm()
{
    //set status2 AF val to zero to reset alarm 
    status2 &= ~PCF8563_ALARM_AF;
    //turn off the interrupt
    status2 &= ~PCF8563_ALARM_AIE;
        
 	esp_err_t ret = i2c_WriteCommand(_i2c_num,_address,
					    PCF8563_STAT2_ADDR,status2,true);
    
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Clear Alarm ERROR!\r\n");
}


/**
 * @brief 获取日期
 * 
 */
void RTC_PCF8563::getDate()
{  
    //读数据
    uint8_t buf[4] = {0};
    esp_err_t ret = i2c_Read_nbyte(_i2c_num,_address,
									PCF8563_DAY_ADDR,buf,4);
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Get Date ERROR!\r\n");


    //0x3f = 0b00111111
    day = bcdToDec((buf[0]) & 0x3f);
    //0x07 = 0b00000111
    weekday = bcdToDec((buf[1]) & 0x07);
    //分离世纪与月
    month = buf[2];
    if (month & PCF8563_CENTURY_MASK) {
        century = 1;
    }
    else {
        century = 0;
    }
    //0x1f = 0b00011111
    month = month & 0x1f;
    month = bcdToDec(month);

    year = bcdToDec(buf[3]);
}


/**
 * @brief 读时间
 * 
 */
void RTC_PCF8563::getTime()
{  
    uint8_t buf[5] = {0};
    esp_err_t ret = i2c_Read_nbyte(_i2c_num,_address,
									PCF8563_STAT1_ADDR,buf,5);
    if(ret != ESP_OK)	ESP_LOGW(TAG,"Get Time ERROR!\r\n");
    
    //request 5 bytes
    status1 = buf[0];
    status2 = buf[1];
    //0x7f = 0b01111111
    sec = bcdToDec((buf[2]) & 0x7f);
    minute = bcdToDec((buf[3]) & 0x7f);
    //0x3f = 0b00111111
    hour = bcdToDec((buf[4]) & 0x3f);
}


// uint8_t RTC_PCF8563::getSecond() {   
//     getTime();
//     return sec; 
// }

// uint8_t RTC_PCF8563::getMinute() {
//     getTime();
//     return minute;
// }

// uint8_t RTC_PCF8563::getHour() {
//     getTime();
//     return hour;
// }

// uint8_t RTC_PCF8563::getDay() {
//     getDate();
//     return day;
// }

// uint8_t RTC_PCF8563::getMonth() {
//     getDate();
//     return month;
// }

// uint8_t RTC_PCF8563::getYear() {
//     getDate();
//     return year;
// }

// uint8_t RTC_PCF8563::getWeekday() {
//     getDate();
//     return weekday;
// }

// char *RTC_PCF8563::formatTime(uint8_t style)
// {
//     getTime();
//     switch (style) {
//         case PCF8563_TIME_HM:
//             strOut[0] = '0' + (hour / 10);
//             strOut[1] = '0' + (hour % 10);
//             strOut[2] = ':';
//             strOut[3] = '0' + (minute / 10);
//             strOut[4] = '0' + (minute % 10);
//             strOut[5] = '\0';
//             break;
//         case PCF8563_TIME_HMS:
//         default:
//             strOut[0] = '0' + (hour / 10);
//             strOut[1] = '0' + (hour % 10);
//             strOut[2] = ':';
//             strOut[3] = '0' + (minute / 10);
//             strOut[4] = '0' + (minute % 10);
//             strOut[5] = ':';
//             strOut[6] = '0' + (sec / 10);
//             strOut[7] = '0' + (sec % 10);
//             strOut[8] = '\0';
//             break;
//         }
//     return strOut;
// }
 

// char *RTC_PCF8563::formatDate(uint8_t style)
// {
//     getDate();
    
//         switch (style) {
        
//         case PCF8563_DATE_ASIA:
//             //do the asian style, yyyy-mm-dd
//             if ( century == 1 ){
//                 strDate[0] = '1';
//                 strDate[1] = '9';
//             }
//             else {
//                 strDate[0] = '2';
//                 strDate[1] = '0';
//             }
//             strDate[2] = '0' + (year / 10 );
//             strDate[3] = '0' + (year % 10);
//             strDate[4] = '-';   
//             strDate[5] = '0' + (month / 10);
//             strDate[6] = '0' + (month % 10);
//             strDate[7] = '-';
//             strDate[8] = '0' + (day / 10);
//             strDate[9] = '0' + (day % 10);
//             strDate[10] = '\0';
//             break;
//         case PCF8563_DATE_US: 
//       //the pitiful US style, mm/dd/yyyy
//             strDate[0] = '0' + (month / 10);
//             strDate[1] = '0' + (month % 10);
//             strDate[2] = '/';
//             strDate[3] = '0' + (day / 10);
//             strDate[4] = '0' + (day % 10);
//             strDate[5] = '/';
//             if ( century == 1 ){
//                 strDate[6] = '1';
//                 strDate[7] = '9';
//             }
//             else {
//                 strDate[6] = '2';
//                 strDate[7] = '0';
//             }
//             strDate[8] = '0' + (year / 10 );
//             strDate[9] = '0' + (year % 10);
//             strDate[10] = '\0';
//             break;
//         case PCF8563_DATE_WORLD:
//         default:
//             //do the world style, dd-mm-yyyy
//             strDate[0] = '0' + (day / 10);
//             strDate[1] = '0' + (day % 10);
//             strDate[2] = '-';
//             strDate[3] = '0' + (month / 10);
//             strDate[4] = '0' + (month % 10);
//             strDate[5] = '-';
            
//             if ( century == 1 ){
//                 strDate[6] = '1';
//                 strDate[7] = '9';
//             }
//             else {
//                 strDate[6] = '2';
//                 strDate[7] = '0';
//             }
//             strDate[8] = '0' + (year / 10 );
//             strDate[9] = '0' + (year % 10);
//             strDate[10] = '\0';
//             break;  
        
//     }
//     return strDate;
// }












