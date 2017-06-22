#ifndef GENERATOR_H
#define GENERATOR_H

class Generator {
  public:
    void init();
    void set_frequency(float);
    float get_frequency();
    void set_amplitude(unsigned int);
    unsigned int get_amplitude();
    void enable();
    void disable();
    
  private:
    float f;
    unsigned int amp;
    IntervalTimer timer0;
    uint32_t freq(float);  
};

#endif
