#include "Arduino.h"
#include "mode.h"

void displayMode(Mode m)
{
  switch(m) {
    case single_frequency:
      Serial.print("single_frequency");
      break;
    case sweep:
      Serial.print("sweep");
      break;
    case sweep_n_sinusoids:
      Serial.print("sweep_n_sinusoids");
      break;
    default:
      Serial.print("invalid");
      break;
  }
}
