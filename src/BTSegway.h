#pragma once
#include "MemorySegway.h"

void printPID(double P, double I, double D)
{
  Serial.print("P = ");
  Serial.print(P);
  Serial.print(" I = ");
  Serial.print(I, 7);
  Serial.print(" D = ");
  Serial.println(D, 5);
}

void switchCase(double &inputAVG, char &btChar, double &btDouble, char &besturing, int aP, int aI, int aD, int aDeadzone, int aSetpoint, int aFactor1, int aFactor2, int aMinSnelheid, int aStuurFactor, int aHoekRijden)
{
  /*Gebruikte letters:
  P, I, D: Waarden PID regelaar
  Z: Deadzone
  S: Setpoint
  F: Factor1
  G: Factor2
  N: Minimale snelheid (PWM waarde)
  O: Print input
  A: Print PID waarden
  H: Help commandos
  */

  if (besturing == ' '){
    besturing = 'stop';
  }

  switch (btChar)
  {
  case 'p':
    Serial.println("P - waarde");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aP));
    storeDoubleEEPROM(aP, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aP));
    btChar = ' ';
    break;

  case 'i':
    Serial.println("I - waarde");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aI), 7);
    storeDoubleEEPROM(aI, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aI), 7);
    btChar = ' ';
    break;

  case 'd':
    Serial.println("D - waarde");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aD), 7);
    storeDoubleEEPROM(aD, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aD), 7);
    btChar = ' ';
    break;

  case 'z':
    Serial.println("Deadzone");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aDeadzone));
    storeDoubleEEPROM(aDeadzone, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aDeadzone));
    btChar = ' ';
    break;

  case 's':
    Serial.println("Setpoint");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aSetpoint));
    storeDoubleEEPROM(aSetpoint, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aSetpoint));
    btChar = ' ';
    break;

  case 'f':
    Serial.println("Factor1");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aFactor1));
    storeDoubleEEPROM(aFactor1, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aFactor1));
    btChar = ' ';
    break;

  case 'g':
    Serial.println("Factor2");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aFactor2));
    storeDoubleEEPROM(aFactor2, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aFactor2));
    btChar = ' ';
    break;

  case 'n':
    Serial.println("Minimale PWM waarde");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aMinSnelheid));
    storeDoubleEEPROM(aMinSnelheid, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aMinSnelheid));
    btChar = ' ';
    break;

    case 'r':
    Serial.println("Hoek vooruit/achteruit rijden");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aHoekRijden));
    storeDoubleEEPROM(aHoekRijden, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aHoekRijden));
    btChar = ' ';
    break;

    case 'k':
    Serial.println("Stuur Factor");
    Serial.print("Old: ");
    Serial.println(readDoubleEEPROM(aStuurFactor));
    storeDoubleEEPROM(aStuurFactor, btDouble);
    Serial.print("New: ");
    Serial.println(readDoubleEEPROM(aStuurFactor));
    btChar = ' ';
    break;

  case 'o':
    Serial.println(inputAVG);
    btChar = ' ';
    break;

  case 'h':
    Serial.println(" P, I, D: Waarden PID regelaar");
    Serial.println("Z: Deadzone");
    Serial.println("S: Setpoint");
    Serial.println("F: Factor1");
    Serial.println("G: Factor2");
    Serial.println("N: Minimale snelheid (PWM waarde)");
    Serial.println("R: Hoek vooruit/achteruit rijden");
    Serial.println("K: Stuur Factor");
    Serial.println("O: Print input");
    Serial.println("A: Print huidige PID waarden");
    Serial.println("H: Help commandos");
    break;

  case 'a':
    printPID(readDoubleEEPROM(aP), readDoubleEEPROM(aI), readDoubleEEPROM(aD));
    break;

  case 'j':
    Serial.println("Vooruit rijden");
    besturing = 'vooruit';
    break;
  
  case 'b':
    Serial.println("Achteruit rijden");
    besturing = 'achteruit';
    break;
  
  case 'l':
    Serial.println("Rechts");
    besturing = 'rechts';
    break;

  case 'e':
    Serial.println("Links");
    besturing = 'links';
    break;

  case 'c':
    Serial.println("Stop");
    besturing = 'stop';
    break;

  default:
    btChar = ' ';
    break;
  }
} // void switchcase

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
} // void bluetooth