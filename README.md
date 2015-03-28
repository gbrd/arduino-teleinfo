# arduino-teleinfo
teleinfo library for arduino 

For french teleinfo system (ERDF)


*** WORK IN PROGRESS, NOT TESTED ***

	#include <SoftwareSerial.h>
	SoftwareSerial serial(2,3);
	TeleInfo teleinfo(&serial);

or 

	TeleInfo teleinfo(&Serial);



	serial.begin(1200) //must be done in setup()