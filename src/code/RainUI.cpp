#include "RainUI.h"
#include "SDCardConfig.h"
#include "ConnectionConfig.h"

/* ------------------------------ Config Setup ------------------------------ */
void configSetup(){
  Serial.begin(115200);
  beginSDCard();
  beginConnection();
  requestServer();
}

/* ------------------------------- Config loop ------------------------------ */
void configLoop(){
  attempsConnection();
}




