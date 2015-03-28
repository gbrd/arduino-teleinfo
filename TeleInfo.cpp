#include "Arduino.h"
#include "TeleInfo.h"


TeleInfo::TeleInfo(Stream* serial)
{
  this->_cptSerial = serial;
  _trame[0] = '\0';
  
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
    caractereRecu = _cptSerial->read() & 0x7F;

/*
    Serial.print(caractereRecu,HEX);
    Serial.print(" ");
    if(caractereRecu == 0x20 || caractereRecu == 0x0A || caractereRecu == 0x0D || caractereRecu == 0x03)
	Serial.println("");
*/
    //"Start Text 002" debut de la trame
    if(caractereRecu == 0x02){
      _trameIndex = 0;
    }
    _trame[_trameIndex] = caractereRecu;
    _trameIndex++;
    
    //  "EndText 003" Fin de trame
    if(caractereRecu == 0x03 && _trame[0] == 0x02){
      _trame[_trameIndex]='\0';
      _trameIndex++;
      Serial.println("");
      //Serial.println("*** will try to decode trame=");
      ////Serial.println(_trame);
      //Serial.println("*** END trame ***");
      _isAvailable = lireTrame();
      if(_isAvailable) break;
    }
  }
}

void TeleInfo::resetAvailable(){
  _isAvailable = false;
}


void TeleInfo::resetAll(){
  _trameIndex=0;
  _dataCount = 0;
  _isAvailable = false;
}




void TeleInfo::begin()
{
  //_cptSerial->begin(1200);
  resetAll();
}

/*------------------------------------------------------------------------------*/
/* Test checksum d'un message (Return 1 si checkum ok)            */
/*------------------------------------------------------------------------------*/
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
	while(_trame[i] != 0x20 && j < LABEL_MAX_SIZE){
		label[j] = _trame[i];
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
	while(_trame[i] != 0x20 && j < DATA_MAX_SIZE){
		data[j] = _trame[i];
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




boolean TeleInfo::lireTrame(){
    Serial.println("will read a trame");
    int j=0;
    int ligneIndex = 0;
    boolean trameOk = true;
    //start i at 1 to skip first char 0x02 (start byte)
	int i=1;
	while(_trame[i] != 0x03 && ligneIndex < LINE_MAX_COUNT){
    		//Serial.println("read trame line start");
		if(_trame[i] != 0x0A){
			Serial.print(_trame[i], HEX);
			Serial.println(" trame KO 0A -- ");
			
			trameOk = false;
			break;
		}

		i++;
		i = readLabel(i,_label[ligneIndex]);
		if(i<0) {
			Serial.println("trame KO label");
			trameOk = false;
			break;
		}
		//Serial.print("label=");
		//Serial.println(_label[ligneIndex]);

		i = readData(i,_data[ligneIndex]);
		if(i<0) {
			Serial.println("trame KO data");
			trameOk = false;
			break;
		}

		//Serial.print("data=");
		//Serial.println(_data[ligneIndex]);

		char checksum = _trame[i];
		//Serial.print("checksum is ");
		//Serial.println(checksum, HEX);
		i++;
		if(_trame[i] != 0x0D){
			Serial.print(_trame[i], HEX);
			Serial.println("  -trame KO 0D");
			trameOk = false;
			break;
		}
		i++;
		if(!isChecksumValid(_label[ligneIndex],_data[ligneIndex],checksum)){
			Serial.println("trame KO checksum");
			trameOk = false;
			break;
		}
		ligneIndex++;
   }
   if(ligneIndex >= LINE_MAX_COUNT){
			Serial.println("trame KO LINE_MAX_COUNT");
		trameOk = false;
   }else{
		_dataCount = ligneIndex;
   }
   if(!trameOk){
		_dataCount = 0;
   }
	if(!trameOk){
	   Serial.println("trame KO ");
	}else{
		Serial.println("trame OK !!! ");
	}

   return trameOk;
}




