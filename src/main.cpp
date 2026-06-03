#include <main.h>

#include <ESP8266WiFi.h>

unsigned long lastDisplayUpdate = 0;
String lastTimeStr = "";

// The function of rendering time in large font in the center
void drawTime(String timeStr) {
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

void setup() {
  Serial.begin(115200);

  // Display
  display.init();
  display.setContrast(255);
  display.flipScreenVertically();
  display.clear();
  display.display();

  // Connecting to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Launching the NTP client and the first synchronization
  timeClient.begin();
  timeClient.update();
}

void loop() {
  // Scheduled NTP updates
  timeClient.update();

  // Updating the display once per second (or when the time changes)
  String currentTime = timeClient.getFormattedTime(); // "HH:MM:SS"
  String timeHM = currentTime.substring(0, 5);       // "HH:MM"

  if (timeHM != lastTimeStr) {
    lastTimeStr = timeHM;
    drawTime(timeHM);
  }

  delay(200); // a small delay for stability
}
