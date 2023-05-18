#pragma once
#include "MemorySegway.h"

void switchCase(char &btChar, double &btDouble, double &KP, double &KI, double &KD, double &deadZone, double &setPoint,
                double &Factor1, double &Factor2, double &minSnelheid, int aP, int aI, int aD, int aDeadzone, int aSetpoint, int aFactor1, int aFactor2, int aMinSnelheid)
{
  switch (btChar)
  {
  case 'p':
    storeDoubleEEPROM(aP, btDouble);
    btChar = ' ';
    break;

  case 'i':
    storeDoubleEEPROM(aI, btDouble);
    btChar = ' ';
    break;

  case 'd':
    storeDoubleEEPROM(aD, btDouble);
    btChar = ' ';
    break;

  case 'z':
    storeDoubleEEPROM(aDeadzone, btDouble);
    btChar = ' ';
    break;

  case 's':
    storeDoubleEEPROM(aSetpoint, btDouble);
    btChar = ' ';
    break;

  case 'f':
    storeDoubleEEPROM(aFactor1, btDouble);
    btChar = ' ';
    break;

  case 'g':
    storeDoubleEEPROM(aFactor2, btDouble);
    btChar = ' ';
    break;

  case 'n':
    storeDoubleEEPROM(aMinSnelheid, btDouble);
    btChar = ' ';
    break;

  default:
    btChar = ' ';
    break;
  }
}//void switchcase

void Bluetooth(char &btChar, double &btDouble, String &readString)
{
  while (Serial.available())
  {
    delay(10);
    // read the input
    char c = Serial.read(); // read a character
    readString += c;        // add it to a String
  }
  if (readString.length() > 0)
  { // als er iets is onvangen
    btChar = readString[0];
    readString.remove(0, 1);
    btDouble = readString.toDouble();
    readString = ""; // reset string voor volgende ontvangst
  }
}//void bluetooth

void printPID(double P, double I, double D){
  Serial.print("P = ");
  Serial.print(P);
  Serial.print(" I = ");
  Serial.print(I, 7);
  Serial.print(" D = ");
  Serial.println(D, 5);
}