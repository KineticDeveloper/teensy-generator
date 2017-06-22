#include "Arduino.h"
#include "state_machine.h"
#include "screen.h"
#include "config.h"
#include "digipot.h"


extern volatile uint32_t acc, m;

float f=0;
int amp=0;
int to;
int tf;
int repetitions;
float n_sinusoids;

void stop()
{
  acc=0;
  m=0;
}

uint32_t freq(float f)
{
  return f * pow(2, 32) / 250000;
}

// This function is a state machine
// It responds to user input, and modifies the global variables above, in order to communicate with the
// generator code in the main module.

void state_machine(Event evt)
{
  enum State {MenuManual_S, MenuFreqSweep_S, MenuAmplSweep_S, MenuThreshold_S, Manual_S, FreqSweep_S, AmplSweep_S, Threshold_S};
  static State state = MenuManual_S;
  static int incf = 0;
  static int inca = 0;

  if(evt != UPDATE_EVT)
    Serial.println(evt);

  switch(state) {
    
    case MenuManual_S:
      clearScreen();
      printScreen(1, "Manuel");
      sendBuffer();
      if(evt == K1R)
        state = MenuFreqSweep_S;
      else if(evt == BT2)
        state = Manual_S;
      Serial.print("going to state ");
      Serial.println(state);
      break;
        
    case MenuFreqSweep_S:
      clearScreen();
      printScreen(1, "Balayage frequence");
      sendBuffer();
      if(evt == K1L)
        state = MenuManual_S;  
      else if(evt == K1R)
        state = MenuAmplSweep_S;
      else if(evt == BT2)
        state = FreqSweep_S;
      break;

    case MenuAmplSweep_S:
      clearScreen();
      printScreen(1, "Balayage amplitude");
      sendBuffer();
      if(evt == K1L)
        state = MenuFreqSweep_S;  
      else if(evt == K1R)
        state = MenuThreshold_S;
      else if(evt == BT2)
        state = AmplSweep_S;
      break;
      
    case MenuThreshold_S:
      clearScreen();
      printScreen(1, "Mesure seuil");
      sendBuffer();
      if(evt == K1L)
        state = MenuAmplSweep_S;
      else if(evt == BT2)
        state = Threshold_S;
      break;
      
    case Manual_S:
      static bool sel = true; // true : we act on frequency, false : we act on amplitude
      clearScreen();
      m=freq(f);
      set_wiper(amp);
      printScreen(1, "f = " + String(f, 3));
      printScreen(2, "incf = " + String(pow(10, incf), 3));
      printScreen(3, "amp = " + String(amp));
      printScreen(4, "inca = " + String((int)pow(10, inca)));
      sendBuffer();
      Serial.println("incf=" + String(incf));
      if(evt == BT1)
        state = MenuManual_S;
      else if (evt == BT2)
        sel = !sel;
      else if(evt == K1R) {
        if(sel==true)
          f = constrain(f+pow(10, incf), 0, 121);
        else
          amp = constrain(amp+pow(10, inca), 0, 1023);
      }
      else if(evt == K1L) {
        if(sel==true)
          f = constrain(f-pow(10, incf), 0, 121);
        else
          amp = constrain(amp-pow(10, inca), 0, 1023);
      }
      else if(evt == K2R) {
        if(sel==true) 
          incf = constrain(incf+1, -2, 2);
        else
          inca = constrain(inca+1, 0, 3);
      }
      else if(evt == K2L) {
        if(sel==true)
          incf = constrain(incf-1, -2, 2);
        else
          inca = constrain(inca-1, 0, 3);
      }
      break;

    case FreqSweep_S:
      clearScreen();
      printScreen(1, "Balayage frequence...");
      sendBuffer();
      if(evt == BT1)
        state = MenuFreqSweep_S;
      break;

    case AmplSweep_S:
      clearScreen();
      printScreen(1, "Balayage amplitude...");
      sendBuffer();
      if(evt == BT1)
        state = MenuAmplSweep_S;
      break;

    case Threshold_S:
      clearScreen();
      printScreen(1, "Seuil...");
      sendBuffer();
      if(evt == BT1)
        state = MenuThreshold_S;
  
      break;
      
    default:
      state=MenuManual_S;
      break;
      
  }
}
