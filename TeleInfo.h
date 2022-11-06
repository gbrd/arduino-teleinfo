#ifndef Teleinfo_h
#define Teleinfo_h

#include <stdint.h>

#include "Arduino.h"
#include <Stream.h>


#if !defined(LABEL_MAX_SIZE)
#define LABEL_MAX_SIZE 8
#endif

#if !defined(DATA_MAX_SIZE)
#define DATA_MAX_SIZE 15
#endif

#if !defined(LINE_MAX_COUNT)
#define LINE_MAX_COUNT 20
#endif

#if !defined(FRAME_MAX_SIZE)
#define FRAME_MAX_SIZE 700 
#endif

#if !defined(TIMESTAMP_MAX_SIZE)
#define TIMESTAMP_MAX_SIZE 15
#endif

class TeleInfo
{
public:
  //TeleInfo(uint8_t rxPin,uint8_t txPin);
  TeleInfo(Stream* serial);
  
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
  void resetAll();
  //int readLabel(int beginIndex, char* label);
  //int readData(int beginIndex, char* data);  
  int readElement(int beginIndex, char* data, int maxSize);  
  
  
  boolean isChecksumValid(char *label, char *timestamp, char *data, char checksum); 
  
  boolean readFrame(); 
  
  Stream* _cptSerial;
  //SoftwareSerial _serial;
  boolean _isDebug = false;
  boolean _isAvailable = false;
  
  char _frame[FRAME_MAX_SIZE];
  int _frameIndex = 0;
  
  char _label[LINE_MAX_COUNT][LABEL_MAX_SIZE+1]; //+1 for '\0' ending
  char _data[LINE_MAX_COUNT][DATA_MAX_SIZE+1];
  char _timestamp[LINE_MAX_COUNT][TIMESTAMP_MAX_SIZE+1];
  int _dataCount = 0;
  
};

#endif
