/*
  JSON Object

  This sketch demonstrates how to use various features
  of the Official Arduino_JSON library, in particular for JSON objects.

  This example code is in the public domain.

  https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en
*/

#include <Arduino_JSON.h>
#include <assert.h>

const char input[] = "{\"rainfall\":{\"data\":[{\"unit\":\"mm\",\"place\":\"Central & Western District\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Eastern District\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Kwai Tsing\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Islands District\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"North District\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Sai Kung\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Sha Tin\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Southern District\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Tai Po\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Tsuen Wan\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Tuen Mun\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Wan Chai\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Yuen Long\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Yau Tsim Mong\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Sham Shui Po\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Kowloon City\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Wong Tai Sin\",\"max\":0,\"main\":\"FALSE\"},{\"unit\":\"mm\",\"place\":\"Kwun Tong\",\"max\":0,\"main\":\"FALSE\"}],\"startTime\":\"2024-05-07T09:45:00+08:00\",\"endTime\":\"2024-05-07T10:45:00+08:00\"},\"icon\":[51],\"iconUpdateTime\":\"2024-05-07T06:00:00+08:00\",\"uvindex\":{\"data\":[{\"place\":\"King's Park\",\"value\":5,\"desc\":\"moderate\"}],\"recordDesc\":\"During the past hour\"},\"updateTime\":\"2024-05-07T11:02:00+08:00\",\"temperature\":{\"data\":[{\"place\":\"King's Park\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Hong Kong Observatory\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Wong Chuk Hang\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Ta Kwu Ling\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Lau Fau Shan\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Tai Po\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Sha Tin\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Tuen Mun\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Tseung Kwan O\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Sai Kung\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Cheung Chau\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Chek Lap Kok\",\"value\":28,\"unit\":\"C\"},{\"place\":\"Tsing Yi\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Shek Kong\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Tsuen Wan Ho Koon\",\"value\":28,\"unit\":\"C\"},{\"place\":\"Tsuen Wan Shing Mun Valley\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Hong Kong Park\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Shau Kei Wan\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Kowloon City\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Happy Valley\",\"value\":32,\"unit\":\"C\"},{\"place\":\"Wong Tai Sin\",\"value\":32,\"unit\":\"C\"},{\"place\":\"Stanley\",\"value\":30,\"unit\":\"C\"},{\"place\":\"Kwun Tong\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Sham Shui Po\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Kai Tak Runway Park\",\"value\":29,\"unit\":\"C\"},{\"place\":\"Yuen Long Park\",\"value\":31,\"unit\":\"C\"},{\"place\":\"Tai Mei Tuk\",\"value\":30,\"unit\":\"C\"}],\"recordTime\":\"2024-05-07T11:00:00+08:00\"},\"warningMessage\":\"\",\"mintempFrom00To09\":\"\",\"rainfallFrom00To12\":\"\",\"rainfallLastMonth\":\"\",\"rainfallJanuaryToLastMonth\":\"\",\"tcmessage\":\"\",\"humidity\":{\"recordTime\":\"2024-05-07T11:00:00+08:00\",\"data\":[{\"unit\":\"percent\",\"value\":72,\"place\":\"Hong Kong Observatory\"}]}}";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  demoParse();
}

void loop() {
}

void demoParse() {
  Serial.println("parse");
  Serial.println("=====");

  JSONVar myObject = JSON.parse(input);

  // JSON.typeof(jsonVar) can be used to get the type of the variable
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  Serial.print("JSON.typeof(myObject) = ");
  Serial.println(JSON.typeof(myObject)); // prints: object

  // myObject.hasOwnProperty(key) checks if the object contains an entry for key
  if (myObject.hasOwnProperty("rainfall")) {
    Serial.print("myObject[\"rainfall\"] = ");
    Serial.println(myObject["rainfall"]);

    Serial.print("myObject[\"rainfall\"][\"data\"] = ");
    Serial.println(myObject["rainfall"]["data"]);

    Serial.print("myObject[\"rainfall\"][\"data\"][0] = ");
    Serial.println(myObject["rainfall"]["data"][0]);

    Serial.print("myObject[\"rainfall\"][\"data\"][0][\"max\"] = ");
    Serial.println(myObject["rainfall"]["data"][0]["max"]);
  }

  if (myObject.hasOwnProperty("uvindex")) {
    Serial.print("myObject[\"uvindex\"][\"data\"][0][\"value\"] = ");
    Serial.println(myObject["uvindex"]["data"][0]["value"]);
  }

  if (myObject.hasOwnProperty("temperature")) {
    Serial.print("myObject[\"temperature\"][\"data\"][0][\"value\"] = ");
    Serial.println(myObject["temperature"]["data"][0]["value"]);
  }

  if (myObject.hasOwnProperty("humidity")) {
    Serial.print("myObject[\"humidity\"][\"data\"][0][\"value\"] = ");
    Serial.println(myObject["humidity"]["data"][0]["value"]);
  }

  // JSONVars can be printed using print or println
  Serial.print("myObject = ");
  Serial.println(myObject);

  Serial.println();
}
