/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"
#include <Adafruit_ST7789.h> 
#include <Adafruit_GFX.h>  //Libraries for OLED Display
#include <SPI.h>
#include <Arduino.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* ssid     = "xxxxxxx";
const char* password = "xxxxxxxxxxx";

const char* ntpServer = "pool.ntp.org";
//For MASS (UTC -5) -18000
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

void setup(){
  Serial.begin(115200);

    // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  // Connect to Wi-Fi
  tft.print("Connecting to ");
  tft.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
  tft.println("");
  tft.println("WiFi connected.");
      //// Delay 1 sec, Clear the screen and then set the cursor back to upper left corner -Ben
    delay(1000);
    tft.fillScreen(0);
    tft.setCursor(0, 0);
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(1000);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    tft.println("Failed to obtain time");
    return;
  }
  tft.fillScreen(0);
  tft.setTextSize(5);
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(&timeinfo, "%H:%M:%S");
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLUE);
  tft.println(&timeinfo, "%A");  
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_GREEN);
  tft.println(&timeinfo, "%B %d,");
  tft.println(&timeinfo, "%Y");
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
}