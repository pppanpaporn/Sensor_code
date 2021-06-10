#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "PMS.h"


PMS pms(Serial);
PMS::DATA data;

const char* ssid = "IT Services_2G";
const char* password = "labadmin";

//Your Domain name with URL path or IP address with path
String serverName = "http://smartlab.psu.ac.th:8080/update";
String postMessage;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.

void setup() {
  Serial.begin(9600);     
  pms.passiveMode();  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  
}

void loop() {
  
  
     if(WiFi.status() == WL_CONNECTED){
      Serial.println("Waking up, wait 30 seconds for stable readings...");
      pms.wakeUp();
      delay(5000);
    
      Serial.println("Send read request...");
      pms.requestRead();
    
      Serial.println("Reading data...");
      if (pms.readUntil(data))
      {
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_1_0);
    
        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(data.PM_AE_UG_2_5);
    
        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_10_0);
      }
      else
      {
        Serial.println("No data.");
      }
    
          
      HTTPClient http;
      WiFiClient wc;
      
      
      float pm1 = data.PM_AE_UG_1_0;
      float pm25 = data.PM_AE_UG_2_5;
      float pm10 = data.PM_AE_UG_10_0;
      
      const int CAPACITY = JSON_OBJECT_SIZE(6);
      StaticJsonDocument<CAPACITY> doc;
      doc["device_id"] = "device4_pm";
      doc["pm_1"] = pm1;
      doc["pm_25"] = pm25;
      doc["pm_10"] = pm10;

      serializeJson(doc, postMessage);
      serializeJson(doc, Serial);
  
      // Your Domain name with URL path or IP address with path
      http.begin(wc,serverName);
      //http.setTimeout(600);
//      // Specify content-type header
//      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//      // Data to send with HTTP POST
//      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
//      // Send HTTP POST request
//      int httpResponseCode = http.POST(httpRequestData);
      
       //If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
     // http.addHeader("Content-Length", String(JSON.length()));
   
    // serializeJsonPretty(doc,Serial);
      int httpResponseCode = http.POST(postMessage);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     Serial.println("Going to pm sleep. ");
      pms.sleep();
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
     // http.end();
     }else {
      Serial.println("WiFi Disconnected");
    }
    delay(10000);
}
