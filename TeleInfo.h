#ifndef Teleinfo_h
#define Teleinfo_h

#include <stdint.h>


#include "Arduino.h"
#include <Stream.h>

//#define HARD_TELEINFO


#ifndef HARD_TELEINFO
#include <SoftwareSerial.h>
#endif


#define LABEL_MAX_SIZE 8
#define DATA_MAX_SIZE 16
#define LINE_MAX_COUNT 32


class TeleInfo
{
public:
#ifdef HARD_TELEINFO
  TeleInfo();
#else
  TeleInfo(uint8_t rxPin,uint8_t txPin);
#endif
  
  
  void begin();
  
  boolean available();
  void process();
  
  void resetAvailable();
  
  /**
   * return NULL if this label is not found
   */
  const char * getStringVal(const char * label);
  /**
   * return a negative valure if this label is not found
   */
  long getLongVal(const char * label);  
  void printAllToSerial();
  
  void setDebug(boolean debug);
  
  
private:
  
  Stream* getStream();
  boolean overflow();
  
  void resetAll();
  int readLabel(int beginIndex, char* label);
  int readData(int beginIndex, char* data);  
  
  boolean isChecksumValid(char *label, char *data, char checksum); 
  
  boolean readFrame(); 
  
#ifndef HARD_TELEINFO
  SoftwareSerial* _softSerial = NULL;
#endif
  
  boolean _isDebug = false;
  boolean _isAvailable = false;
  
  char _frame[512];
  int _frameIndex = 0;
  
  char _label[LINE_MAX_COUNT][LABEL_MAX_SIZE+1]; //+1 for '\0' ending
  char _data[LINE_MAX_COUNT][DATA_MAX_SIZE+1];
  int _dataCount = 0;
  
};

#endif
