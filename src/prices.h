#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

struct PriceResult
{
    bool success;
    JsonDocument doc;
};

PriceResult fetchPrices(WiFiClientSecure *client, HTTPClient *http, const char *ids[], int idsLength, const char *vsCurrency = "usd");
