#pragma once

#include <JPEGDEC.h>
JPEGDEC jpegdec;
uint8_t *buf;
size_t buf_allocated_size = 0;

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  return 1;
}

void showJpeg(HTTPClient *http, size_t len)
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
        gfx->fillScreen(BLACK);

        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t w = gfx->width();
        uint16_t h = gfx->height();

        // scale to fit height
        int scale;
        int iMaxMCUs;
        float ratio = (float)jpegdec.getHeight() / h;
        if (ratio <= 1)
        {
          scale = 0;
          iMaxMCUs = w / 16;
          // x = (w - jpegdec.getWidth()) / 2;
          y = (h - jpegdec.getHeight()) / 2;
        }
        else if (ratio <= 2)
        {
          scale = JPEG_SCALE_HALF;
          iMaxMCUs = w / 8;
          // x = (w - (jpegdec.getWidth() / 2)) / 2;
          y = (h - (jpegdec.getHeight() / 2)) / 2;
        }
        else if (ratio <= 4)
        {
          scale = JPEG_SCALE_QUARTER;
          iMaxMCUs = w / 4;
          // x = (w - (jpegdec.getWidth() / 4)) / 2;
          y = (h - (jpegdec.getHeight() / 4)) / 2;
        }
        else
        {
          scale = JPEG_SCALE_EIGHTH;
          iMaxMCUs = w / 2;
          // x = (w - (jpegdec.getWidth() / 8)) / 2;
          y = (h - (jpegdec.getHeight() / 8)) / 2;
        }
        jpegdec.setMaxOutputSize(iMaxMCUs);
        jpegdec.setPixelType(RGB565_BIG_ENDIAN);
        if (!jpegdec.decode(x, y, scale))
        {
          Serial.print(F("JPEG decode failed!"));
        }
        jpegdec.close();
      }
    }
  }
}
