#include <config.h>

#include <NTPClient.h>
#include <SH1106Wire.h>
#include <Wire.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define I2C_ADDR        0x3C
#define I2C_SDA         5
#define I2C_SCL         4

#define BUTTON_UP 12
#define BUTTON_DOWN 13
#define BUTTON_A 14

SH1106Wire display = SH1106Wire(I2C_ADDR, I2C_SDA, I2C_SCL);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TIME_ZONE * 3600, NTP_UPDATE_MIN * 60000);
