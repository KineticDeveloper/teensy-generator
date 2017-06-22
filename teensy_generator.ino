#include <Bounce2.h>
#include <RotaryEncoder.h>
#include <U8g2lib.h>
#include "config.h"
#include "state_machine.h"

#include "generator.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

RotaryEncoder encoder1(encoder1_pin1, encoder1_pin2);
RotaryEncoder encoder2(encoder2_pin1, encoder2_pin2);

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
volatile bool button1_pressed = false;
volatile bool button2_pressed = false;
volatile bool button3_pressed = false;

IntervalTimer timer1;  // used to check buttons and rotary encoders

Generator generator;

void tick() // timer1 callback for the buttons and encoders
{
  encoder1.tick();
  
  encoder2.tick();
  debouncer1.update();
  if(debouncer1.fell()) {
    button1_pressed = true;
  }
  debouncer2.update();  
  if(debouncer2.fell()) {
    button2_pressed = true;
  }
  debouncer3.update();  
  if(debouncer3.fell()) {
    button3_pressed = true;
  }
}

void setup() {
  Serial.begin(9600);
  SIM_SCGC6 |= SIM_SCGC6_DAC0;
  DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;

  
  pinMode(ledPin, OUTPUT);

  u8g2.begin();

  timer1.begin(tick, 1000); // 1000 usec : checks buttons every millisecond
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  debouncer1.attach(button1_pin);
  debouncer1.interval(debouncer_interval);
  debouncer2.attach(button2_pin);
  debouncer2.interval(debouncer_interval);
  debouncer3.attach(button3_pin);
  debouncer3.interval(debouncer_interval);

  generator.init();
}

void loop() {
  static int encPos1=0, encPos2=0;

  state_machine(UPDATE_EVT);

  int newEncPos1 = encoder1.getPosition();
  int newEncPos2 = encoder2.getPosition();

  if(newEncPos1 > encPos1)
    state_machine(K1R);
  else if (newEncPos1 < encPos1)
    state_machine(K1L);
  encPos1=newEncPos1;

  if(newEncPos2 > encPos2)
    state_machine(K2R);
  else if (newEncPos2 < encPos2)
    state_machine(K2L);
  encPos2=newEncPos2;

  if(button1_pressed) {
    state_machine(BT1);
    button1_pressed=false;
  }
  if(button2_pressed) {
    state_machine(BT2);
    button2_pressed=false;
  }
  if(button3_pressed) {
    state_machine(BT3);
    button3_pressed=false;
  }

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
