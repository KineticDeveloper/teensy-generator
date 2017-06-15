#include "Arduino.h"
#include "state_machine.h"
#include "screen.h"
#include "memory.h"
#include "mode.h"
#include "config.h"


extern volatile uint32_t acc, m;
extern float f;
extern bool running;
extern int to;
extern int tf;
extern int repetitions;
extern float n_sinusoids;
extern Mode current_mode;
extern void stop();



// This function is a state machine
// It responds to user input, and modifies the global variables above, in order to communicate with the
// generator code in the main module.

void state_machine(Event evt)
{
  enum State {MenuManual_S, MenuSweep_S, MenuThreshold_S, Manual_S, Sweep_S, Threshold_S};
  static State state = MenuManual_S;

  if(evt != UPDATE_EVT)
    Serial.println(evt);

  switch(state) {
    
    case MenuManual_S:
      printScreen("Manuel");
      Serial.println("Manuel");
      if(evt == K1R)
        state = MenuSweep_S;
      else if(evt == BT2)
        state = Manual_S;
      Serial.print("going to state ");
      Serial.println(state);
      break;
        
    case MenuSweep_S:
      printScreen("Balayage");
      if(evt == K1L)
        state = MenuManual_S;  
      else if(evt == K1R)
        state = MenuThreshold_S;
      else if(evt == BT2)
        state = Sweep_S;
      break;

    case MenuThreshold_S:
      printScreen("Mesure seuil");
      if(evt == K1L)
        state = MenuSweep_S;
      else if(evt == BT2)
        state = Threshold_S;
      break;
      
    case Manual_S:
      printScreen("f= a=");
      if(evt == BT1)
        state = MenuManual_S;
      break;

    case Sweep_S:
      printScreen("Balayage...");
      if(evt == BT1)
        state = MenuSweep_S;
      break;

    case Threshold_S:
      printScreen("Seuil...");
      if(evt == BT1)
        state = MenuThreshold_S;
  
      break;
      
    case UPDATE_EVT:
      break; 

    default:
      state=MenuManual_S;
      break;
      
  }
}
