#include <Arduino.h>
#include "generator.h"
#include "sineTable.h"
#include "digipot.h"


volatile uint32_t acc=0, m=0;

Generator::Generator()
{
  m=0;
  f=0;
  amp=0;
  enabled=false;
}

uint32_t Generator::freq(float f)
{
  return f * pow(2, 32) / 250000;
}

void clk() // timer0 callback for the DDS
{
  *(int16_t *)&(DAC0_DAT0L) = sineTable[acc>>23];
  acc+=m;
}

void Generator::init() {
  timer0.begin(clk, 4); // 4 usec -> f = 250 kHz
  init_digipot();
}

void Generator::set_frequency(float df)
{
  f=df;
  if(enabled)
    m=freq(f);
}

float Generator::get_frequency()
{
  return f;
}

void Generator::set_amplitude(unsigned int da)
{
  if(amp!=da) {
    set_wiper(amp);
    amp=da;
  }
}

unsigned int Generator::get_amplitude()
{
  return amp;
}

void Generator::enable()
{
  m=freq(f);
  enabled=true;
}

void Generator::disable()
{
  m=0;
  acc=0;
  enabled=false;
}

void Generator::toggle()
{
  if(enabled)
    disable();
  else
    enable();
}

