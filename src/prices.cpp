#include <prices.h>

JsonDocument fetchPrices(WiFiClientSecure* client, HTTPClient* http, const char* ids[], int idsLength) {
  JsonDocument doc;

  if (WiFi.status() != WL_CONNECTED) return doc;

  String idsParam = "";
  for (int i = 0; i < idsLength; i++) {
    if (i > 0) idsParam += ",";
    idsParam += ids[i];
  }

  String URL = "https://api.coingecko.com/api/v3/simple/price?ids=" + idsParam + "&vs_currencies=usd";

  http->begin((*client), URL);
  int httpCode = http->GET();
  Serial.printf("GET %s\n", URL.c_str());

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http->getString();
      deserializeJson(doc, payload);
    }
  } else {
    Serial.printf("Error when requesting the price, code: %s\n", http->errorToString(httpCode).c_str());
  }

  http->end();
  client->stop();

  return doc;
}
