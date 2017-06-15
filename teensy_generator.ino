#include <Bounce2.h>
#include <RotaryEncoder.h>
#include <U8g2lib.h>
#include "sineTable.h"
#include "mode.h"
#include "config.h"
#include "mode.h"
#include "state_machine.h"
#include "memory.h"

// #########################################################################
// These global variables are used to drive the generator from other modules
volatile uint32_t acc=0, m=0;
float f=0;
bool running = false;
int to=0;
int tf=0;
int repetitions=0;
float n_sinusoids = 0;
Mode current_mode = sweep;
// #########################################################################

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
IntervalTimer timer0;
RotaryEncoder encoder1(4,5);
RotaryEncoder encoder2(6,7);

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3

uint32_t freq(float f)
{
  return f * pow(2, 32) / 250000;
}

void stop()
{
  acc=0;
  m=0;
}

void clk()
{
  *(int16_t *)&(DAC0_DAT0L) = sineTable[acc>>23];
  acc+=m;
}

void setup() {
  Serial.begin(9600);
  SIM_SCGC6 |= SIM_SCGC6_DAC0;
  DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;

  timer0.begin(clk, 4); // 4 usec -> f = 250 kHz
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  loadAllVariables(); // We load all the variables from the EEPROM into the global variables
  u8g2.begin();
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  debouncer1.attach(BUTTON1_PIN);
  debouncer1.interval(5);
  debouncer2.attach(BUTTON2_PIN);
  debouncer2.interval(5);
  
}

void loop() {
  
  static int encPos1=0, encPos2=0;
  static int mm=0;
  //Serial.println(millis()-mm);
  //m=millis();
  encoder1.tick();
  encoder2.tick();
  debouncer1.update();
  debouncer2.update();
  int newEncPos1 = encoder1.getPosition();
  int newEncPos2 = encoder2.getPosition();

  if(newEncPos1 > encPos1)
    state_machine(K1R);
  else if (newEncPos1 < encPos1)
    state_machine(K1L);
  encPos1=newEncPos1;

  if(newEncPos1 > encPos1)
    state_machine(K1R);
  else if (newEncPos1 < encPos1)
    state_machine(K1L);
  encPos1=newEncPos1;

  if(debouncer1.fell())
    state_machine(BT1);
  if(debouncer2.fell())
    state_machine(BT2);
    
  //Serial.println(encoder.getPosition());

  return;
  
/*
  state_machine(); // We check for input, and manage it with a state machine

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
  */
}
