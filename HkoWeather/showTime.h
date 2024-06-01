#pragma once

void setClock()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

int lastShowMin = -1;
void showTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("No time available (yet)");
  }
  else
  {
    if (timeinfo.tm_min != lastShowMin)
    {
      gfx->fillRect(0, 0, 280, 29, BLACK);

      gfx->setFont(u8g2_font_ncenB14_tr);
      // gfx->setFont(u8g2_font_unifont_t_chinese);
      gfx->setTextColor(WHITE);

      char timeStr[80];
      strftime(timeStr, 80, "%Y-%m-%d   (%a)   %H:%M", &timeinfo);
      printCenter(0, 0, gfx->width(), 29, timeStr);

      lastShowMin = timeinfo.tm_min;
    }
  }
}
