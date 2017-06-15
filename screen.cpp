#include <U8g2lib.h>
#include "screen.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

const int fontHeight = 14;
const int fontWidth = 7;
const int lineSpacing = 2;

void clearScreen()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x14_tr);
}

void printScreen(unsigned int line, String msg)
{
  u8g2.setCursor(0, (fontHeight + lineSpacing) * line);
  u8g2.println(msg);
}

void sendBuffer()
{
  u8g2.sendBuffer();
}

void drawCursor(unsigned int column, unsigned int line)
{
  u8g2.drawHLine((column-1) * fontWidth, line * (fontHeight+lineSpacing)+lineSpacing/2, fontWidth);
}

