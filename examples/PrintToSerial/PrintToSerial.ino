#include <TeleInfo.h>


TeleInfo teleinfo(&Serial1);

void setup(){
  Serial.begin(9600);
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
