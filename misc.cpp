#include "Arduino.h"
#include "config.h"

void ledOn()
{
  digitalWrite(ledPin, HIGH);
}

void ledOff()
{
  digitalWrite(ledPin, LOW);
}

