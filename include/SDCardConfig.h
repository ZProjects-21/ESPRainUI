#ifndef SDCardConfig_h
#define SDCardConfig_h

#include <SD.h>
#include <Arduino.h>

void beginSDCard();
void cardSize();
String getSDCardSize();

#endif