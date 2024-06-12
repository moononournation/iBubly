/*******************************************************************************
 * LVGL HKO Weather example
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

#include <lvgl.h>
// #define DIRECT_MODE // Uncomment to enable full frame buffer
#include "src/ui/ui.h"

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#define GFX_BL 17
Arduino_DataBus *bus = new Arduino_ESP32SPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 12 /* RST */, 1 /* rotation */, true /* IPS */, 240 /* width */, 280 /* height */, 0 /* col offset 1 */, 20 /* row offset 1 */, 0 /* col offset 2 */, 20 /* row offset 2 */);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/*******************************************************************************
 * Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"

#include <esp_task_wdt.h>
#include <WiFi.h>

uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf;
lv_disp_drv_t disp_drv;
lv_obj_t *tabview;
int cur_tab_idx = 0;
lv_obj_t *ui_Image[3];
size_t weatherPhotoBitmapSize[3] = {0, 0, 0};
uint8_t *weatherPhotoBitmap[3];
lv_img_dsc_t lv_img_dsc[3];
unsigned long next_photo_load_millis[] = {0, 0, 0};
char textBuf[1024];

#include "httpsRequest.h"
#include "decodeJpeg.h"
#include "getWeather.h"
#include "setClock.h"

#ifdef GFX_BL
#include <driver/ledc.h>
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

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
#ifndef DIRECT_MODE
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif
#endif // #ifndef DIRECT_MODE

  lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("iBubly LVGL HKO Weather");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin(80000000))
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);
  ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
  ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
#endif

  Serial.println("Init WiFi");
  // gfx->println("Init WiFi");
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

  setClock();

  Serial.println("Init ESP32 WDT");
  // gfx->println("Init ESP32 WDT");
  esp_task_wdt_init(30, true); // 30 seconds
  enableLoopWDT();

  // Init touch device
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

#ifdef DIRECT_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 40;
#endif

#ifdef ESP32
#if defined(DIRECT_MODE) && defined(RGB_PANEL)
  disp_draw_buf = (lv_color_t *)gfx->getFramebuffer();
#else  // !DIRECT_MODE
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif // !DIRECT_MODE
#else  // !ESP32
  Serial.println("LVGL draw_buf allocate MALLOC_CAP_INTERNAL failed! malloc again...");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif // !ESP32
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, bufSize);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
#ifdef DIRECT_MODE
    disp_drv.direct_mode = true;
#endif
    lv_disp_drv_register(&disp_drv);

    /* Initialize the input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* Init SquareLine prepared UI */
    ui_init();

    tabview = lv_tabview_create(ui_Screen1, LV_DIR_BOTTOM, 40);
    lv_obj_set_width(tabview, 280);
    lv_obj_set_height(tabview, 202);
    lv_obj_set_x(tabview, 0);
    lv_obj_set_y(tabview, 38);
    lv_obj_move_background(tabview);
    lv_obj_set_style_text_font(tabview, &ui_font_Font2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "中環碼頭");
    lv_obj_clear_flag(tab1, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "海事博物館");
    lv_obj_clear_flag(tab2, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "環球貿易");
    lv_obj_clear_flag(tab3, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_Image[0] = lv_img_create(tab1);
    lv_obj_set_width(ui_Image[0], LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Image[0], LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Image[0], LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image[0], LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_Image[0], LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_Image[1] = lv_img_create(tab2);
    lv_obj_set_width(ui_Image[1], LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Image[1], LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Image[1], LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image[1], LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_Image[1], LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_Image[2] = lv_img_create(tab3);
    lv_obj_set_width(ui_Image[2], LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Image[2], LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Image[2], LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image[2], LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_Image[2], LV_OBJ_FLAG_SCROLLABLE); /// Flags
  }

  Serial.println("Setup done");
}

void loop()
{
  cur_tab_idx = lv_tabview_get_tab_act(tabview);
  // Serial.println(cur_tab_idx);
  if (millis() >= next_photo_load_millis[cur_tab_idx])
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
      sprintf(textBuf, WEATHER_PHOTO_URL_TEMPLATE[cur_tab_idx], now);
      Serial.printf("URL: %s\n", textBuf);

      if (httpsRequest(textBuf, &decodeJpeg))
      {
        if (httpsRequest((char *)CURRENT_WEATHER_URL, &getWeather))
        {
          if (httpsRequest((char *)WEATHER_WARNSUM_URL, &getWarning))
          {
            next_photo_load_millis[cur_tab_idx] = (((millis() / 60000L) + 5) * 60000L); // next 5 minutes
          }
        }
      }
    }
  }

  updateDateTime();

  feedLoopWDT();

  lv_timer_handler(); /* let the GUI do its work */

#ifdef DIRECT_MODE
#ifdef RGB_PANEL
  gfx->flush();
#else
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)draw_buf, screenWidth, screenHeight);
#endif
#endif // #ifdef DIRECT_MODE

  // delay(5);
}
