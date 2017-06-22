#ifndef GENERATOR_H
#define GENERATOR_H

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
    
  private:
    float f;
    unsigned int amp;
    IntervalTimer timer0;
    uint32_t freq(float);  
    bool enabled;
};

#endif
