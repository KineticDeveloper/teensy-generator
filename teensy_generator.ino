
#include "sineTable.h"
#include "mode.h"
#include "misc.h"
#include "config.h"
#include "mode.h"
#include "console.h"
#include "memory.h"

volatile uint32_t acc=0, m=0;
float f=0;
bool running = false;
int to=0;
int tf=0;
int repetitions=0;
float n_sinusoids = 0;
Mode current_mode = sweep;

IntervalTimer timer0;

float cor=1; // cor = measured frequency (with frequency counter) divided by desired frequency



uint32_t freq(float f)
{
  return f * pow(2, 32) / (250000 * cor);
}

void stop()
{
  acc=0;
  m=0;
}

void setup() {
  Serial.begin(9600);
  SIM_SCGC6 |= SIM_SCGC6_DAC0;
  DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;

  timer0.begin(clk, 4); // 4 usec -> f = 250 kHz
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  loadAllVariables();
}

void clk()
{
  *(int16_t *)&(DAC0_DAT0L) = sineTable[acc>>23];
  acc+=m;
}





void loop() {

  console();

  if(running == false) {
    ledOff();
    return;
  }

  if(current_mode == single_frequency) {
    m = freq(f);
    ledOn();
  }
  else if(current_mode == sweep) {
    if(f<=f2) {
      for(int i=0;i<repetitions;i++) {
        m=freq(f);
        ledOn();
        delay(to);
        stop();
        ledOff();
        delay(tf); 
      }
      f+=0.01;
    }
  }
  else if(current_mode == sweep_n_sinusoids) {
    if(f<=f2) {
      for(int i=0;i<repetitions;i++) {
        m=freq(f);
        ledOn();
        delay(n_sinusoids*1000/f);
        stop();
        ledOff();
        delay(n_sinusoids*1000/f);
      }
      f+=0.01;
    }
  }
  


}



