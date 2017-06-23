#include <Arduino.h>
#include "generator.h"
#include "sineTable.h"
#include "digipot.h"
#include "config.h"


volatile uint32_t acc=0, m=0;



Generator::Generator()
{
  m=0;
  f=0;
  amp=0;
  enabled=false;

  mode=None;
  ta=0;
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
  enabled=true;
  counter.reset();
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

void Generator::set_mode(Mode m)
{
  mode=m;
}

void Generator::tick()
{
  if(!enabled)
    return;
  if(mode == Manual)
    m=freq(f);
  else if(mode == AmplitudeSweep) {
    if(counter.get_elapsed_time() > ta) {
      amp = (amp+1) % maxAmplitude;
      counter.reset();
    }
  }
}

void Generator::set_ta(unsigned int t)
{
  ta=t;
}

unsigned int Generator::get_ta()
{
  return ta;
}

