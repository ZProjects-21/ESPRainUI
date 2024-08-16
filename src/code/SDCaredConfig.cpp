#include "SDCardConfig.h"

void beginSDCard(){
 if(!SD.begin()){
    Serial.println("SD card is missing");
  }
    Serial.println("SD card is attach");
}

void cardSize() {
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %llu MB\n", cardSize);
}

String getSDCardSize() {
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  return String(cardSize) + " MB";
}




/**  
  
 


  ******          ******
 ***                  ***
    ****          ****   
  ********      ******** 
  ********      ******** 
    ****          ****   
           ****          
            **          
  ****              ****
   *****          *****
    ******************
       *************   




    I present all of this just to you 
    
    uwu


  **/