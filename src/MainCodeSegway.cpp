#include <Arduino.h>
#include <AutoPID.h>
#include <Ticker.h>

#include "BTSegway.h"
#include "MotorControlSegway.h"
#include "MemorySegway.h"



unsigned long StartTijd;


// initialiseer pins
#define LAR PD2  // links vooruit
#define LVR PD4  // links achteruit
#define RAR PD7  // rechts vooruit
#define RVR PD6  // rechts achteruit
#define LPWM PD3 // links pwm signaal
#define RPWM PD5 // rechts pwm signaal
#define INCL PC5 // inclinometer

// initisaliseer een aantal (globale) variabelen
double pwm = 0, factor1, factor2, minSnelheid, deadZone, KP, KI, KD, input, setPoint, outputVal, inputAVG, StuurFactor, HoekRijden;
char besturing = ' ';

// BT ontvangst variabelen
char btChar;
double btDouble;
String readString = "";

// adressen voor EEPROM (Variabelen die aangepast kunnen worden)
int aP = 0, aI = 8, aD = 16, aDeadzone = 24, aSetpoint = 32, aFactor1 = 40, aFactor2 = 48, aMinSnelheid = 56, aStuurFactor = 64, aHoekRijden = 72;

// min-max output PID regelaar
#define OUTPUT_MIN -255
#define OUTPUT_MAX 255

// Maak PID object aan
AutoPID myPID(&inputAVG, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);


// void voor het runnen van PID, motor richting zetten en motoren aansturen
void runPID()
{
  myPID.run();
  setMotorRichting(outputVal, LVR, LAR, RVR, RAR);

  // outputVal mappen van 0-255 naar 30-255 (omdat motoren pas bij 30 draaien)
  double outputMapped = abs(outputVal)+minSnelheid;

  // stuk code voor deadzone
  if (abs(inputAVG - setPoint) < deadZone)
  {
    outputMapped = 0;
  }

  pwmToMotor(outputMapped, factor1, factor2, StuurFactor, besturing, LPWM, RPWM);
}//void runPID

// funtie voor input ophalen en gemiddelde nemen (weer 2 functies want maker van Ticker.h verdient een nekschot)
void InputAVGfunc(double &inputAVG)
{
  //start timer
  /*unsigned long StartTijd = micros();*/

  double total = 0;
  for (int i = 0; i < 8; i++)
  {
    total += analogRead(INCL);
  }
  inputAVG = total / 8;
  //8 inputs is sneller, schuifregister

  //stop timer
  /*unsigned long EindTijd = micros();
  unsigned long Looptijd = EindTijd - StartTijd;
  Serial.println(Looptijd, 10);*/
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

  TCCR2B = TCCR2B & B11111000 | B00000011; //pwm PD3 naar 1000Hz
  TCCR0B = TCCR0B & B11111000 | B00000011; //pwm PD5 freq naar 1000Hz

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

  // Seriele communicatie met baudrate van BT module
  Serial.begin(9600);

  // haal variabelen op uit geheugen
  updateVars(KP, KI, KD, deadZone, setPoint, factor1, factor2, minSnelheid, StuurFactor, HoekRijden, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid, aStuurFactor, aHoekRijden);
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
    switchCase(inputAVG, btChar, btDouble, besturing, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid, aStuurFactor, aHoekRijden);
    updateVars(KP, KI, KD, deadZone, setPoint, factor1, factor2, minSnelheid, StuurFactor, HoekRijden, besturing, aP, aI, aD, aDeadzone, aSetpoint, aFactor1, aFactor2, aMinSnelheid, aStuurFactor, aHoekRijden);
    myPID.setGains(KP, KI, KD);
  }

  // update tickers en run functies indien nodig
  InclTicker.update();
  PIDTicker.update();
}