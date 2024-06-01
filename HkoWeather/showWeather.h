#pragma once

#include <Arduino_JSON.h>

void printValue(int x, int y, int w, int h, int temperature, int humidity)
{
  gfx->setTextBound(x, y, w, h);
  gfx->setCursor(x, y);
  gfx->print(temperature);
  gfx->println(F("\xB0"
                 "C"));
  gfx->print(humidity);
  gfx->println(F("%"));
  gfx->setTextBound(0, 0, gfx->width(), gfx->height());
}

void showWeather(HTTPClient *http, size_t len)
{
  String payload = http->getString();

  JSONVar myObject = JSON.parse(payload);

  int rainfall = (int)myObject["rainfall"]["data"][13]["max"]; // 13: "Yau Tsim Mong"
  Serial.print(myObject["rainfall"]["data"][13]["place"]);
  Serial.print(": myObject[\"rainfall\"][\"data\"][13][\"max\"] = ");
  Serial.println(rainfall);

  double uvindex = (double)myObject["uvindex"]["data"][0]["value"]; // 0: "King's Park"
  Serial.print(myObject["uvindex"]["data"][0]["place"]);
  Serial.print(": myObject[\"uvindex\"][\"data\"][0][\"value\"] = ");
  Serial.println(uvindex);

  int temperature = (int)myObject["temperature"]["data"][0]["value"]; // 0: "King's Park"
  Serial.print(myObject["temperature"]["data"][0]["place"]);
  Serial.print(": myObject[\"temperature\"][\"data\"][0][\"value\"] = ");
  Serial.println(temperature);

  int humidity = (int)myObject["humidity"]["data"][0]["value"]; // 0: "Hong Kong Observatory"
  Serial.print(myObject["humidity"]["data"][0]["place"]);
  Serial.print(": myObject[\"humidity\"][\"data\"][0][\"value\"] = ");
  Serial.println(humidity);

  gfx->fillRect(0, 221, 280, 19, BLACK);
  gfx->setFont(u8g2_font_unifont_t_chinese);
  gfx->setTextColor(WHITE, BLACK);

  char weatherStr[80];
  sprintf(weatherStr, "    雨量: %d, UV: %.1f    ", rainfall, uvindex);
  printCenter(0, 221, gfx->width(), 19, weatherStr);

  gfx->setFont(u8g2_font_fub30_tf);
  gfx->setTextColor(BLACK);
  printValue(186, 154, 92, 64, temperature, humidity);
  printValue(188, 156, 92, 64, temperature, humidity);
  gfx->setTextColor(WHITE);
  printValue(187, 155, 92, 64, temperature, humidity);
}
