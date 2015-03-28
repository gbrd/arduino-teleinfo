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
    Serial.println("--- tele info available --- ");
    teleinfo.printAllToSerial();
    Serial.println("------ ");
    const char *periodTarif = teleinfo.getStringVal("PTEC");
    Serial.print("Period Tarifaire = ");
    periodTarif == NULL ? Serial.println("unknown") : Serial.println(periodTarif);
    long power = teleinfo.getLongVal("PAPP");
    Serial.print("Power = ");
    power < 0 ? Serial.println("unknown") : Serial.println(power);
    teleinfo.resetAvailable();
  }    
}