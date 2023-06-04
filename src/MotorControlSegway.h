#pragma once

void pwmToMotor(double &outputVal, double &factor1, double &factor2, double &StuurFactor, char &besturing, int LPWM, int RPWM)
{
  switch (besturing){
    case 'rechts':
      if (outputVal * factor1 > 255)
      {
        analogWrite(LPWM, 255);
      }
      else
      {
        analogWrite(LPWM, outputVal * factor1);
      }

      if (outputVal * factor2 * StuurFactor > 255)
      {
        analogWrite(RPWM, 255);
      }
      else
      {
        analogWrite(RPWM, outputVal * factor2 * StuurFactor);
      }
      break;
    
    case 'links':
      if (outputVal * factor1 * StuurFactor > 255)
      {
        analogWrite(LPWM, 255);
      }
      else
      {
        analogWrite(LPWM, outputVal * factor1 * StuurFactor);
      }

      if (outputVal * factor2 > 255)
      {
        analogWrite(RPWM, 255);
      }
      else
      {
        analogWrite(RPWM, outputVal * factor2);
      }
      break;
    
    default:
      if (outputVal * factor1 > 255)
      {
        analogWrite(LPWM, 255);
      }
      else
      {
        analogWrite(LPWM, outputVal * factor1);
      }

      if (outputVal * factor2 > 255)
      {
        analogWrite(RPWM, 255);
      }
      else
      {
        analogWrite(RPWM, outputVal * factor2);
      }
      break;
  }
}//void pwmtomotor

//voids ricting
void Lachteruit(int LVR, int LAR)
{
  digitalWrite(LVR, LOW);
  digitalWrite(LAR, HIGH);
}
void Rachteruit(int RVR, int RAR)
{
  digitalWrite(RVR, LOW);
  digitalWrite(RAR, HIGH);
}
void Lvooruit(int LVR, int LAR)
{
  digitalWrite(LVR, HIGH);
  digitalWrite(LAR, LOW);
}
void Rvooruit(int RVR, int RAR)
{
  digitalWrite(RVR, HIGH);
  digitalWrite(RAR, LOW);
}//voids richting

void setMotorRichting(double outputVal, int LVR, int LAR, int RVR, int RAR)
{
  if (outputVal <= 0)
  {
    Lachteruit(LVR, LAR);
    Rachteruit(RVR, RAR);
  }
  else
  {
    Lvooruit(LVR, LAR);
    Rvooruit(RVR, RAR);
  }
}//void setmotorrichting

double mapDouble(double x, double inMin, double inMax, double outMin, double outMax) {
  // Scale the input value to an integer range
  int scaledX = (x - inMin) * 255 / (inMax - inMin);

  // Map the scaled value to the output range
  int mappedX = map(scaledX, 0, 255, outMin, 255);

  // Scale the mapped value back to a double range
  double mappedDouble = (mappedX - 20) * (outMax - outMin) / (255 - 20) + outMin;

  return mappedDouble;
}