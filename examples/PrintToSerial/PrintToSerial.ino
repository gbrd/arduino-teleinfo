#include <TeleInfo.h>

#include <SoftwareSerial.h>

const int rxPin = 2;
const int txPin = 3;

SoftwareSerial serial(2,3);
TeleInfo teleinfo(&serial);

void setup(){
  serial.begin(1200);
  Serial.begin(9600);
  teleinfo.begin();
}
void loop(){
    teleinfo.process();
  if(teleinfo.available()){
    teleinfo.resetAvailable();
    teleinfo.printAllToSerial();
  }    
}
