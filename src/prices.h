#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

JsonDocument fetchPrices(WiFiClientSecure* client, HTTPClient* http, const char* ids[], int idsLength);
