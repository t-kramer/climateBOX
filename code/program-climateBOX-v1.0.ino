
/* climateBOX - Indoor climate monitoring station


-- NTP-Version (WiFi connection required) --

 This program was developed for the use on ESP32 microcontrollers (DOIT ESP32 DEVKIT V1).
 The following components are used:

  a) DHT-22                   Air temperature, relative humidity
  b) DS18B20                  Globe temperature
  c) Modern Device revC       Air velocity
  d) SD card module           -
  e) SSD1306                  0.96" OLED display


More information about the climateBOX can be found here:

- Publication:
Kramer, T., Garcia-Hansen, V., & Omrani, S. (2023). 
climateBOX: a low-cost and open-source monitoring device for personal thermal comfort evaluation. 
Energy and Buildings, 112830.

- GitHub page:
https://github.com/t-kramer/climateBOX


The code is provided by:

School of Architecture & Built Environment,
Queensland University of Technology

Contact:
t.kramer@qut.edu.au

 
 License:                       */

//______________________________________LIBRARIES______________________________________//

#include <Arduino.h>
#include "DHT.h"
#include "DHT_U.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "PubSubClient.h"
#include "WiFi.h"
#include "time.h"
#include <ESP32Time.h>

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/



//____________________________________USER CONFIGS____________________________________//

const char* boxID = "box2";                                 // climateBOX identifier*
const double diameter = 0.04;                               // Ping-pong ball diameter
const double emissivity = 0.95;                             // depending on ping-pong ball paint
float zeroWindAdjustment = 0;                            // revC calibration variable*


// Calibration coefficients

const double C_A_velo = 0.5588;
const double C_B_velo = -1.8402;
const double C_C_velo = 1.6074;

const double C_A_temp = 1.1423;
const double C_B_temp = 8.2098;
const double C_C_temp = -15.8171;

const double C_A_humi = 0.9388;
const double C_B_humi = -0.2178;
const double C_C_humi = 14.3411;

const double C_A_globe = 1.0185;
const double C_B_globe = -0.2829;


// WiFi & MQTT
  
const char* ssid = "PiLan";                                 // network SSID*
const char* wifi_password = "raspberry007";                 // network password*

const char* mqtt_server = "192.168.66.1";                   // IP of the MQTT broker*
const char* humidity_topic = "system/box2/humi";            // MQTT topics*
const char* temperature_topic = "system/box2/temp";         //
const char* globe_topic = "system/box2/glob";               //
const char* velocity_topic = "system/box2/velo";            //
const char* mqtt_username = "tobi";                         // MQTT username*
const char* mqtt_password = "tobi";                         // MQTT password*
const char* clientID = "client_box2";                       // MQTT client ID*

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 32400;                           // Brisbane, Australia*
const int daylightOffset_sec = 3600;

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/



//______________________________STATIC COMPONENT CONFIGS_____________________________//

#define DHTPIN 4                                                // DHT-22
#define DHTTYPE DHT22                                           // (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define oneWireBus 15                                           // DS18B20
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#define analogPinForRV    34                                    // revC air anemometer  
#define analogPinForTMP   35                                    // revC thermistor

#define SCREEN_WIDTH 128                                        // OLED display width, in pixels
#define SCREEN_HEIGHT 64                                        // OLED display height, in pixels
#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SD_CS 5                                                 // SD module


WiFiClient wifiClient;                                          // Initialise the WiFi and MQTT Client objects

PubSubClient client(mqtt_server, 1883, wifiClient);             // 1883 is the listener port for the Broker



ESP32Time rtc(32400);                                           // offset in seconds GMT+9



//__________________________________DYNAMIC CONFIGS__________________________________//

double tempReading = 0;
float rawTemp;

double humReading = 0;

double globeReading = 0;

double veloReading = 0;
float tmpRaw;
float tmpRawScale;
float rvRaw;
float rvRawScale;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  180       /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR unsigned long rememberTime;

String timer;

void print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0 :
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_EXT1 :
            Serial.println("Wakeup caused by external signal using RTC_CNTL");
            break;
        case ESP_SLEEP_WAKEUP_TIMER :
            Serial.println("Wakeup caused by timer");
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD :
            Serial.println("Wakeup caused by touchpad");
            break;
        case ESP_SLEEP_WAKEUP_ULP :
            Serial.println("Wakeup caused by ULP program");
            break;
        default :
            Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
            break;
    }
}

//int data_count = 0;
int wificounter = 0;




// 'climateBOX symbol', 128x64px
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x06, 0x00, 0x38, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0x00, 0x3c, 0x00, 0x00, 0x1d, 0x06, 0x06, 0x0d, 0x30, 0xe0, 0x40, 0x1c, 0x00, 
  0x00, 0x00, 0x07, 0x00, 0x3c, 0x00, 0x00, 0x37, 0x06, 0x03, 0x06, 0xd1, 0x91, 0xfc, 0x63, 0x00, 
  0x00, 0x00, 0x07, 0x00, 0x30, 0x00, 0x00, 0x41, 0x06, 0x01, 0x04, 0x90, 0x18, 0x40, 0x41, 0x00, 
  0x00, 0x00, 0x0e, 0xfc, 0x30, 0x00, 0x00, 0x40, 0x06, 0x01, 0x04, 0x91, 0xf8, 0x40, 0x7f, 0x00, 
  0x00, 0x00, 0x1c, 0xff, 0xf3, 0x00, 0x00, 0x40, 0x06, 0x01, 0x04, 0x92, 0x18, 0x40, 0x40, 0x00, 
  0x00, 0x00, 0x7d, 0xff, 0xf3, 0xe0, 0x00, 0x63, 0x06, 0x01, 0x04, 0x9b, 0x38, 0x66, 0x63, 0x00, 
  0x00, 0x01, 0xff, 0xf7, 0xf3, 0xc0, 0x00, 0x1c, 0x1f, 0x8f, 0xee, 0xc9, 0xcc, 0x3c, 0x3e, 0x00, 
  0x00, 0x03, 0xff, 0xff, 0xe7, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xf7, 0xff, 0xff, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xff, 0xcf, 0xff, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xff, 0xff, 0x3c, 0x40, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x02, 0x83, 0xff, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xfc, 0x0f, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xff, 0xe0, 0x31, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xc1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xf0, 0x0f, 0xf1, 0x80, 0x00, 0x00, 0x00, 0xff, 0x03, 0xe1, 0xef, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xff, 0x80, 0x31, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x87, 0xf1, 0xef, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x07, 0xfe, 0x01, 0x80, 0x00, 0x00, 0x00, 0x31, 0xce, 0x38, 0x6e, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xc0, 0x3f, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x71, 0x8c, 0x18, 0x7c, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x01, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x9c, 0x1c, 0x38, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x0f, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x71, 0xdc, 0x1c, 0x3c, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xc0, 0x7f, 0x83, 0x80, 0x00, 0x00, 0x00, 0x30, 0xcc, 0x18, 0x6c, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x07, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x71, 0xce, 0x38, 0xc6, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x07, 0xe0, 0x21, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc7, 0xf1, 0xef, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x70, 0x7e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x03, 0xe1, 0xe7, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x06, 0x07, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};







//__________________________________SAVING FUNCTION_________________________________//
void storeDataToSDCard(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending data to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.println(message)) {
    Serial.println("Data appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}





//__________________________________MQTT FUNCTION_________________________________//

void connect_MQTT(){
  Serial.print("Connecting to ");
  display.clearDisplay();
  display.setTextSize(1.25);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Connecting to ");
  display.display();
  Serial.println(ssid);
  display.println(ssid);
  display.display();

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while ((WiFi.status() != WL_CONNECTED) && (wificounter <= 30)) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
    wificounter++;
    if (wificounter == 30) {
      Serial.println(".");
      display.println(".");
      Serial.println("No WiFi available.");
      display.println("No WiFi available.");
      display.display();      
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
  // Debugging - Output the IP Address of the ESP
    Serial.println("WiFi connected.");
    display.println("WiFi connected.");
    Serial.print("IP address: ");
    display.print("IP : ");
    Serial.println(WiFi.localIP());
    display.println(WiFi.localIP());
    display.display();
  }
  
  delay(1000);

  // Connect to MQTT Broker

  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
    display.println("Connected to MQTT Broker!");
    display.display();
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
    display.println("Connection to MQTT Broker failed...");
    display.display();
  }

  delay(1000); 
  
}


//__________________________________READING FUNCTION_________________________________//

void recordNewData() {

  
// Air velocity

  tmpRaw = analogRead(analogPinForTMP);
  rvRaw = analogRead(analogPinForRV);


  for (int i = 0; i < 9; i++) {
    rvRaw += analogRead(analogPinForRV);
    tmpRaw += analogRead(analogPinForTMP);
    delay(i*1000);
  }


  rvRaw /= 10;
  tmpRaw /= 10;

  tmpRawScale = tmpRaw / 1000;
  rvRawScale = rvRaw / 1000;


  veloReading = C_A_velo*pow(rvRawScale, 3) + (C_B_velo * tmpRawScale) + C_C_velo;

  veloReading = veloReading + zeroWindAdjustment; // adjust zero windspeed

  if (veloReading < 0.00) {
    veloReading = 0;
  }



// Air temperature

  rawTemp = dht.readTemperature();

  tempReading = C_A_temp * rawTemp + C_B_temp * rvRawScale + C_C_temp;



// Relative humidity

  humReading = dht.readHumidity();

  humReading = C_A_humi * humReading + C_B_humi * rawTemp + C_C_humi;



// Globe temperature
  
  sensors.requestTemperatures();
  globeReading = sensors.getTempCByIndex(0);

  globeReading = C_A_globe * globeReading + C_B_globe;




  connect_MQTT();


  delay(1000);


  configTime(0, daylightOffset_sec, ntpServer);             // get time from NTP server


  delay(1000);


  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo);
    timer = rtc.getTime("%d/%m/%y %H:%M");
//    rememberTime = rtc.getEpoch();
  }
  else {  
    Serial.println("Failed to obtain time");
//    rtc.setTime(rememberTime);
    timer = "No Time";
//    return;
  }



  delay(1000);
  
  // Printing in Serial Monitor
//  Serial.print(&timeinfo, "%d/%m/%y %H:%M ");
  Serial.print(timer); 
  Serial.print(F(" Humidity: "));
  Serial.print(humReading);
  Serial.print(F("%  Air Temperature: "));
  Serial.print(tempReading);
  Serial.print(F("°C  Globe:  "));
  Serial.print(globeReading);
  Serial.print(F("°C  Velocity:  "));
  Serial.print(veloReading);
  Serial.print(F("m/s  RawRVScale:  "));
  Serial.print(rvRawScale,3);
  Serial.print(F("  RawTMPScale:  "));
  Serial.println(tmpRawScale,3);

  // Printing on OLED Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("I am " + String(boxID));
  display.setTextSize(1);
  display.println("");
  display.setTextSize(1.75);
  display.print("Temperature:  ");
  display.print((float)tempReading);
  display.println(" C");
  display.print("Rel.Humidity: ");
  display.print((float)humReading);
  display.println(" %");
  display.print("Globe:        ");
  display.print((float)globeReading);
  display.println(" C");
  display.print("Velocity:     ");
  display.print(veloReading);
  display.println("m/s");
  display.display();

//  data_count++; // counter to time WiFi connection attempts
  
  delay(5000);

  // Saving to SD Card
  String message = String(boxID) + "," + (timer) + "," + String(tempReading) + "," + String(humReading) + "," + String(globeReading) + ","  + String(veloReading) + "," + String(rvRawScale) + "," + String(tmpRawScale);
  storeDataToSDCard(SD, "/data.txt", message.c_str());
  

  // Sending via MQTT
  // MQTT can only transmit strings
  String hs="Hum: "+String((float)humReading)+" % ";
  String ts="Temp: "+String((float)tempReading)+" C ";
  String gs="Globe: "+String((float)globeReading)+" C ";
  String vs="Velo: "+String((float)veloReading)+" C ";

  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  client.connect(clientID, mqtt_username, mqtt_password);
  if (client.publish(temperature_topic, String(tempReading).c_str())) {
    Serial.println("Temperature sent!");
    
  }
  
  else {
    Serial.println("Temperature failed to send. Trying again.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(50); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, String(tempReading).c_str());
  }


  // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
  if (client.publish(humidity_topic, String(humReading).c_str())) {
    Serial.println("Humidity sent!");
  }

  else {
    Serial.println("Humidity failed to send. Trying again.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(50); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(humidity_topic, String(humReading).c_str());
  }

  // PUBLISH to the MQTT Broker (topic = Globe, defined at the beginning)
  if (client.publish(globe_topic, String(globeReading).c_str())) {
    Serial.println("Globe temperature sent!");
  }

  else {
    Serial.println("Globe temperature failed to send. Trying again.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(50); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(globe_topic, String(globeReading).c_str());
  }


  // PUBLISH to the MQTT Broker (topic = Velocity, defined at the beginning)
  if (client.publish(velocity_topic, String(veloReading).c_str())) {
    Serial.println("Velocity sent!");
  }

  else {
    Serial.println("Velocity failed to send. Trying again.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(50); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(velocity_topic, String(veloReading).c_str());
  }

  delay(500);
  
  client.disconnect();  // disconnect from the MQTT broker

  delay(500);
  
  WiFi.disconnect();

  delay(500);

  if (WiFi.status() != WL_CONNECTED) {
  // Debugging - Output the IP Address of the ESP
    Serial.println("WiFi disconnected.");
    display.clearDisplay();
    display.println("WiFi disconnected.");
    }

  delay(1000);
  
}



//_______________________________________CHECK WiFi_____________________________________//

//void check_wifi() {
//
//  if ((WiFi.status() != WL_CONNECTED) && (data_count >= 10)) {
//    Serial.println("Searching for WiFi...");
//    display.println("Searching for WiFi...");
//    display.display();
//    wificounter = 0;
//    data_count = 0;
//    
//    connect_MQTT();
//  }
//  
//}



//_______________________________________SETUP______________________________________//

void setup() {
  Serial.begin(9600);

  delay(1000);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  
  print_wakeup_reason();
  
//  connect_MQTT();

  if(!SD.begin(SD_CS)){
    Serial.println("Card Mount Failed");
//    display.println("Card Mount Failed");
//    display.display();
    return;
  } else {
    Serial.println("SD Card mounted with success");
//    display.println("SD Card mounted with success");
//    display.display();
    
  }

  delay(1000);


  dht.begin();                                            //DHT22 initialisation
  sensors.begin();                                        //DS18B20 initialisation

  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE);
  display.display();

////////////////////////////////////////////////////////////////

  recordNewData();

/////////////////////////////////////////////////////////////////

  Serial.println("Going to sleep now.");
  display.println("Going to sleep now.");
  display.display();

  delay(2000);

  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE);
  display.display();
  
  Serial.flush();
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();

}




//_______________________________________LOOP______________________________________//

void loop() {

}
