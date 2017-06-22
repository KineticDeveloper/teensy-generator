#include <SPI.h>
#include "config.h"

void init_digipot()
{
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  pinMode(digipot_cs_pin, OUTPUT);
  digitalWrite(digipot_cs_pin, HIGH);
}

void set_wiper(unsigned int value)
{
  digitalWrite(digipot_cs_pin, LOW);
  delay(1);
  byte command = 0x0;
  value=(value<<6);
  byte byte0 = byte(highByte(value));
  byte byte1 = byte(lowByte(value));
  SPI.transfer(command);
  SPI.transfer(byte0);
  SPI.transfer(byte1);
  delay(1);
  digitalWrite(digipot_cs_pin, HIGH);
}

