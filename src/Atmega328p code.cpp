#include <Arduino.h>
#include <AutoPID.h>
#include <Ticker.h>
#include "BTSegway.h"
#include "MotorControlSegway.h"
#include "MemorySegway.h"

// initialiseer pins
#define LAR PD2  // links vooruit
#define LVR PD4  // links achteruit
#define RAR PD7  // rechts vooruit
#define RVR PD6  // rechts achteruit
#define LPWM PD3 // links pwm signaal
#define RPWM PD5 // rechts pwm signaal
#define INCL PC5 // inclinometer

// initisaliseer een aantal (globale) variabelen

// pwm (0-255)
double pwm = 0;

// vermenigvuldigingsfactor motor links en rechts
double factor1;
double factor2;

// BT ontvangst variabelen
char btChar;
double btDouble;
String readString = "";

// variabelen m.b.t. motoren
double minSnelheid;
double deadZone;

// min-max output PID regelaar
#define OUTPUT_MIN -255
#define OUTPUT_MAX 255

// PID parameters
double KP, KI, KD;
double input, setPoint, outputVal, inputAVG;

// adressen voor EEPROM (Variabelen die aangepast kunnen worden)
int aP = 0, aI = 8, aD = 16, aDeadzone = 24, aSetpoint = 32, aFactor1 = 40, aFactor2 = 48, aMinSnelheid = 56;

// Maak PID object aan
AutoPID myPID(&inputAVG, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

// void voor het runnen van PID want ticker is incapabel
void runPID()
{
  myPID.run();
}

// funtie voor input ophalen en gemiddelde nemen (functie in functie want ticker zuigt ballen)
void InputAVGfunc(double &inputAVG)
{
  double total = 0;
  for (int i = 0; i < 10; i++)
  {
    total += analogRead(INCL);
  }
  inputAVG = total / 10;
}

void callInAVGFunc()
{
  InputAVGfunc(inputAVG);
}

// tickers declareren moet globaal want zowel in setup als in loop aangeroepen
// ticker voor Inclinometer
Ticker InclTicker(callInAVGFunc, 1, 0, MILLIS);
// ticker voor PID algoritme
Ticker PIDTicker(runPID, 1, 0, MILLIS);

void setup()
{

  // Logic pins
  pinMode(LVR, OUTPUT);
  pinMode(LAR, OUTPUT);
  pinMode(RVR, OUTPUT);
  pinMode(RAR, OUTPUT);

  // pwm pin(s)
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  // inclinometer pin
  pinMode(INCL, INPUT);

  // pid setup functies
  myPID.setTimeStep(0);
  // myPID.setBangBang(250, 250);

  // Seriele communicatie met baudrate van BT module
  Serial.begin(9600);

  // haal variabelen op uit geheugen
  updateVars(KP, KI, KD, deadZone, setPoint, factor1, factor2, minSnelheid, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid);
  // wijs pid gains toe
  myPID.setGains(KP, KI, KD);

  // start de tickers
  PIDTicker.start();
  InclTicker.start();
}

void loop()
{
  // als data over bluetooth wordt gestuurd, ga uitlezen en aanpassen
  if (Serial.available())
  {
    Bluetooth(btChar, btDouble, readString);
    switchCase(btChar, btDouble, KP, KI, KD, deadZone, setPoint, factor1, factor2, minSnelheid, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid);
    updateVars(KP, KI, KD, deadZone, setPoint, factor1, factor2, minSnelheid, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid);
    myPID.setGains(KP, KI, KD);
  }
  // update tickers en run functies indien nodig
  PIDTicker.update();
  InclTicker.update();

  // bepalen en setten van de motor richting (moet voor abs(outputval))

  setMotorRichting(outputVal, LVR, LAR, RVR, RAR);

  // outputval absoluut nemen zodat het signaal naar de motoren kan
  outputVal = abs(outputVal);

  // outputVal mappen van 0-255 naar 30-255 (omdat motoren pas bij 30 draaien)
  double outputMapped = mapDouble(outputVal, 0, 255, 30, 255);

  // stuk code voor deadzone
  double ERROR = abs(inputAVG - setPoint);

  if (ERROR < deadZone)
  {
    outputVal = 0;
  }

  pwmToMotor(outputMapped, factor1, factor2, LPWM, RPWM);

  // misc prints voor debugging
  Serial.print("InputAVG: ");
  Serial.print(inputAVG);
  Serial.print(" Output = ");
  Serial.println(outputVal);

  /*
    Serial.print("InputAVG: ");
    Serial.print(inputAVG);
    Serial.print(" Output = ");
    Serial.print(outputVal);
    Serial.print("Deadzone = ");
    Serial.print(deadZone);
    Serial.print(" Input = ");
    Serial.print(input);
    Serial.print(" KP = ");
    Serial.print(KP, 7);
    Serial.print(" KI = ");
    Serial.print(KI, 7);
    Serial.print(" KD = ");
    Serial.println(KD, 7);
    Serial.print("LVR = ");
    Serial.print(digitalRead(LVR));
    Serial.print("LAR = ");
    Serial.print(digitalRead(LAR));
    Serial.print("RVR = ");
    Serial.print(digitalRead(RVR));
    Serial.print("RAR = ");
    Serial.println(digitalRead(RAR));
    */
}