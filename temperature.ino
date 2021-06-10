#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"



Adafruit_AM2320 am2320 = Adafruit_AM2320();

const char* ssid = "IT Services_2G";
const char* password = "labadmin";
//Your Domain name with URL path or IP address with path
String serverName = "http://smartlab.psu.ac.th:8080/update";
String postMessage;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
      
void setup() {
  Serial.begin(9600);     
  Serial.println("Adafruit AM2320 Basic Test");
  am2320.begin();
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

    
          
      HTTPClient http;
      WiFiClient wc;
      
      
       
     
      Serial.print("Temp: "); Serial.println(am2320.readTemperature());
      Serial.print("Hum: "); Serial.println(am2320.readHumidity());
      
      float t = am2320.readTemperature();
      float h = am2320.readHumidity();


      
  
      const int CAPACITY = JSON_OBJECT_SIZE(6);
      StaticJsonDocument<CAPACITY> doc;
      doc["device_id"] = "device1_temp";
      doc["temp"] = t;
      doc["humid"] = h;
      

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
  
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
     // http.end();
     }else {
      Serial.println("WiFi Disconnected");
    }
  
}
