#ifndef ConnectionConfig_h
#define ConnectionConfig_h

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <SPI.h>
#include <ESPmDNS.h>
#include <DNSServer.h>


#define led 4

extern const char* ssidAP;
extern const char* passwordAP;

extern String ssid;
extern String password;

extern AsyncWebServer server;
extern AsyncWebSocket webSocket;
extern DNSServer dnsServer;

extern IPAddress localIP;
extern IPAddress Gateway;
extern IPAddress subnet;
extern IPAddress dns1;       //Optional
extern IPAddress dns2;       //Optional


void beginConnection();
void requestServer();
void connectToWiFi();
void SDServerRequest(const char* address, const char* location, const char* fileType);
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void attempsConnection();

#endif