#ifndef COUNTER_H
#define COUNTER_H

class Counter {
  public:
    void reset();
    unsigned int get_elapsed_time();
    
  private:
    unsigned int ms;
};

#endif
