#pragma once

#include <Arduino_JSON.h>

void getWeather(HTTPClient *http, size_t len)
{
  String payload = http->getString();

  JSONVar myObject = JSON.parse(payload);

  int rainfall = (int)myObject["rainfall"]["data"][13]["max"]; // 13: "Yau Tsim Mong"
  Serial.print(myObject["rainfall"]["data"][13]["place"]);
  Serial.print(": myObject[\"rainfall\"][\"data\"][13][\"max\"] = ");
  Serial.println(rainfall);

  int icon = (int)myObject["icon"][0];
  Serial.print(": myObject[\"icon\"][0] = ");
  Serial.println(icon);

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

  sprintf(textBuf, "%d℃  %d%%", temperature, humidity);
  lv_label_set_text(ui_Label1, textBuf);
  sprintf(textBuf, " 雨: %d UV: %.1f", rainfall, uvindex);
  lv_label_set_text(ui_Label2, textBuf);

  switch (icon)
  {
  case 50:
    lv_img_set_src(ui_Icon1, &ui_img_pic50_png);
    break;
  case 51:
    lv_img_set_src(ui_Icon1, &ui_img_pic51_png);
    break;
  case 52:
    lv_img_set_src(ui_Icon1, &ui_img_pic52_png);
    break;
  case 53:
    lv_img_set_src(ui_Icon1, &ui_img_pic53_png);
    break;
  case 54:
    lv_img_set_src(ui_Icon1, &ui_img_pic54_png);
    break;
  case 60:
    lv_img_set_src(ui_Icon1, &ui_img_pic60_png);
    break;
  case 61:
    lv_img_set_src(ui_Icon1, &ui_img_pic61_png);
    break;
  case 62:
    lv_img_set_src(ui_Icon1, &ui_img_pic62_png);
    break;
  case 63:
    lv_img_set_src(ui_Icon1, &ui_img_pic63_png);
    break;
  case 64:
    lv_img_set_src(ui_Icon1, &ui_img_pic64_png);
    break;
  case 65:
    lv_img_set_src(ui_Icon1, &ui_img_pic65_png);
    break;
  case 70:
    lv_img_set_src(ui_Icon1, &ui_img_pic70_png);
    break;
  case 71:
    lv_img_set_src(ui_Icon1, &ui_img_pic71_png);
    break;
  case 72:
    lv_img_set_src(ui_Icon1, &ui_img_pic72_png);
    break;
  case 73:
    lv_img_set_src(ui_Icon1, &ui_img_pic73_png);
    break;
  case 74:
    lv_img_set_src(ui_Icon1, &ui_img_pic74_png);
    break;
  case 75:
    lv_img_set_src(ui_Icon1, &ui_img_pic75_png);
    break;
  case 76:
    lv_img_set_src(ui_Icon1, &ui_img_pic76_png);
    break;
  case 77:
    lv_img_set_src(ui_Icon1, &ui_img_pic77_png);
    break;
  case 80:
    lv_img_set_src(ui_Icon1, &ui_img_pic80_png);
    break;
  case 81:
    lv_img_set_src(ui_Icon1, &ui_img_pic81_png);
    break;
  case 82:
    lv_img_set_src(ui_Icon1, &ui_img_pic82_png);
    break;
  case 83:
    lv_img_set_src(ui_Icon1, &ui_img_pic83_png);
    break;
  case 84:
    lv_img_set_src(ui_Icon1, &ui_img_pic84_png);
    break;
  case 85:
    lv_img_set_src(ui_Icon1, &ui_img_pic85_png);
    break;
  case 90:
    lv_img_set_src(ui_Icon1, &ui_img_pic90_png);
    break;
  case 91:
    lv_img_set_src(ui_Icon1, &ui_img_pic91_png);
    break;
  case 92:
    lv_img_set_src(ui_Icon1, &ui_img_pic92_png);
    break;
  case 93:
    lv_img_set_src(ui_Icon1, &ui_img_pic93_png);
    break;
  }
}
