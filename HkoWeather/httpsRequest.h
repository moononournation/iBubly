#pragma once

#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char *rootCACertificate =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIF3jCCA8agAwIBAgIUY/4DvYu7lRKkCR7ZPF/tFMDhgdUwDQYJKoZIhvcNAQEL\n"
    "BQAwbzELMAkGA1UEBhMCSEsxEjAQBgNVBAgTCUhvbmcgS29uZzESMBAGA1UEBxMJ\n"
    "SG9uZyBLb25nMRYwFAYDVQQKEw1Ib25na29uZyBQb3N0MSAwHgYDVQQDExdIb25n\n"
    "a29uZyBQb3N0IFJvb3QgQ0EgMzAeFw0xNzA2MDMwNDA3NTBaFw0zMjA2MDMwNDA3\n"
    "NTBaMHoxCzAJBgNVBAYTAkhLMRIwEAYDVQQIEwlIb25nIEtvbmcxEjAQBgNVBAcT\n"
    "CUhvbmcgS29uZzEWMBQGA1UEChMNSG9uZ2tvbmcgUG9zdDErMCkGA1UEAxMiSG9u\n"
    "Z2tvbmcgUG9zdCBlLUNlcnQgU1NMIENBIDMgLSAxNzCCASIwDQYJKoZIhvcNAQEB\n"
    "BQADggEPADCCAQoCggEBAIagJaH0N6+HN+9jlWGSk8cajAmkcma40f97saznEfrH\n"
    "1H1boG6FWDSRZ4Gc0AbPVS3PWEW1nMX9DujF5fnhRYQZeuwviJGbIlEhTYuvF7N/\n"
    "TXPQN1SP+bq1eJIkn0kdhtaWOIajq5vGEHvQ6CFZEE/yEWlrKnH0FNXpRx2Yj7bN\n"
    "+UKOTz5XkABuSV0QYqm6UDt5kRDFE9t/ChshTABxGsEM2vpe9LvKemsuOBtw0lee\n"
    "0I6MOsl0/kSN4kpoLjPeqqGHai3nUXCA+qWp+m+zwdR2OYTxl/TDBZp8wDLKhJTk\n"
    "/AFayjUwl1I+qTnU0WjppV5vwhc5vFBdZGM+RtrFPlkCAwEAAaOCAWUwggFhMBIG\n"
    "A1UdEwEB/wQIMAYBAf8CAQAwfAYIKwYBBQUHAQEEcDBuMD4GCCsGAQUFBzAChjJo\n"
    "dHRwOi8vd3d3MS5ob25na29uZ3Bvc3QuZ292LmhrL3Jvb3Qvcm9vdF9jYV8zLmNy\n"
    "dDAsBggrBgEFBQcwAYYgaHR0cDovL29jc3AxLmhvbmdrb25ncG9zdC5nb3YuaGsw\n"
    "OAYDVR0gBDEwLzAtBgRVHSAAMCUwIwYIKwYBBQUHAgEWF3d3dy5ob25na29uZ3Bv\n"
    "c3QuZ292LmhrMA4GA1UdDwEB/wQEAwIBBjAfBgNVHSMEGDAWgBQXnc0ei9Y5K3DT\n"
    "XNSguB+wAPzFYTBDBgNVHR8EPDA6MDigNqA0hjJodHRwOi8vY3JsMS5ob25na29u\n"
    "Z3Bvc3QuZ292LmhrL2NybC9Sb290Q0EzQVJMLmNybDAdBgNVHQ4EFgQUkjewcJyO\n"
    "eduzGRO4m6UywLfWJ2IwDQYJKoZIhvcNAQELBQADggIBAIXSFgE04Z6AcMFFUWPz\n"
    "e4ZuBav4xr8tbAEDDSbaPRinQ2HZFpw0aIhAG4h4WaDD0dDrNs/w8XrWQY+YngTw\n"
    "6z/cta+ByVu4kGFKGRYLl9tWX8a1oWhETHcQ5uAUf+rT94gv4DWnSKDDNrT0AaGj\n"
    "bo//IF0UdQIBbCsZj0NQCgMouOgYC6zCUv4JO4tgwBwAVEXbFUyXH+rXtE1ZKSs1\n"
    "2XOnIeA8TCvPht5FJm8RPJNSu3QHYMdvle8ZJ7/Sa0uZMvpdMHT4OfQEO0UhP3MS\n"
    "flYUMfUhu6MIQqjSjwjHq9LyJvg0ehunFRot/d98LtApyRecd+sjNcLDf6EGqydE\n"
    "RAGub5g63McG9Lhz/85dSGTNIOjrD8qQyLxchmdvq2FQcsomTVlMXtlM8NXLDmPn\n"
    "Qjyue8q9Q+QO8BTDMSkmFYi5ss7ZzR8d3m4INx08g9TBY3J7iYTXppkputbu+6SF\n"
    "bR26v3uPz89u2L9bnnLBZglcJlDO7jpcGvCy07L5dQE015xsWq8txgC2Esf82HPt\n"
    "wludZKyEPpkqSwtOhpUy/4pnW9Nezj+BPLrgdkmkugwQtfsV/j6LPNdY43/10kz0\n"
    "OQVDufIavloNdaiv+8ATXEq1LXzSuOun4SPYmLNLqS590IGxjRuJe4DlRxeIJpEA\n"
    "HAiv/TLU/E8/SU/AWcwH0ZEz\n"
    "-----END CERTIFICATE-----\n";

typedef void(HTTP_REQUEST_CALLBACK)(HTTPClient *, size_t);

WiFiClientSecure *client;
HTTPClient *http;
WiFiClient *http_stream;

bool httpsRequest(char *url, HTTP_REQUEST_CALLBACK *callback)
{
  if (!client)
  {
    client = new WiFiClientSecure;
    if (!client)
    {
      return false;
    }
    else
    {
      client->setCACert(rootCACertificate);
    }
  }

  Serial.printf("[HTTPS] begin...\n");

  if (!http)
  {
    http = new HTTPClient;
  }

  http->begin(*client, url);
  int httpCode = http->GET();

  Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  // HTTP header has been send and Server response header has been handled
  if (httpCode <= 0)
  {
    // gfx->printf("[HTTPS] GET... code: %d\n", httpCode);
    Serial.printf("[HTTPS] GET... failed, error: %s\n", http->errorToString(httpCode).c_str());
    // gfx->printf("[HTTPS] GET... failed, error: %s\n", http->errorToString(httpCode).c_str());

    http->end();
    return false;
  }
  else
  {
    if (httpCode != 200)
    {
      // gfx->printf("[HTTPS] GET... code: %d\n", httpCode);
      Serial.printf("[HTTP] Not OK!\n");
      // gfx->printf("[HTTP] Not OK!\n");

      http->end();
      return false;
    }
    else
    {
      int len = http->getSize();
      Serial.printf("[HTTP] size: %d\n", len);

      unsigned long start = millis();

      (*callback)(http, len);

      Serial.printf("Time used: %lu\n", millis() - start);
    }
  }

  http->end();
  return true;
}

static int32_t readStream(HTTPClient *http, uint8_t *buf, int32_t len)
{
  WiFiClient *http_stream = http->getStreamPtr();

  size_t r = 0;
  while (r < len)
  {
    r += http_stream->readBytes(buf + r, len - r);
    // Serial.printf("1st byte: %d, 2nd byte: %d, last byte: %d, len: %d, r: %d\n", buf[0], buf[1], buf[r - 1], len, r);
  }

  return r;
}
