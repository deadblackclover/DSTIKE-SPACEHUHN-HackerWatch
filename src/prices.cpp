#include <prices.h>

PriceResult fetchPrices(WiFiClientSecure *client, HTTPClient *http,
                        const char *ids[], int idsLength,
                        const char *vsCurrency)
{
  PriceResult result{false, JsonDocument(), ""};

  if (!client || !http)
  {
    Serial.println("Null client or http");
    return result;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected");
    return result;
  }

  if (idsLength <= 0)
  {
    Serial.println("No coin IDs provided");
    return result;
  }

  String idsParam;
  idsParam.reserve(idsLength * 20);
  for (int i = 0; i < idsLength; i++)
  {
    if (!ids[i])
      continue;
    if (i > 0)
      idsParam += ',';
    idsParam += ids[i];
  }

  if (idsParam.length() == 0)
  {
    Serial.println("All IDs were null");
    return result;
  }

  String URL = "https://api.coingecko.com/api/v3/simple/price?ids=" +
               idsParam + "&vs_currencies=" + vsCurrency;

  if (URL.length() > 2048)
  {
    Serial.println("URL too long");
    return result;
  }

  http->begin(*client, URL);
  int httpCode = http->GET();

  Serial.print("GET ");
  Serial.println(URL);

  if (httpCode > 0)
  {
    String payload = http->getString();
    if (httpCode == HTTP_CODE_OK)
    {
      DeserializationError err = deserializeJson(result.doc, payload);
      if (!err)
      {
        result.success = true;
      }
      else
      {
        Serial.println("JSON parse error: " + String(err.c_str()));
      }
    }
    else
    {
      Serial.println("HTTP error " + String(httpCode) + ": " + payload);
    }
  }
  else
  {
    Serial.println("Connection error: " + http->errorToString(httpCode));
  }

  http->end();
  return result;
}
