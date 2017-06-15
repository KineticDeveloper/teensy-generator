#include "Arduino.h"
#include <EEPROM.h>
#include "mode.h"
#include "config.h"

extern float f;
extern int amp;
extern int to;
extern int tf;
extern int repetitions;
extern int n_sinusoids;
extern Mode current_mode;
extern bool running;

// ### Addresses for variables stored in the eeprom
int freqAddr = 0;
int autostartAddr = freqAddr + sizeof(float);
int modeAddr = autostartAddr + 1;
int toAddr = modeAddr + sizeof(Mode);
int tfAddr = toAddr + sizeof(int);
int repetitionsAddr = tfAddr + sizeof(int);
int nSinusoidsAddr = repetitionsAddr + sizeof(int);

void saveFrequency(float freq)
{
  EEPROM.put(freqAddr, freq);
}

float loadFrequency()
{
  float ret=0;
  EEPROM.get(freqAddr, ret);
  return ret;
}

void resetFrequency()
{
    saveFrequency(f1);
    f=f1;
}

bool loadAutostart()
{
  return EEPROM.read(autostartAddr) == 1 ? true : false;
}

void saveAutostart(bool val)
{
  if(val==true)
    EEPROM.write(autostartAddr, 1);
  else
    EEPROM.write(autostartAddr, 0);
}

Mode loadMode()
{
  return (Mode)EEPROM.read(modeAddr);
}

void saveMode(Mode ms)
{
  EEPROM.write(modeAddr, (byte)ms);
}

void saveTo(int t)
{
  EEPROM.put(toAddr, t);
  to=t;
}

int loadTo()
{
  int ret=0;
  EEPROM.get(toAddr, ret);
  return ret;
}

void saveTf(int t)
{
  EEPROM.put(tfAddr, t);
  tf=t;
}

int loadTf()
{
  int ret=0;
  EEPROM.get(tfAddr, ret);
  return ret;
}

void saveRepetitions(int r)
{
  EEPROM.put(repetitionsAddr, r);
  repetitions=r;
}

int loadRepetitions()
{
  int ret=0;
  EEPROM.get(repetitionsAddr, ret);
  return ret;
}

void saveNSinusoids(float n)
{
  EEPROM.put(nSinusoidsAddr, n);
  n_sinusoids=n;
}

float loadNSinusoids()
{
  float ret=0;
  EEPROM.get(nSinusoidsAddr, ret);
  return ret;
}

void loadAllVariables()
{
  f = loadFrequency();
  if(loadAutostart())
    running = true;
  current_mode = loadMode();

  to = loadTo();
  tf = loadTf();
  repetitions=loadRepetitions();
  n_sinusoids=loadNSinusoids();
}
