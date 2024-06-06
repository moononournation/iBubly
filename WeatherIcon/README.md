# HKO Weather Icon Sources

 * https://www.hko.gov.hk/textonly/v2/explain/wxicon_e.htm
 * https://data.weather.gov.hk/weatherAPI/doc/HKO_Open_Data_API_Documentation.pdf
 * https://rss.weather.gov.hk/rsse.html

# Download

```Console
wget -i hko_weather_icon_list.txt
```

# Convert GIF to PNG

```Console
mogrify -format png *.gif
```
