#include "Arduino.h"
#include "state_machine.h"
#include "screen.h"
#include "memory.h"
#include "mode.h"
#include "io.h"
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
  enum State {MenuFrequency_S, MenuAmplitude_S, MenuSweep_S, Frequency_S, Amplitude_S};
  static State state = Frequency_S;

  Serial.println(evt);

  switch(state) {
    
    case MenuFrequency_S:
      printScreen("Frequence");
      if(evt == K1R)
        state = MenuAmplitude_S;
        break;
        
    case MenuAmplitude_S:
      printScreen("Amplitude");
      if(evt == K1L)
        state = MenuFrequency_S;  
      if(evt == K1R)
        state = MenuSweep_S;
      break;
      
    case MenuSweep_S:
      printScreen("Balayage");
      if(evt == K1L)
        state = MenuAmplitude_S;
      break;
      

    default:
      state=MenuFrequency_S;
      break;
      
  }
}
