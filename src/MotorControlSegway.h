#pragma once

void pwmToMotor(double &outputVal, double &factor1, double &factor2, int LPWM, int RPWM)
{
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
}

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
}

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
}