
/* climateBOX - Indoor climate monitoring station

 
 -- RTC-Version (no internet connection necessary) --

 This program was developed for the use on ESP32 microcontrollers (DOIT ESP32 DEVKIT V1).
 The following components are used:

  a) DHT-22                   Air temperature, relative humidity
  b) DS18B20                  Globe temperature
  c) Modern Device revC       Air velocity
  d) SD card module           -
  e) DS1302                   RTC module


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
#include <FS.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h> 

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/



//____________________________________USER CONFIGS____________________________________//

const char* boxID = "boxID";                                // climateBOX identifier*
const double diameter = 0.04;                               // Ping-pong ball diameter
const double emissivity = 0.95;                             // depending on ping-pong ball paint
float zeroWindAdjustment = 0;                               // revC calibration variable*


// Calibration coefficients

const double C_A_velo = 0.6274;
const double C_B_velo = -1.8484;
const double C_C_velo = 1.6044;

const double C_A_temp = 1.1423;
const double C_B_temp = 8.2097;
const double C_C_temp = -14.6171;

const double C_A_humi = 0.9388;
const double C_B_humi = -0.2178;
const double C_C_humi = 14.3411;

const double C_A_globe = 1.0185;
const double C_B_globe = -0.2829;

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\/



//______________________________STATIC COMPONENT CONFIGS_____________________________//

#define DHTPIN 4                                              // DHT-22
#define DHTTYPE DHT22                                         // (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define oneWireBus 15                                         // DS18B20
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#define analogPinForRV    34                                  // revC air anemometer
#define analogPinForTMP   35                                  // revC thermistor

#define SD_CS 5                                               // SD module

#define SDA 21                                                // RTC SDA
#define SCL 22                                                // RTC SCL

RTC_DS3231 rtc;

File file;


//__________________________________DYNAMIC CONFIGS__________________________________//

double tempReading = 0;
float rawTemp;

double humReading = 0;

double globeReading = 0;

double veloReading = 0;
float tmpRaw;
float rvRaw;
float rvRawScale;
float tmpRawScale;

#define uS_TO_S_FACTOR 1000000                  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  180                      /* Time ESP32 will go to sleep (in seconds) */


RTC_DATA_ATTR long globalCounter = 1;


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



//__________________________________SAVING FUNCTIONS_________________________________//

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}


//__________________________________READING FUNCTION_________________________________//

void recordNewData() {

  String dataString = "";


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

  veloReading = veloReading - zeroWindAdjustment; // adjust zero windspeed

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


// Time

  DateTime now = rtc.now();


  // Printing in Serial Monitor
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(',');
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  //inserts a 0 before minutes from 1-9 so that the time displays correctly, ie 12:05 instead of 12:5 
  if (now.minute()<10) {
    Serial.print('0');
    Serial.print(now.minute(), DEC);
  }
  else {
    Serial.print(now.minute(), DEC);
  }
  Serial.print(',');
  Serial.print(' ');
  Serial.print(F("Humidity: "));
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
  
  dataString += String(boxID);
  dataString += ",";
  dataString += String(now.timestamp());
  dataString += ",";
  dataString += String(tempReading);
  dataString += ",";
  dataString += String(humReading);
  dataString += ",";
  dataString += String(globeReading);
  dataString += ",";
  dataString += String(veloReading);
  dataString += ",";
  dataString += String(rvRawScale,3);
  dataString += ",";
  dataString += String(tmpRawScale,3);
  dataString += "\r\n";

  Serial.println(dataString);
  appendFile(SD, "/datalog_ESP32.txt", dataString.c_str());
    
  delay(2000);

}



//_______________________________________SETUP______________________________________//

void setup() {
  
  Serial.begin(9600);

  delay(1000);

  print_wakeup_reason();

  Wire.begin(SDA,SCL);

  rtc.begin();
  
    
  if (! rtc.begin()) {                                           // Check RTC
   Serial.println("Couldn't find RTC");
   while (1);
   }
  
//  rtc.adjust(DateTime(__DATE__, __TIME__));                     // only activate if you want to reset the RTC


  dht.begin();                                                    //DHT22 initialisation
  sensors.begin();                                                //DS18B20 initialisation


  delay(1000);


  SD.begin(SD_CS);                                                // Initialise SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  delay(1000);

  if (!SD.exists("/datalog_ESP32.txt")) {
    writeFile(SD, "/datalog_ESP32.txt", "BoxID, Time, Air Temperature, Humidity, Globe Temperature, Velocity, VeloRaw, TmpRaw \r\n");
  }
  else {
    Serial.println("File already exists :)");
  }

  delay(1000);

  ///////////////////////////////////

  
  recordNewData();                                    // main recording function is executed


  ////////////////////////////////


  Serial.println("Going to sleep now.");              // deep sleep activation

  delay(2000);
  
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
