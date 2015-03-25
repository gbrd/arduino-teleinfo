#include "Arduino.h"
#include "TeleInfo.h"


TeleInfo::TeleInfo(Stream* serial)
//TeleInfo::TeleInfo(uint8_t rxPin,uint8_t txPin)
{
  //_rxPin = rxPin;
  //_txPin = txPin;
  //this->_cptSerial = new SoftwareSerial(2,3);
  this->_cptSerial = serial;
  
  _checksum[0] = '\0';
  //Ligne[0] = '\0';
  Etiquette[0] = '\0';
  Donnee[0] = '\0';
  _trame[0] = '\0';
  
  MOTDETAT[0] = '\0';
  
}
void TeleInfo::resetAll(){
  // vider les infos de la dernière trame lue 

  memset(_trame,'\0',512);
  memset(ADCO,'\0',12);
  HCHC = 0;
  HCHP = 0;
  memset(PTEC,'\0',4);
  memset(HHPHC,'\0',2);
  IINST = 0;
  PAPP = 0;
  IMAX = 0;
  memset(OPTARIF,'\0',4);
  memset(MOTDETAT,'\0',10);
  ISOUSC = 0;
}



void TeleInfo::begin()
{
  //_cptSerial->begin(1200);
  resetAll();
  
  //if (cptSerial.available()){
  //  Serial.write(cptSerial.read() & 0x7F);
  //}
  
  
}

/*------------------------------------------------------------------------------*/
/* Test checksum d'un message (Return 1 si checkum ok)            */
/*------------------------------------------------------------------------------*/
boolean TeleInfo::checksum_ok(char *etiquette, char *valeur, char checksum) 
{
   unsigned char sum = 32 ;      // Somme des codes ASCII du message + un espace
   int i ;
 
   for (i=0; i < strlen(etiquette); i++) sum = sum + etiquette[i] ;
   for (i=0; i < strlen(valeur); i++) sum = sum + valeur[i] ;
   sum = (sum & 63) + 32 ;
   /*
   Serial.print(etiquette);Serial.print(" ");
   Serial.print(valeur);Serial.print(" ");
   Serial.println(checksum);
   Serial.print("Sum = "); Serial.println(sum);
   Serial.print("Cheksum = "); Serial.println(int(checksum));
   */
   return ( sum == checksum);
}


int TeleInfo::lireEtiquette(char *ligne){
    int i;
    int j=0;
    memset(Etiquette,'\0',9);
    for (i=1; i < strlen(ligne); i++){ 
      if (ligne[i] != 0x20) { // Tant qu'on est pas au SP, c'est qu'on est sur l'étiquette
        Etiquette[j++]=ligne[i];
      }else { //On vient de finir de lire une etiquette
        //  Serial.print("Etiquette : ");
        //  Serial.println(Etiquette);
        return j+2; // on est sur le dernier caractère de l'etiquette, il faut passer l'espace aussi (donc +2) pour arriver à la valeur
      }
   }
}


int TeleInfo::lireValeur(char *ligne, int offset){
    int i;
    int j=0;
    memset(Donnee,'\0',13);
    for (i=offset; i < strlen(ligne); i++){ 
      if (ligne[i] != 0x20) { // Tant qu'on est pas au SP, c'est qu'on est sur l'étiquette
          Donnee[j++]=ligne[i];
      }else { //On vient de finir de lire une etiquette
        //  Serial.print("Valeur : ");
        //  Serial.println(Donnee);
        return j+2; // on est sur le dernier caractère de la valeur, il faut passer l'espace aussi (donc +2) pour arriver à la valeur
      }
   }
}

void TeleInfo::lireChecksum(char *ligne, int offset){
  int i;
  int j=0;
  memset(_checksum,'\0',32);
  for (i=offset; i < strlen(ligne); i++){ 
        _checksum[j++]=ligne[i];
     //  Serial.print("Chekcsum : ");
     //  Serial.println(_checksum);
  }
}


boolean TeleInfo::affecteEtiquette(char *etiquette, char *valeur){

 if(strcmp(etiquette,"ADCO") == 0) { 
   memset(ADCO,'\0',12); memcpy(ADCO, valeur,strlen(valeur));
   Serial.print("ADCO="); Serial.println(ADCO);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(etiquette,"HCHC") == 0) { HCHC = atol(valeur); 
   Serial.print("HCHC="); Serial.println(HCHC);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(etiquette,"HCHP") == 0) { HCHP = atol(valeur); 
   Serial.print("HCHP="); Serial.println(HCHP);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(etiquette,"HHPHC") == 0) { 
   memset(HHPHC,'\0',2); strcpy(HHPHC, valeur);
   Serial.print("HHPHC="); Serial.println(HHPHC);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(etiquette,"PTEC") == 0) { memset(PTEC,'\0',4); memcpy(PTEC, valeur,strlen(valeur)); 
   Serial.print("PTEC="); Serial.println(PTEC);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(Etiquette,"IINST") == 0) { IINST = atoi(valeur);
   Serial.print("IINST="); Serial.println(IINST);
   Serial.print("valeur="); Serial.println(valeur);
 }
 else
 if(strcmp(Etiquette,"PAPP") == 0) { PAPP = atol(valeur);
   Serial.print("PAPP="); Serial.println(PAPP);
   Serial.print("valeur="); Serial.println(valeur); 
 }
 else
 if(strcmp(Etiquette,"IMAX") == 0) { IMAX = atol(valeur);
   Serial.print("IMAX="); Serial.println(IMAX);
   Serial.print("valeur="); Serial.println(valeur); 
 }
 else
 if(strcmp(Etiquette,"OPTARIF") == 0) { memset(OPTARIF,'\0',4); memcpy(OPTARIF, valeur,strlen(valeur));
   Serial.print("OPTARIF="); Serial.println(OPTARIF);
   Serial.print("valeur="); Serial.println(valeur); 
 }
 else
 if(strcmp(Etiquette,"ISOUSC") == 0) { ISOUSC = atoi(valeur);
   Serial.print("ISOUSC="); Serial.println(ISOUSC);
   Serial.print("valeur="); Serial.println(valeur);  
 }
 else
 if(strcmp(Etiquette,"MOTDETAT") == 0) { memset(MOTDETAT,'\0',10); memcpy(MOTDETAT, valeur, strlen(valeur));
   Serial.print("MOTDETAT="); Serial.println(MOTDETAT);
   Serial.print("valeur="); Serial.println(valeur);  
 }
 else
 return false;

 return true;
}


boolean TeleInfo::decodeLigne(char *ligne){ 
  
  //_checksum='\0';
  int debutValeur; 
  int debutChecksum;
  // Décomposer en fonction pour lire l'étiquette etc ...  
  debutValeur=lireEtiquette(ligne);
  debutChecksum=lireValeur(ligne, debutValeur);
  lireChecksum(ligne,debutValeur + debutChecksum -1);


  // !!!!!! TODO check why we use _checksum[0] here !!!!
  if (checksum_ok(Etiquette, Donnee, _checksum[0])){ // si la ligne est correcte (checksum ok) on affecte la valeur à l'étiquette
    return affecteEtiquette(Etiquette,Donnee);
  } 
  else return false;

}


boolean TeleInfo::lireTrame(){
    int i;
    int j=0;
    char ligne[32] = "";
    boolean trameOk = true;
    memset(ligne,'\0',32); //really useful ?
    //start i at 1 to skip first char 0x02 (start byte)
    for (i=1; i < strlen(_trame); i++){
      if (_trame[i] != 0x0D) { // Tant qu'on est pas au CR, c'est qu'on est sur une ligne du groupe
          ligne[j++]=_trame[i];
      }else { //On vient de finir de lire une ligne, on la décode (récupération de l'etiquette + valeur + controle checksum
          boolean ligneOk = decodeLigne(ligne);
          trameOk = trameOk && ligneOk;
          //memset(ligne,'\0',32); // on vide la ligne pour la lecture suivante
          j=0;
      }
   }
   
   return trameOk;
}



boolean TeleInfo::available(){
  return _isAvailable;
}




void TeleInfo::process(){
  char caractereRecu ='\0';
  while (_cptSerial->available()) {
    caractereRecu = _cptSerial->read() & 0x7F;
    //"Start Text 002" debut de la trame
    if(caractereRecu == 0x02){
      _trameIndex = 0;
    }
    _trame[_trameIndex] = caractereRecu;
    _trameIndex++;
    
    //  "EndText 003" Fin de trame
    if(caractereRecu == 0x03){
      _trame[_trameIndex]='\0';
      _trameIndex++;
      _isAvailable = lireTrame();
    }
  }
}

void TeleInfo::resetAvailable(){
  _isAvailable = false;
}

/***********************************************
   getTeleinfo
   Decode Teleinfo from serial
   Input : n/a
   Output : n/a
***********************************************/
/*
void Teleinfo::getTeleinfo() {
  
  int i = 0;

  resetAll();
  int trameComplete=0;
  char CaractereRecu ='\0';

  while (!trameComplete){
    while(CaractereRecu != 0x02) // boucle jusqu'a "Start Text 002" début de la trame
    {
       if (_cptSerial->available()) {
         CaractereRecu = _cptSerial->read() & 0x7F;
       }
    }

    i=0; 
    while(CaractereRecu != 0x03) // || !trame_ok ) // Tant qu'on est pas arrivé à "EndText 003" Fin de trame ou que la trame est incomplète
    { 
      if (_cptSerial->available()) {
        CaractereRecu = _cptSerial->read() & 0x7F;
        _trame[i++]=CaractereRecu;
      }   
    }
    //finTrame = i;
    _trame[i++]='\0';

    Serial.println(_trame);
  
    lireTrame();


  }
}

*/



