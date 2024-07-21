#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <SPI.h>

const char* ssidAP = "ESP32";
const char* passwordAP= "test1234";

String wifiSSID;
String wifiPassword;

void connectToWiFi();

AsyncWebServer server(80);

  IPAddress localIP(192,168,1,1);
  IPAddress Gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  // IPAddress dns1(1,1,1,1);
  // IPAddress dns2(1,0,0,2);



void setup(){
  Serial.begin(9600);
  WiFi.disconnect();
  WiFi.softAP(ssidAP,passwordAP);
  
  WiFi.softAPConfig(localIP, Gateway, subnet);

  if(!SD.begin()){
    Serial.println("SD card is missing");
  }
    Serial.println("SD card is attach");

  server.on("/", HTTP_GET , [](AsyncWebServerRequest* request){
    request->send(SD ,"/Page/WifiConfig.html", "text/html");
  });
  server.on("/styles/style.css", HTTP_GET , [](AsyncWebServerRequest* request){
    request->send(SD ,"/styles/style.css", "text/css");
  });

  server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
  int params = request->params();
    for(int i=0; i<params; i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->name() == "ssid"){
        wifiSSID = p->value().c_str();
      }
      if(p->name() == "password"){
        wifiPassword = p->value().c_str();
      }
    }
    
    request->send(SD ,"/Page/index.html", "text/html");

    connectToWiFi();
  });



  server.begin();
}

void loop(){

}

void connectToWiFi() {
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(WiFi.localIP());

}