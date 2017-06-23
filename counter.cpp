#include <Arduino.h>
#include "counter.h"

void Counter::reset()
{
  ms = millis();
}

unsigned int Counter::get_elapsed_time()
{
  return millis()-ms;
}

