#pragma once

#include <Arduino_JSON.h>

const char *WEATHER_PHOTO_URL_TEMPLATE[] = {
    "https://www.hko.gov.hk/wxinfo/aws/hko_mica/cp1/latest_CP1.jpg?v=%lu123", // Central Pier
    "https://www.hko.gov.hk/wxinfo/aws/hko_mica/hmm/latest_HMM.jpg?v=%lu123", // Hong Kong Maritime Museum
    "https://www.hko.gov.hk/wxinfo/aws/hko_mica/ic1/latest_IC1.jpg?v=%lu123"  // International Commerce Centre
};

const char *CURRENT_WEATHER_URL = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en";
const char *WEATHER_WARNSUM_URL = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=warnsum&lang=en";

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

  sprintf(textBuf, "%d℃ %d%%", temperature, humidity);
  lv_label_set_text(ui_Label1, textBuf);
  sprintf(textBuf, "降雨: %d mm\nUV: %.1f", rainfall, uvindex);
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

void getWarning(HTTPClient *http, size_t len)
{
  String payload = http->getString();

  JSONVar myObject = JSON.parse(payload);

  int cur_warn_icon_idx = 0;
  lv_obj_t *warn_icons[] = {
      ui_Icon2,
      ui_Icon3,
      ui_Icon4,
      ui_Icon5,
      ui_Icon6,
      ui_Icon7,
      ui_Icon8,
      ui_Icon9};
  int warn_icon_count = 8;

  if (myObject.hasOwnProperty("WFIRE"))
  {
    String code = myObject["WFIRE"]["code"];
    String actionCode = myObject["WFIRE"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      if (code.equals("WFIREY"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_firey_png);
      }
      else if (code.equals("WFIRER"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_firer_png);
      }
    }
  }
  if (myObject.hasOwnProperty("WFROST"))
  {
    String actionCode = myObject["WFROST"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_frost_png);
    }
  }
  if (myObject.hasOwnProperty("WHOT"))
  {
    String actionCode = myObject["WHOT"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_vhot_png);
    }
  }
  if (myObject.hasOwnProperty("WCOLD"))
  {
    String actionCode = myObject["WCOLD"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_cold_png);
    }
  }
  if (myObject.hasOwnProperty("WMSGNL"))
  {
    String actionCode = myObject["WMSGNL"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_sms_png);
    }
  }
  if (myObject.hasOwnProperty("WRAIN"))
  {
    String code = myObject["WRAIN"]["code"];
    String actionCode = myObject["WRAIN"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      if (code.equals("WRAINA"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_raina_png);
      }
      else if (code.equals("WRAINR"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_rainr_png);
      }
      else if (code.equals("WRAINB"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_rainb_png);
      }
    }
  }
  if (myObject.hasOwnProperty("WFNTSA"))
  {
    String actionCode = myObject["WFNTSA"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_ntfl_png);
    }
  }
  if (myObject.hasOwnProperty("WL"))
  {
    String actionCode = myObject["WL"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_landslip_png);
    }
  }
  if (myObject.hasOwnProperty("WTCSGNL"))
  {
    String code = myObject["WTCSGNL"]["code"];
    String actionCode = myObject["WTCSGNL"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      if (code.equals("TC1"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc1_png);
      }
      else if (code.equals("TC3"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc3_png);
      }
      else if (code.equals("TC8NE"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc8ne_png);
      }
      else if (code.equals("TC8SE"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc8b_png);
      }
      else if (code.equals("TC8NW"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc8d_png);
      }
      else if (code.equals("TC8SW"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc8c_png);
      }
      else if (code.equals("TC9"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc9_png);
      }
      else if (code.equals("TC10"))
      {
        lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tc10_png);
      }
    }
  }
  if (myObject.hasOwnProperty("WTMW"))
  {
    String actionCode = myObject["WTMW"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_tsunami_png);
    }
  }
  if (myObject.hasOwnProperty("WTS"))
  {
    String actionCode = myObject["WTS"]["actionCode"];
    if (!actionCode.equals("CANCEL"))
    {
      lv_img_set_src(warn_icons[cur_warn_icon_idx++], &ui_img_ts_png);
    }
  }

  // clear remain icons
  for (; cur_warn_icon_idx < warn_icon_count;)
  {
    lv_img_set_src(warn_icons[cur_warn_icon_idx++], NULL);
  }
}
