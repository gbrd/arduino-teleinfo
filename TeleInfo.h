#ifndef Teleinfo_h
#define Teleinfo_h

#include <stdint.h>


#include "Arduino.h"
#include <Stream.h>


class TeleInfo
{
  public:
    //TeleInfo(uint8_t rxPin,uint8_t txPin);
	TeleInfo(Stream* serial);

    void begin();
    
    boolean available();
    void process();
    
    void resetAvailable();

    char* getAdco();
	int getIsousc();
	int getIinst();
	long getHchc();
	long getHcHp();
	long getPapp();
	
	//char* getPtec();
	//char* getHhphc();
    //long getImax();
    //char* getOptTarif()
    //char* getMotdetat(); 
    

  private:
    void resetAll();
    boolean checksum_ok(char *etiquette, char *valeur, char checksum); 
    int lireEtiquette(char *ligne);
    int lireValeur(char *ligne, int offset);
    void lireChecksum(char *ligne, int offset);
    boolean affecteEtiquette(char *etiquette, char *valeur);
    boolean decodeLigne(char *ligne);
    boolean lireTrame(); 
    
    //uint8_t _rxPin;
    //uint8_t _txPin;
    Stream* _cptSerial;
    

    
    /***************** Teleinfo configuration part *******************/
	
    boolean _isAvailable = false;
    //char CaractereRecu ='\0';
    
    char _trame[512];
    int _trameIndex = 0;
    
    char _checksum[32];
    char Etiquette[9];
    char Donnee[13];
    
	//TODO rename according naming conventions !
	//TODO autres options tarifaires !
    char _adco[12] ;      // Adresse du concentrateur de téléreport (numéro de série du compteur), 12 numériques + \0
    int _isousc = 0;    // Intensité souscrite, 2 numériques, A
    int _iinst = 0;     // Monophasé - Intensité Instantanée, 3 numériques, A  (intensité efficace instantanée)    
    long _hchc = 0;      // Index option Heures Creuses - Heures Creuses, 8 numériques, Wh
    long _hchp = 0;      // Index option Heures Creuses - Heures Pleines, 8 numériques, Wh
    long _papp = 0;      // Puissance apparente, 5 numérique, VA (arrondie à la dizaine la plus proche)
    
    char PTEC[4] ;      // Période Tarifaire en cours, 4 alphanumériques
    char HHPHC[2] ; // Horaire Heures Pleines Heures Creuses, 1 alphanumérique (A, C, D, E ou Y selon programmation du compteur)
    long IMAX = 0;      // Monophasé - Intensité maximale appelée, 3 numériques, A
    char OPTARIF[4] ;    // Option tarifaire choisie, 4 alphanumériques (BASE => Option Base, HC.. => Option Heures Creuses, EJP. => Option EJP, BBRx => Option Tempo [x selon contacts auxiliaires])
    char MOTDETAT[10];  // Mot d'état du compteur, 10 alphanumériques
    
    
    /******************* END OF CONFIGURATION *******************/
    

    
};

#endif
