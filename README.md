# arduino-teleinfo
teleinfo library for arduino 

For french teleinfo system (ERDF)

WARN: it requires "a lot" of memory (about 600 bytes) and may be configured in Teleinfo.h for bigger frame size (other contracts). 
Nano and Uno has 2kB of memory. Arduino Mega 2560 has 8kB.
Note: Arduino mega that has several hardware serial and bigger memory, it's nice for this usage.

##Note

In setup, you have to call:

    serial.begin(1200)

Initialization for software serial:

    #include <SoftwareSerial.h>
    SoftwareSerial serial(2,3);
    TeleInfo teleinfo(&serial);

Initialization for hardware serial:
    TeleInfo teleinfo(&Serial);

## Example: 

    #include <TeleInfo.h>
    #include <SoftwareSerial.h>
    
    
    const int rxPin = 2;
    const int txPin = 3;
    
    SoftwareSerial serial(rxPin,txPin);
    TeleInfo teleinfo(&serial);
    //TeleInfo teleinfo(&Serial2);

    void setup(){
      Serial.begin(9600);
      serial.begin(1200);
      //Serial2.begin(1200);
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
    
## output example

    --- tele info available --- 
    ADCO => 012345678901
    OPTARIF => HC..
    ISOUSC => 45
    HCHC => 043208559
    HCHP => 059288067
    PTEC => HP..
    IINST => 003
    IMAX => 044
    PAPP => 00680
    HHPHC => A
    MOTDETAT => 400000
    ------ 
    Option Tarifaire = HC..
    Power = 680
