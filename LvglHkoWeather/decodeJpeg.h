#pragma once

#include <JPEGDEC.h>
JPEGDEC jpegdec;
uint8_t *buf;
size_t buf_allocated_size = 0;

int16_t weatherPhotoBitmapW;
int16_t weatherPhotoBitmapH;

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx_draw_bitmap_to_framebuffer(
      pDraw->pPixels, pDraw->iWidth, pDraw->iHeight,
      (uint16_t *)weatherPhotoBitmap[cur_tab_idx], pDraw->x, pDraw->y, weatherPhotoBitmapW, weatherPhotoBitmapH);

  return 1;
}

void decodeJpeg(HTTPClient *http, size_t len)
{
  if (len <= 0)
  {
    Serial.printf("[HTTP] Unknow content size: %d\n", len);
    gfx->printf("[HTTP] Unknow content size: %d\n", len);
  }
  else
  {
    if (buf_allocated_size == 0)
    {
      buf = (uint8_t *)malloc(len);
      buf_allocated_size = len;
    }
    else if (buf_allocated_size < len)
    {
      buf = (uint8_t *)realloc(buf, len);
      buf_allocated_size = len;
    }

    size_t r = readStream(http, buf, len);
    // Serial.printf("%c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], buf[18], buf[19], buf[20], buf[21], buf[22], buf[23], buf[24], buf[25], buf[26], buf[27], buf[28], buf[29], buf[30], buf[31]);
    // Serial.printf("%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], buf[18], buf[19], buf[20], buf[21], buf[22], buf[23], buf[24], buf[25], buf[26], buf[27], buf[28], buf[29], buf[30], buf[31]);
    if (r != len)
    {
      Serial.print(F("HTTP stream read failed!"));
    }
    else
    {
      int jpeg_result = jpegdec.openRAM(buf, len, jpegDrawCallback);

      if (!jpeg_result)
      {
        Serial.print(F("JPEG open failed!"));
      }
      else
      {
        weatherPhotoBitmapW = jpegdec.getWidth();
        weatherPhotoBitmapH = jpegdec.getHeight();

        size_t imgBitmapSize = weatherPhotoBitmapW * weatherPhotoBitmapH * 2;
        if (weatherPhotoBitmapSize[cur_tab_idx] == 0)
        {
          weatherPhotoBitmap[cur_tab_idx] = (uint8_t *)malloc(imgBitmapSize);
          weatherPhotoBitmapSize[cur_tab_idx] = imgBitmapSize;
        }
        else if (weatherPhotoBitmapSize[cur_tab_idx] < imgBitmapSize)
        {
          weatherPhotoBitmap[cur_tab_idx] = (uint8_t *)realloc(weatherPhotoBitmap[cur_tab_idx], imgBitmapSize);
          weatherPhotoBitmapSize[cur_tab_idx] = imgBitmapSize;
        }
        if (weatherPhotoBitmap[cur_tab_idx])
        {
#if (LV_COLOR_16_SWAP != 0)
          jpegdec.setPixelType(RGB565_BIG_ENDIAN);
#endif
          int jpeg_result = jpegdec.decode(0, 0, 0 /* scale */);

          if (!jpeg_result)
          {
            Serial.print(F("JPEG decode failed!"));
          }
          else
          {
            lv_img_dsc[cur_tab_idx].header.cf = LV_IMG_CF_TRUE_COLOR;
            lv_img_dsc[cur_tab_idx].header.w = weatherPhotoBitmapW;
            lv_img_dsc[cur_tab_idx].header.h = weatherPhotoBitmapH;
            lv_img_dsc[cur_tab_idx].header.always_zero = 0;
            lv_img_dsc[cur_tab_idx].data_size = imgBitmapSize;
            lv_img_dsc[cur_tab_idx].data = weatherPhotoBitmap[cur_tab_idx];

            uint16_t zW = gfx->width() * 256 / weatherPhotoBitmapW;
            uint16_t zH = gfx->height() * 256 / weatherPhotoBitmapH;
            Serial.printf("zW: %d, zH: %d\n", zW, zH);
            lv_img_set_zoom(ui_Image[cur_tab_idx], (zW < zH) ? zW : zH);
            lv_img_set_src(ui_Image[cur_tab_idx], &lv_img_dsc[cur_tab_idx]);

            // lv_obj_invalidate(ui_Screen1);
          }
        }
      }
      jpegdec.close();
    }
  }
}
