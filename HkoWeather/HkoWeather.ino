/*******************************************************************************
 * HKO Weather example
 *
 * Dependent libraries:
 * JPEGDEC: https://github.com/bitbank2/JPEGDEC.git
 * Arduino_JSON: https://github.com/arduino-libraries/Arduino_JSON.git
 *
 * Setup steps:
 * 1. Fill your own SSID_NAME, SSID_PASSWORD
 ******************************************************************************/

/* WiFi settings */
const char *SSID_NAME = "YourAP";
const char *SSID_PASSWORD = "PleaseInputYourPasswordHere";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = (8 * 60 * 60);
const int daylightOffset_sec = 0;

const char *WEATHER_PHOTO_URL_TEMPLATE = "https://www.hko.gov.hk/wxinfo/aws/hko_mica/cp1/latest_CP1.jpg?v=%lu123"; // Central Pier
// const char *WEATHER_PHOTO_URL_TEMPLATE = "https://www.hko.gov.hk/wxinfo/aws/hko_mica/hmm/latest_HMM.jpg?v=%lu123"; // Hong Kong Maritime Museum
// const char *WEATHER_PHOTO_URL_TEMPLATE = "https://www.hko.gov.hk/wxinfo/aws/hko_mica/ic1/latest_IC1.jpg?v=%lu123"; // International Commerce Centre

const char *CURRENT_WEATHER_URL = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en";
const char *WEATHER_WARNSUM_URL = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=warnsum&lang=en";

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <U8g2lib.h> // must include before <Arduino_GFX_Library.h>
#include <Arduino_GFX_Library.h>
#define GFX_BL 17
Arduino_DataBus *bus = new Arduino_ESP32SPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 12 /* RST */, 1 /* rotation */, true /* IPS */, 240 /* width */, 280 /* height */, 0 /* col offset 1 */, 20 /* row offset 1 */, 0 /* col offset 2 */, 20 /* row offset 2 */);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include <driver/ledc.h>
#include <esp_task_wdt.h>
#include <WiFi.h>

#include "printCenter.h"
#include "httpsRequest.h"
#include "showJpeg.h"
#include "showTime.h"
#include "showWeather.h"

char url[1024];
unsigned long next_show_millis = 0;

#ifdef GFX_BL
ledc_channel_config_t ledc_channel = {
    .gpio_num = GFX_BL,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_2,
    .timer_sel = LEDC_TIMER_0,
    .duty = 63,
    .hpoint = 0,
};
ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,   // timer mode
    .duty_resolution = LEDC_TIMER_8_BIT, // resolution of PWM duty
    .timer_num = LEDC_TIMER_0,           // timer index
    .freq_hz = 12000,                    // frequency of PWM signal
    .clk_cfg = LEDC_USE_RTC8M_CLK,       // Force source clock to RTC8M
};
#endif

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX WiFi Photo Frame example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin(80000000))
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);
  gfx->setUTF8Print(true); // enable UTF8 support for the Arduino print() function

#ifdef GFX_BL
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);
  ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
  ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
#endif

  Serial.println("Init WiFi");
  gfx->println("Init WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    gfx->print(".");
  }
  Serial.println(" CONNECTED");
  gfx->println(" CONNECTED");

  setClock();

  Serial.println("Init ESP32 WDT");
  gfx->println("Init ESP32 WDT");
  esp_task_wdt_init(30, true); // 30 seconds
  enableLoopWDT();
}

void loop()
{
  if (millis() < next_show_millis)
  {
    // Serial.println("Wait for next minute...");
    delay(1000);
  }
  else
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Reconnect WiFi");
      // gfx->println("Reconnect WiFi");
      WiFi.mode(WIFI_STA);
      WiFi.begin(SSID_NAME, SSID_PASSWORD);
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
        // gfx->print(".");
      }
      Serial.println(" CONNECTED");
      // gfx->println(" CONNECTED");
    }
    else
    {
      // setup url query value with LCD dimension
      time_t now;
      time(&now);
      sprintf(url, WEATHER_PHOTO_URL_TEMPLATE, now);
      Serial.printf("URL: %s\n", url);

      if (httpsRequest(url, &showJpeg))
      {
        if (httpsRequest((char *)CURRENT_WEATHER_URL, &showWeather))
        {
          next_show_millis = (((millis() / 60000L) + 5) * 60000L); // next 5 minutes
        }
      }
    }
  }

  showTime();
  feedLoopWDT();

  esp_sleep_enable_timer_wakeup(20 * 1000000); // light sleep for 20 seconds
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
  esp_light_sleep_start();
}
