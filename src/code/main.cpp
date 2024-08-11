#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <SPI.h>
#include <ESPmDNS.h>

const char* ssidAP = "ESP32";
const char* passwordAP= "ESP32RAINUI";

String ssid;
String password;

void connectToWiFi();
void SDServerRequest(const char* address, const char* location, const char* fileType);


AsyncWebServer server(80);

  IPAddress localIP(192,168,1,1);
  IPAddress Gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  IPAddress dns1(1,1,1,1);       //Optional
  IPAddress dns2(1,0,0,2);       //Optional

void setup(){
  Serial.begin(115200);          //You can change it to 9600 to more cleaner serial monitor
  WiFi.softAP(ssidAP,passwordAP);
  
  WiFi.softAPConfig(localIP, Gateway, subnet);

  if (!MDNS.begin("rainhome"))
  {
    Serial.println("can't start mDNS");
  }
  

  if(!SD.begin()){
    Serial.println("SD card is missing");
  }
    Serial.println("SD card is attach");

  /* ---------------------------------- HTML ---------------------------------- */
  SDServerRequest("/", "/page/index.html", "text/html");
  SDServerRequest("/setwifi", "/page/WifiConfig.html", "text/html");

  /* ----------------------------------- CSS ---------------------------------- */
  SDServerRequest("/styles/style.css", "/styles/style.css", "text/css");

  /* ------------------------ Other webserver handealer ------------------------ */
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("ssid")) {
      ssid = request->getParam("ssid")->value();
    }
    if (request->hasParam("password")) {
      password = request->getParam("password")->value();
    }

  if (ssid.isEmpty() || password.isEmpty()) {
    Serial.println("SSID or Password missing!");
    request->send(SD ,"/page/WifiConfig.html", "text/html");
    Serial.println("Back To Wifi Set");
  } else {

    connectToWiFi();

    if (WiFi.status() ==  WL_CONNECTED){
      request->send(SD ,"/page/WifiConnected.html", "text/html");
      } else {
      request->send(SD ,"/page/WifiDisconnected.html", "text/html");
    }
    
  }
  });
  
  //start the server
  server.begin();
}

void loop(){

}

/* -------------------------------- Function -------------------------------- */

void connectToWiFi() {

  if (ssid.isEmpty() || password.isEmpty()) {
    Serial.println("SSID or Password missing!");
    return;
  }

  WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected to Wi-Fi");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Failed to connect to Wi-Fi"); 
  }
}

void SDServerRequest(const char* address, const char *location, const char* fileType){
  server.on(address, HTTP_GET , [location, fileType](AsyncWebServerRequest* request){
    request->send(SD , location, fileType);
  });
}