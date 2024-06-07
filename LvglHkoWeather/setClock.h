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
void updateDateTime()
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
      strftime(textBuf, 1024, "%Y %b %d   (%a)   %H:%M", &timeinfo);
      lv_label_set_text(ui_LabelDate, textBuf);

      lastShowMin = timeinfo.tm_min;
    }
  }
}