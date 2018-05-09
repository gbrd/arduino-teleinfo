#include <TeleInfo.h>
#include <SoftwareSerial.h>

SoftwareSerial serial(2,3);
TeleInfo teleinfo(&serial);


void setup(){
  Serial.begin(9600);
  serial.begin(1200);
  teleinfo.begin();
  //teleinfo.setDebug(true);
}
void loop(){
  teleinfo.process();
  if(teleinfo.available()){
    Serial.println("--- tele info available --- ");
    teleinfo.printAllToSerial();
    Serial.println("------ ");
    
    //const char *periodTarif = teleinfo.getStringVal("PTECZ");
    //Serial.print("Period Tarifaire = ");
    //periodTarif == NULL ? Serial.println("unknown") : Serial.println(periodTarif);
    
    const char *opTarif = teleinfo.getStringVal("OPTARIF");
    Serial.print("Option Tarifaire = ");
    opTarif == NULL ? Serial.println("unknown") : Serial.println(opTarif);
    
    long power = teleinfo.getLongVal("PAPP");
    Serial.print("Power = ");
    power < 0 ? Serial.println("unknown") : Serial.println(power);
    
    teleinfo.resetAvailable();
  }    
}
