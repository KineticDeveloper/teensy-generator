#include <U8g2lib.h>
#include "screen.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void printScreen(char *msg)
{
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0, 20);
  u8g2.println(msg);
  u8g2.sendBuffer();
}

