// ========== WI-FI SETTINGS ==========
const char* ssid     = "SSID";
const char* password = "PASSWORD";

// ========== TIME SETTINGS ==========
#define NTP_SERVER      "pool.ntp.org"
#define TIME_ZONE       7          // GMT+7
#define NTP_UPDATE_MIN  60         // NTP synchronization interval in minutes

// ========== PRICES SETTINGS ==========
const char* pricesIds[] = {"bitcoin", "ethereum"};
int pricesIdsLength = 2;
