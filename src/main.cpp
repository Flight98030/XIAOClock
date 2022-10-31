#include "config.h"

#include <WiFi.h>
#include <esp_wifi.h> 

#include <ArduinoJson.h>
/**
 * @brief PlatformIO ESP32 Packet Version：5.1.1
 * 
 */

#ifdef AliyunIOT
    /*AliyunIOT阿里云IOT*/
    #include <AliyunIoTSDK.h>
    #include "PubSubClient.h"
    #error
    #define PRODUCT_KEY "Your AliyunIOT key"
    #define DEVICE_NAME "XIAO"
    #define DEVICE_SECRET "Your AliyunIOT Device Secret"
    #define REGION_ID "cn-shanghai"
    WiFiClient espClient;
#else
    #define BLINKER_PRINT Serial
    #define BLINKER_WIFI

    #define SmartConfig
    #ifdef SmartConfig
        #define BLINKER_ESP_SMARTCONFIG
    #endif
    /*小爱同学*/
    #define BLINKER_MIOT_SENSOR


    #include <Blinker.h>
    #error
    char auth[] = "Your Blinker auth";
    char ssid[] = "Mi10S";
    char pswd[] = "1234567890";
    /*温湿度*/
    BlinkerNumber HUMI("humi");
    BlinkerNumber TEMP("temp");
    BlinkerNumber CO2D("CO2");
    #ifdef GET_O2_FUNCTION
    BlinkerNumber O2D("O2");
    #endif
    BlinkerButton Button1("btn-Clock");
    BlinkerButton Button2("btn-Data");
    BlinkerButton Button3("btn-Weather");

#endif


#define BUTTON_PIN 	D1










bool Connected_fl = 0;
bool Connected_st = 0;

uint8_t GuiNum = 1;
#define DelayTime   20
// bool buttonState = 0;         // variable for reading the pushbutton status

/*心知天气*/
bool Weather_update = 0;
#ifdef Weather_Library
int count=0;      //用于控制显示页面：0显示时间，1显示今天天气，2显示明天天气，3显示后天天气
Forecast forecast;         // 建立Forecast对象用于获取心知天气3天预报信息
#error
String reqUserKey = "Your Key";                // 心知天气密钥
String reqLocation = "ip";                         // 城市拼音
String reqUnit = "c";
String Weather_Code[3] , Weather_HighTemperature[3] , Weather_LowTemperature[3] , Weather_Humidity[3];

#else
#error
const char *Weather_host = "api.seniverse.com";
const char *Weather_privateKey = "Your Key";
const char *Weather_city = "ip";
const char *Weather_language = "en";
WetherData Weather;
#endif

/*NTP Server*/
const char* ntpServer1 = "time.cloudflare.com";


void Set_Time(void*arg)
{
    while(1){
        while ((WiFi.status() != WL_CONNECTED) /*&& (Connected_fl == 0)*/) {
            printf("Connecting to Wi-Fi... \r\n");
            Connected_fl = 0;
            
            vTaskDelay(1000 / portTICK_PERIOD_MS);

        }

        printf("Getting NTP Time...\r\n");
        Connected_fl = 1;   
        
        Serial.print("Connecting to Wi-Fi ");
        while (WiFi.status() != WL_CONNECTED) {
           

            Connected_fl = 0;
            vTaskDelay(500/portTICK_RATE_MS);
        }
        Serial.println(" CONNECTED");

        
        // Set ntp time to local
        configTime(8 * 3600, 0, ntpServer1);

        struct tm timeInfo;
        if (getLocalTime(&timeInfo)) {
            //设置时、分、秒
            pcf.setTime(timeInfo.tm_hour, timeInfo.tm_min,timeInfo.tm_sec);
            //设置日、星期、月、世纪(0= 20xx, 1=19xx)、年(0~99)
            pcf.setDate(timeInfo.tm_mday, timeInfo.tm_wday, timeInfo.tm_mon + 1,0, timeInfo.tm_year-100);
            printf("SNTP GET TIME\r\n");
        }
        else if(!getLocalTime(&timeInfo)) printf("SNTP GET TIME ERROR\r\n");

        
        Connected_fl = 1;
        Weather_update = 1;


        xTaskCreate(Get_Weather_Task,
                "Get_Weather_Task",
                1024*3,
                NULL,
                15,
                NULL);
                


        vTaskDelete( NULL );
        
        
    }
}


#ifdef Weather_Library
//获取今天、明天、后天的天气情况
void Get_WeatherForecast(void)
{
    int j;
    if(forecast.update())       // 更新天气信息
    {
        for(j=0;j<=2;j++)
        {
            Weather_Code[j]=forecast.getDayCode(j);
            Weather_HighTemperature[j]=forecast.getHigh(j);
            Weather_LowTemperature[j]=forecast.getLow(j);
            Weather_Humidity[j]=forecast.getHumidity(j);
        }  
    } 
}


#else
void Get_Weather_Json()
{
    // put your main code here, to run repeatedly:
    WiFiClient Weather_client;

    if (!Weather_client.connect(Weather_host, 80))
    {
        printf("Connect host failed!\n");
        // Serial.println("Connect host failed!");
        return;
    }
    printf("host Conected!\n");
    // Serial.println("host Conected!");

    String getUrl = "/v3/weather/now.json?key=";
    getUrl += Weather_privateKey;
    getUrl += "&location=";
    getUrl += Weather_city;
    getUrl += "&language=";
    getUrl += Weather_language;
    // getUrl += "&unit=c";

    Weather_client.print(String("GET ") + getUrl + " HTTP/1.1\r\n" + 
                "Host: " + Weather_host + "\r\n" + 
                "Connection: close\r\n\r\n");
    // Serial.println("Get send");
    printf("Get send\n");
    delay(1000);  

    /* Read all the lines of the reply from server and print them to Serial */
    String answer;
    while(Weather_client.available()){
      String line = Weather_client.readStringUntil('\r');
      answer += line;
    }
    /* Disconnect to the server */
    Weather_client.stop();
    Serial.println();
    printf("closing Weather connection\n");
    // Serial.println("");

    /* Get Json data*/
    String jsonAnswer;
    int jsonIndex;
    for (int i = 0; i < answer.length(); i++) {
      if (answer[i] == '{') {
        jsonIndex = i;
        break;
      }
    }
    jsonAnswer = answer.substring(jsonIndex);
    Serial.println();

    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 210;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, jsonAnswer);

    JsonObject results_0 = doc["results"][0];

    JsonObject results_0_location = results_0["location"];
    const char* results_0_location_id = results_0_location["id"]; // "WS10730EM8EV"
    const char* results_0_location_name = results_0_location["name"]; // "深圳"
    const char* results_0_location_country = results_0_location["country"]; // "CN"
    const char* results_0_location_path = results_0_location["path"]; // "深圳,深圳,广东,中国"
    const char* results_0_location_timezone = results_0_location["timezone"]; // "Asia/Shanghai"
    const char* results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

    JsonObject results_0_now = results_0["now"];
    const char* results_0_now_text = results_0_now["text"]; // "阴"
    const char* results_0_now_code = results_0_now["code"]; // "9"
    const char* results_0_now_temperature = results_0_now["temperature"]; // "27"

    const char* results_0_last_update = results_0["last_update"]; // "2022-04-26T10:50:00+08:00"


    strcpy(Weather.weather,results_0_now_text);
    strcpy(Weather.City1,results_0_location_name);

    Weather.City = results_0_location_name;
    Weather.code = atoi(results_0_now_code);
    Weather.temp = atoi(results_0_now_temperature);


    // Serial.print("city:Weather:");
    // Serial.println(results_0_now_text);  
    printf("City: %s, Weather: %s\n",results_0_location_name,results_0_now_text);
    printf("Weather Code: %d\n",Weather.code);
    printf("Weather Temp: %d\n",Weather.temp);


}
#endif

void Get_Weather_Task(void *arg)
{
     printf("Get Weather Task Creat\r\n");
    while(1){
        if(Connected_fl && Weather_update){
            #ifdef Weather_Library
                Get_WeatherForecast();        //获取天气
            #else
                Get_Weather_Json();
            #endif

            Weather_update = 0;
        }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}



#ifdef Button_Interrupt
void IRAM_ATTR Key_Detectuin() 
{
    printf("Key Interrupt!!\n");
    vTaskDelay(DelayTime / portTICK_PERIOD_MS);   /* 延时50ms消抖*/
    
    if (digitalRead(BUTTON_PIN) == 0)
    {
        printf("Key1 pressed!\n");
        // Button4_flag = 1;
        GuiNum++;
        if(GuiNum > 3)  GuiNum = 1;
    }
}

#else
/* 按键检测任务函数 */
void Button_Task(void *arg)
{
    static int key_up = 1;   /* 按键松开标志 */
    while (1)
    {
        /* 检测按键是否按下 */
        if (key_up && (digitalRead(BUTTON_PIN) == 0) )
        {
            vTaskDelay(DelayTime / portTICK_PERIOD_MS);   /* 延时50ms消抖*/
            key_up = 0;
            if (digitalRead(BUTTON_PIN) == 0)
            {
                printf("Key1 pressed!\n");
                GuiNum++;
                if(GuiNum > 3)  GuiNum = 1;
            }
        }
        
        else if(digitalRead(BUTTON_PIN) == 1)
        {
            key_up = 1;     /* 按键已松开 */
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
#endif






#include "OLED/oled.h"
#include "OLED/oled_gui.h"
void OLED_Task(void *arg)
{
	OLED_Init();
	OLED_ColorTurn(0);  //0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    StartUp();
    OLED_Clear();
    uint8_t GuiNum_f = GuiTime;
    // uint8_t t = ' ';
	while(1)
	{
		
        if(GuiNum != GuiNum_f) {
            OLED_Clear();
            GuiNum_f = GuiNum;
        }
        switch (GuiNum)
		{
			case GuiTempHum:
				OledGui_Temp_Hum();		//温湿度界面
				break;
			case GuiTime:
				OledGui_Time();			//时钟界面
				break;
			case GuiWeather:
				OledGui_Weather();		//天气界面
				break;
		}
        OLED_Refresh();
	    vTaskDelay(50/ portTICK_RATE_MS);

	}
}








// 按下按键即会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("Time button: ", state);
    GuiNum = GuiTime;
}

// 按下按键即会执行该函数
void button2_callback(const String & state) {
    BLINKER_LOG("Temp&RH button state: ", state);
    GuiNum = GuiTempHum;
}

// 按下按键即会执行该函数
void button3_callback(const String & state) {
    BLINKER_LOG("Weather button state: ", state);
    GuiNum = GuiWeather;
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    
}


/**
 * @brief 小爱同学
 * 
 * @param queryCode 
 */
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.temp(DHT20_1.Temperature);
            BlinkerMIOT.humi(DHT20_1.Humidity);
            BlinkerMIOT.co2(SGP30_1.eCO2);
            BlinkerMIOT.print();
            GuiNum = GuiTempHum;
            break;
        default :
            BlinkerMIOT.temp(20);
            BlinkerMIOT.humi(20);
            break;
    }
}



void heartbeat()
{
    HUMI.print(DHT20_1.Humidity);
    TEMP.print(DHT20_1.Temperature);
    CO2D.print(SGP30_1.eCO2);
    #ifdef GET_O2_FUNCTION
    O2D.print(MIX8410_1.O2results);
    #endif
}

void dataStorage()
{
    Blinker.dataStorage("temp", DHT20_1.Temperature);
    Blinker.dataStorage("humi", DHT20_1.Humidity);
    #ifdef GET_O2_FUNCTION
    Blinker.dataStorage("O2", MIX8410_1.O2results);
    #endif
}


void setup() {
  	
    
    Serial.begin(115200);
    
    /*注册重配网回调函数BOOT*/
    #ifdef Button_Interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN,Key_Detectuin,FALLING);      //外部中断引脚设置为D3，下降沿触发，中断服务函数为 Key_Detectuin
    #else
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    #endif


    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif


    #ifdef SmartConfig
        Blinker.begin(auth);
    #else
        Blinker.begin(auth, ssid, pswd);
    #endif 
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    Blinker.attachDataStorage(dataStorage);
    BlinkerMIOT.attachQuery(miotQuery); /*小爱同学*/

    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);

 	
    //心知天气库
    #ifdef Weather_Library
    forecast.config(reqUserKey, reqLocation, reqUnit);      //心知天气对象初始化
    // Get_WeatherForecast();        //获取天气
    #endif
    /*创建任务*/



                    
    xTaskCreate(Set_Time,
                "Set_Time",
                1024*2,
                NULL,
                5,
                NULL);


    xTaskCreate(OLED_Task,
                "OLED_Task",
                1024*2,
                NULL,
                4,
                NULL);

    #ifndef Button_Interrupt
    xTaskCreate(Button_Task,
                "Button_Task",
                1024*2,
                NULL,
                12,
                NULL); 
    #endif       

    xTaskCreate(PCF8563_Task,
                "PCF8563_Task",
                1024*2,
                NULL,
                13,
                NULL);

    xTaskCreate(DHT_Task,
                "DHT_Task",
                1024*2,
                NULL,
                14,
                NULL);

    xTaskCreate(SGP30_Task,
                "SGP30_Task",
                1024*2,
                NULL,
                15,
                NULL);

    #ifdef GET_O2_FUNCTION
    xTaskCreate(MIX8410_Task,
                "MIX8410_Task",
                1024*2,
                NULL,
                16,
                NULL);
    #endif

    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    #ifdef AliyunIOT
        AliyunIoTSDK::begin(espClient, 
                            PRODUCT_KEY, 
                            DEVICE_NAME, 
                            DEVICE_SECRET, 
                            REGION_ID);
    #endif
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id


    // 发送一个数据到云平台，PowerSwitch 是在设备产品中定义的物联网模型的 id
    
    
}
 
void loop() {
    #ifdef AliyunIOT
        AliyunIoTSDK::send("CurrentTemperature", DHT20_1.Temperature);
        AliyunIoTSDK::send("CurrentHumidity", DHT20_1.Humidity);
        AliyunIoTSDK::send("co2", SGP30_1.eCO2);
        delay(1000);
    #endif
  	Blinker.run();
}
