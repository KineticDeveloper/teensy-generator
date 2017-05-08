#ifndef MEMORY_H
#define MEMORY_H

#include "mode.h"

void saveFrequency(float);
float loadFrequency();
void resetFrequency();
bool loadAutostart();
void saveAutostart(bool);
Mode loadMode();
void saveMode(Mode);
void saveTo(int);
int loadTo();
void saveTf(int);
int loadTf();
void saveRepetitions(int);
int loadRepetitions();
void saveNSinusoids(float);
float loadNSinusoids();

#endif
