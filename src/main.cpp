#include <fetch.h>
#include <main.h>
#include <prices.h>

#include <ESP8266WiFi.h>

volatile long lastUpdate = 0;
long interval = 30;

int currentScreen = 0;
int currentCoinIndex = 0;

String lastTimeStr = "";

JsonDocument pricesDoc;
bool pricesValid = false;

// The function of rendering time in large font in the center
void drawTime(String timeStr)
{
  display.clear();

  display.setFont(ArialMT_Plain_24);
  display.setColor(WHITE);

  // Calculating the coordinates for centering
  uint16_t w = display.getStringWidth(timeStr);
  int x = (SCREEN_WIDTH - w) / 2;
  int y = SCREEN_HEIGHT / 2 - 12;

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, timeStr);
  display.display();
}

void displayPriceScreen(String title, float price)
{
  display.clear();

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 10, title);

  display.setFont(ArialMT_Plain_24);
  String priceStr = String(price, 2);
  display.drawString(64, 35, "$" + priceStr);
  display.display();
}

void setup()
{
  Serial.begin(115200);

  // Display
  display.init();
  display.setContrast(255);
  display.flipScreenVertically();
  display.clear();
  display.display();

  // Button
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);

  // Connecting to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Launching the NTP client and the first synchronization
  timeClient.begin();
  timeClient.update();

  wifiClientSecure.setInsecure();
}

void loop()
{
  // Scheduled NTP updates
  timeClient.update();

  // Updating the display once per second (or when the time changes)
  String currentTime = timeClient.getFormattedTime();
  String timeHM = currentTime.substring(0, 5);

  long now = timeClient.getEpochTime();

  if (now - lastUpdate >= interval)
  {
    Serial.println("Update price (now=" + String(now) + ")");
    lastUpdate = now;
    PriceResult result = fetchPrices(&wifiClientSecure, &httpClient,
                                     pricesIds, pricesIdsLength);
    if (result.success)
    {
      pricesDoc = std::move(result.doc);
      pricesValid = true;
    }
    else
    {
      Serial.println("Price fetch error: " + result.errorMsg);
      pricesValid = false;
    }
  }

  if (digitalRead(BUTTON_A) == LOW)
  {
    Serial.println("BUTTON_A pressed");
    currentScreen = 1 - currentScreen;
    delay(200);
  }

  if (currentScreen == 1)
  {
    if (digitalRead(BUTTON_UP) == LOW)
    {
      Serial.println("BUTTON_UP pressed");
      currentCoinIndex = (currentCoinIndex - 1 + pricesIdsLength) % pricesIdsLength;
      delay(200);
    }
    if (digitalRead(BUTTON_DOWN) == LOW)
    {
      Serial.println("BUTTON_DOWN pressed");
      currentCoinIndex = (currentCoinIndex + 1) % pricesIdsLength;
      delay(200);
    }
  }

  if (currentScreen == 0)
  {
    if (timeHM != lastTimeStr)
    {
      lastTimeStr = timeHM;
    }
    drawTime(timeHM);
  }
  else
  {
    if (pricesValid)
    {
      const char *coinId = pricesIds[currentCoinIndex];

      String coinName = String(coinId);
      coinName.toUpperCase();

      JsonVariant priceVar = pricesDoc[coinId]["usd"];
      if (!priceVar.isNull())
      {
        float price = priceVar.as<float>();
        displayPriceScreen(coinName, price);
      }
      else
      {
        Serial.println("No data for " + coinName);
        currentScreen = 0;
      }
    }
    else
    {
      Serial.println("Price unavailable");
      currentScreen = 0;
    }
  }

  delay(100);
}
