#include <Arduino.h>
#include "generator.h"
#include "sineTable.h"
#include "digipot.h"
#include "config.h"


volatile uint32_t acc=0, m=0;



Generator::Generator()
{
  m=0;
  enabled=false;
  f=0;
  amp=0;
  

  mode=None;
  ta=0;
  tf=0;

  pc=0;
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
  set_frequency(defaultFrequency);
  set_amplitude(defaultAmplitude);
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
    set_wiper(da);
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

void Generator::set_mode(Mode mo)
{
  mode=mo;
}

void Generator::tick()
{
  if(!enabled)
    return;
  if(mode == Manual)
    m=freq(f);
  else if(mode == AmplitudeSweep) {
    m=freq(f);
    if(counter.get_elapsed_time() > ta) {
      amp = (amp+1) % maxAmplitude;
      set_wiper(amp);
      counter.reset();
    }
  }
  else if(mode == FrequencySweep) {
    m=freq(f);
    if(counter.get_elapsed_time() > tf) {
      f=f+0.01;
      if(f>f2)
        f=0;
      counter.reset();
    }
  }
  else if(mode == Pulse) {
    if(counter.get_elapsed_time() < tOn)
      m=freq(f);
    else {
      m=0;
      acc=0;
    }
    if(counter.get_elapsed_time() > tOn + tOff) {
      pc++;
      counter.reset();
    }
    if(pc>=5) {
      f+=0.01;
      pc=0;
    }
    if(f>=121)
      disable();
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

void Generator::set_tf(unsigned int t)
{
  tf=t;
}

unsigned int Generator::get_tf()
{
  return tf;
}
