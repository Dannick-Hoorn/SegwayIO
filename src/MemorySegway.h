#pragma once

#include "BTSegway.h"
#include <avr/eeprom.h>

// Define the EEPROM address
#define EEPROM_ADDRESS 0

void storeDoubleEEPROM(int address, double variable) {
  eeprom_write_block(&variable, (void *)(address), sizeof(variable));
}

double readDoubleEEPROM(int address) {
  double variable;
  eeprom_read_block(&variable, (void *)(address), sizeof(variable));
  return variable;
}

void updateVars(double& KP, double& KI, double& KD, double& deadZone, double& setPoint, double& Factor1, double& Factor2, double& minSnelheid, double& StuurFactor, double& Hoekrijden, char& besturing, char& rijden,
                int aP, int aI, int aD, int aDeadzone, int aSetpoint, int aFactor1, int aFactor2, int aMinSnelheid, int aStuurFactor, int aHoekRijden){
    KP = readDoubleEEPROM(aP);
    KI = readDoubleEEPROM(aI);
    KD = readDoubleEEPROM(aD);
    deadZone = readDoubleEEPROM(aDeadzone);
    Factor1 = readDoubleEEPROM(aFactor1);
    Factor2 = readDoubleEEPROM(aFactor2);
    minSnelheid = readDoubleEEPROM(aMinSnelheid);
    StuurFactor = readDoubleEEPROM(aStuurFactor);
    Hoekrijden = readDoubleEEPROM(aHoekRijden);


    switch(rijden){
      case 'v':
        setPoint = readDoubleEEPROM(aSetpoint) + Hoekrijden; //checken of dit + of - moet zijn
        break;
      
      case 'a': //naam achteruit mocht niet
        setPoint = readDoubleEEPROM(aSetpoint) - Hoekrijden; //checken of dit + of - moet zijn
        break;

      default:
        setPoint = readDoubleEEPROM(aSetpoint);
        break;
    }
}