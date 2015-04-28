# arduino-teleinfo
teleinfo library for arduino 

For french teleinfo system (ERDF)


##Example: 
    #include <TeleInfo.h>

    #include <SoftwareSerial.h>

    const int rxPin = 2;
    const int txPin = 3;
    TeleInfo teleinfo(rxPin,txPin);

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

