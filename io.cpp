#include "Arduino.h"

void prompt()
{
  Serial.print("> ");
}

bool availableInput()
{
  if(Serial.available() == 0)
    return false;
  if(Serial.peek() == '\r') {
    Serial.read();
    return false;
  }
  if(Serial.peek() == '\n') {
    Serial.read();
    return false;
  }
  return true;
}

int waitInt(String name)
{
  int ret;
  Serial.print(name);
  Serial.print(" = ");
  while(availableInput()==false);
  ret = Serial.parseInt();
  Serial.println(ret);
  return ret;
}

int waitFloat(String name)
{
  float ret;
  Serial.print(name);
  Serial.print(" = ");
  while(availableInput()==false);
  ret = Serial.parseFloat();
  Serial.println(ret);
  return ret;
}

