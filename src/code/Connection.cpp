#include "ConnectionConfig.h"
#include "SDCardConfig.h"

const char* ssidAP = "ESP32";
const char* passwordAP = "ESP32RAINUI";

String ssid;
String password;

AsyncWebServer server(80);
AsyncWebSocket webSocket("/websocket");
DNSServer dnsServer;

IPAddress localIP(192,168,1,1);
IPAddress Gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(1,1,1,1);       //Optional
IPAddress dns2(1,0,0,2);       //Optional


void beginConnection(){
  WiFi.softAP(ssidAP,passwordAP);
  WiFi.softAPConfig(localIP, Gateway, subnet);
  WiFi.mode(WIFI_AP_STA);
  dnsServer.start(53, "*", localIP);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  if (!MDNS.begin("Rainhome"))
  {
    Serial.println("can't start mDNS");
  }

  webSocket.onEvent(onWebSocketEvent);
  server.addHandler(&webSocket); 
}

/* -------------------------- Setup Server Handler -------------------------- */

void requestServer(){

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

  server.begin();
}

void connectToWiFi() {

  if (ssid.isEmpty() || password.isEmpty()) {
    Serial.println("SSID or Password missing!");
    return;
  }

  WiFi.begin(ssid.c_str(), password.c_str());
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
}

void SDServerRequest(const char* address, const char *location, const char* fileType){
  server.on(address, HTTP_GET , [location, fileType](AsyncWebServerRequest* request){
    request->send(SD , location, fileType);
  });
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client connected\n");
    client->text(getSDCardSize());
  } 
}

void attempsConnection(){
  webSocket.cleanupClients();
  int attemps = 0;
  int max_attemps = 30;
  while (WiFi.status() == WL_DISCONNECTED && ssid != "" && password != ""  &&  attemps < max_attemps)
  {
    connectToWiFi();
    Serial.println(attemps);
    attemps++;
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(led, HIGH);
  }  
  if(attemps >= max_attemps){
    ssid = "";
    password = "";
  }
  
  delay(500);
  
  yield();
}