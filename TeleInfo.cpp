#include "Arduino.h"
#include "TeleInfo.h"


TeleInfo::TeleInfo(HardwareSerial* serial)
{
  this->_cptSerial = serial;
  _frame[0] = '\0'; 
}
/*
TeleInfo::TeleInfo(uint8_t rxPin,uint8_t txPin) : _serial(rxPin,txPin){

  //_cptSerial = &_serial;
  _frame[0] = '\0'; 
}
*/

void TeleInfo::setDebug(boolean debug){
  _isDebug = debug;
}

const char * TeleInfo::getStringVal(const char * label){
  int i = 0;
  char * res = NULL;
  while((strcmp(label,_label[i]) != 0) && i<_dataCount){
    i++;
  }
  if(i >= _dataCount){
    res = NULL;
  }else{
    res = _data[i];
  }
  return res;
}
long TeleInfo::getLongVal(const char * label){
  const char * stringVal = getStringVal(label);
  long res;
  if(stringVal == NULL){
    res = -1;
  }else{
    res = atol(stringVal);
  }
  return res;
}
void TeleInfo::printAllToSerial(){
  for(int i = 0 ; i< _dataCount ; i++){
    Serial.print(_label[i]);
    Serial.print(" => "); 
    Serial.println(_data[i]);
  }
}

boolean TeleInfo::available(){
  return _isAvailable;
}


void TeleInfo::process(){
  char caractereRecu ='\0';
  while (_cptSerial->available()) {


    //if(_cptSerial->overflow()){
      //_frameIndex = 0;
      //if(_isDebug){
      //  Serial.println("overflow");
      //}
    //}

    caractereRecu = _cptSerial->read() & 0x7F;
    
    if(_isDebug){
      Serial.print(caractereRecu,HEX);
      Serial.print(" ");
      if(caractereRecu == 0x20 || caractereRecu == 0x0A || caractereRecu == 0x0D || caractereRecu == 0x03)
        Serial.println("");
    }
    
    //"Start Text 002" debut de la trame
    if(caractereRecu == 0x02){
      _frameIndex = 0;
    }
    //TODO check _ frame overflow !!!
    _frame[_frameIndex] = caractereRecu;
    _frameIndex++;
    
    //  "EndText 003" Fin de trame
    if(caractereRecu == 0x03 && _frame[0] == 0x02){
      _frame[_frameIndex]='\0';
      _frameIndex++;
      
      if(_isDebug){
        Serial.println("");
        Serial.println("*** will try to decode a new trame");
        ////Serial.println(_frame);
        //Serial.println("*** END trame ***");
      }
      _isAvailable = readFrame();
      if(_isAvailable){
        break;
      }
    }
  }
}

void TeleInfo::resetAvailable(){
  _isAvailable = false;
}


void TeleInfo::resetAll(){
  _frameIndex=0;
  _dataCount = 0;
  _isAvailable = false;
}




void TeleInfo::begin()
{
  //_cptSerial->begin(1200);
  _cptSerial->begin(1200);
  resetAll();
}


boolean TeleInfo::isChecksumValid(char *label, char *data, char checksum) 
{
  unsigned char sum = 32 ;      // Somme des codes ASCII du message + un espace
  int i ;
  
  for (i=0; i < strlen(label); i++) sum = sum + label[i] ;
  for (i=0; i < strlen(data); i++) sum = sum + data[i] ;
  sum = (sum & 63) + 32 ;
  return ( sum == checksum);
}


int TeleInfo::readLabel(int beginIndex, char* label){
  int i = beginIndex;
  int j=0;
  while(_frame[i] != 0x20 && j < LABEL_MAX_SIZE){
    label[j] = _frame[i];
    j++;
    i++;
  }
  if(j == LABEL_MAX_SIZE+1){
    return -1;
  }else{
    label[j] = '\0';
    i++;
    return i;
  }
}


int TeleInfo::readData(int beginIndex, char *data){
  int i = beginIndex;
  int j=0;
  while(_frame[i] != 0x20 && j < DATA_MAX_SIZE){
    data[j] = _frame[i];
    j++;
    i++;
  }
  if(j == DATA_MAX_SIZE+1){
    return -1;
  }else{
    data[j] = '\0';
    i++;
    return i;
  }
}




boolean TeleInfo::readFrame(){
  if(_isDebug) Serial.println("will read a trame");
  int j=0;
  int ligneIndex = 0;
  boolean trameOk = true;
  //start i at 1 to skip first char 0x02 (start byte)
  int i=1;
  while(_frame[i] != 0x03 && ligneIndex < LINE_MAX_COUNT){
    if(_frame[i] != 0x0A){
      if(_isDebug){
        Serial.print(_frame[i], HEX);
        Serial.println(" trame KO 0A -- ");
      }
      trameOk = false;
      break;
    }
    
    i++;
    i = readLabel(i,_label[ligneIndex]);
    if(i<0) {
      if(_isDebug){
        Serial.println("trame KO label");
      }
      trameOk = false;
      break;
    }
    if(_isDebug) {
      Serial.print("label=");
      Serial.println(_label[ligneIndex]);
    }
    
    i = readData(i,_data[ligneIndex]);
    if(i<0) {
      if(_isDebug) Serial.println("trame KO data");
      trameOk = false;
      break;
    }
    
    if(_isDebug) {
      Serial.print("data=");
      Serial.println(_data[ligneIndex]);
    }
    
    char checksum = _frame[i];
    i++;
    if(_isDebug) {
      Serial.print("checksum is ");
      Serial.println(checksum, HEX);
    }
    if(_frame[i] != 0x0D){
      if(_isDebug) Serial.println("  -trame KO 0D");
      trameOk = false;
      break;
    }
    i++;
    if(!isChecksumValid(_label[ligneIndex],_data[ligneIndex],checksum)){
      if(_isDebug) Serial.println("trame KO checksum");
      trameOk = false;
      break;
    }
    ligneIndex++;
  }
  if(ligneIndex >= LINE_MAX_COUNT){
    if(_isDebug) Serial.println("trame KO LINE_MAX_COUNT");
    trameOk = false;
  }else{
    _dataCount = ligneIndex;
  }
  if(!trameOk){
    _dataCount = 0;
  }
  if(_isDebug) {
    if(!trameOk){
      Serial.println("trame KO ");
    }else{
      Serial.println("trame OK !!! ");
    }
  }
  
  return trameOk;
}




