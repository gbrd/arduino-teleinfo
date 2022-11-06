#include "Arduino.h"
#include "TeleInfo.h"


TeleInfo::TeleInfo(Stream* serial)
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
  while (_cptSerial->available() && !_isAvailable) {


    // checksum is enough...
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
      if(caractereRecu == 0x20 || caractereRecu == 0x09 || caractereRecu == 0x0A || caractereRecu == 0x0D || caractereRecu == 0x03)
        Serial.println("");
    }
    
    //"Start Text 002" - frame start
    if(caractereRecu == 0x02){
      _frameIndex = 0;
    }
    //TODO check _ frame overflow !!!
    _frame[_frameIndex] = caractereRecu;
    _frameIndex++;
    
    //  "EndText 003" - frame end
    if(caractereRecu == 0x03 && _frame[0] == 0x02){
      _frame[_frameIndex]='\0';
      _frameIndex++;
      
      if(_isDebug){
        Serial.println("");
        Serial.println("*** will try to decode a new frame");
        ////Serial.println(_frame);
        //Serial.println("*** END frame ***");
      }
      _isAvailable = readFrame();
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
  resetAll();
}


boolean TeleInfo::isChecksumValid(char *label, char* timestamp, char *data, char checksum) 
{
  unsigned char sum = 0;      
  unsigned char sumH = 0x20;    // En mode historique : Somme des codes ASCII du message + un espace  
  unsigned char sumS = 0x09 + 0x09; // En mode standard : Somme des codes ASCII du message + 2 tabs 
  unsigned int i ;
  
  for (i=0; i < strlen(label); i++) sum = sum + label[i] ;
  for (i=0; i < strlen(data); i++) sum = sum + data[i] ;
  if(timestamp != NULL){
    for (i=0; i < strlen(timestamp); i++) sum = sum + data[i] ;
    sumS = sumS + 0x09; // only in standard mode, one more separator 
  }
  sumS = ((sumS+sum) & 63) + 32 ;
  sumH = ((sumH+sum) & 63) + 32 ;
  return (sumS == checksum || sumH == checksum);
}

/**
 * Read a label in 'label' buffer. Start reading at beginInder in frame buffer.
 * return index pointing just after the label 
 */
  

//DATA_MAX_SIZE or LABEL_MAX_SIZE
int TeleInfo::readElement(int beginIndex, char *data, int maxSize){
  int i = beginIndex;
  int j=0;
  while(_frame[i] != 0x20 && _frame[i] != 0x09 && j < maxSize){
    data[j] = _frame[i];
    j++;
    i++;
  }
  if(j == maxSize+1){
    return -1;
  }else{
    data[j] = '\0';
    i++;
    return i;
  }
}



boolean TeleInfo::readFrame(){
  if(_isDebug) Serial.println("will read a frame");
  //int j=0;
  int lineIndex = 0;
  boolean frameOk = true;
  //start i at 1 to skip first char 0x02 (start byte)
  int i=1;
  while(_frame[i] != 0x03 && lineIndex < LINE_MAX_COUNT){
    if(_frame[i] != 0x0A){
      if(_isDebug){
        Serial.print(_frame[i], HEX);
        Serial.println(" frame KO 0A -- ");
      }
      frameOk = false;
      break;
    }
    
    i++;
    i = readElement(i,_label[lineIndex],LABEL_MAX_SIZE);
    if(i<0) {
      if(_isDebug){
        Serial.println("frame KO label");
      }
      frameOk = false;
      break;
    }
    if(_isDebug) {
      Serial.print("label=");
      Serial.println(_label[lineIndex]);
    }
        
    
    i = readElement(i,_data[lineIndex],DATA_MAX_SIZE);
    if(i<0) {
      if(_isDebug) Serial.println("frame KO data");
      frameOk = false;
      break;
    }
    
    if(_isDebug) {
      Serial.print("data=");
      Serial.println(_data[lineIndex]);
    }
    if (_frame[i] == 0x9){
      // we have a frame with timestamp !
      // => copy data to timestamp and read again data
      strcpy(_timestamp[lineIndex],_data[lineIndex]);
      i = readElement(i,_data[lineIndex],DATA_MAX_SIZE);
    }
    
    char checksum = _frame[i];
    i++;
    if(_isDebug) {
      Serial.print("checksum is ");
      Serial.println(checksum, HEX);
    }
    if(_frame[i] != 0x0D){
      if(_isDebug) Serial.println("  -frame KO 0D");
      frameOk = false;
      break;
    }
    i++;
    if(!isChecksumValid(_label[lineIndex],_timestamp[lineIndex],_data[lineIndex],checksum)){
      if(_isDebug) Serial.println("frame KO checksum");
      frameOk = false;
      break;
    }
    lineIndex++;
  }
  if(lineIndex >= LINE_MAX_COUNT){
    if(_isDebug) Serial.println("frame KO LINE_MAX_COUNT");
    frameOk = false;
  }else{
    _dataCount = lineIndex;
  }
  if(!frameOk){
    _dataCount = 0;
  }
  if(_isDebug) {
    if(!frameOk){
      Serial.println("frame KO ");
    }else{
      Serial.println("frame OK !!! ");
    }
  }
  
  return frameOk;
}




