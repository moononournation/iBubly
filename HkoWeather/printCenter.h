#pragma once

void printCenter(int16_t x, int16_t y, int16_t w, int16_t h, const char *str)
{
  int16_t xOut, yOut;
  uint16_t wOut, hOut;
  gfx->getTextBounds(str, 0, 0, &xOut, &yOut, &wOut, &hOut);
  // Serial.printf("xOut: %d, yOut: %d, wOut: %d, hOut: %d\n", xOut, yOut, wOut, hOut);
  gfx->setCursor(x + ((w - wOut) / 2) - xOut, y + ((h - hOut) / 2) - yOut);
  gfx->print(str);
}
