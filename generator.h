#ifndef GENERATOR_H
#define GENERATOR_H

#include "counter.h"

enum Mode {None, Manual, AmplitudeSweep};

class Generator {
  public:
    Generator();
    void init();
    void set_frequency(float);
    float get_frequency();
    void set_amplitude(unsigned int);
    unsigned int get_amplitude();
    void enable();
    void disable();
    void toggle();

    void set_mode(Mode);
    void tick();
    void set_ta(unsigned int);
    unsigned int get_ta();
    
  private:
    float f;
    unsigned int amp;
    IntervalTimer timer0;
    uint32_t freq(float);  
    bool enabled;

    Mode mode;
    Counter counter;
    unsigned int ta;
    
};

#endif
