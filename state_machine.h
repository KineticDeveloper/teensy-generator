#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

enum Event {BT1, BT2, BT3, K1L, K1R, K2L, K2R, BT4, BT5, UPDATE_EVT};
void state_machine(Event);


#endif
