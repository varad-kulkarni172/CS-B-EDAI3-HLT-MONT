#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3o12SLJzj"
#define BLYNK_TEMPLATE_NAME "Healthcare Device"
#define BLYNK_AUTH_TOKEN "F8-DggpnG08Kbe0oFcCwkBkmfABhpX_n"

#include <Wire.h>
//#include <MAX30100_PublicDefine.h>
#include <MAX30100_Registers.h>
#include <MAX30100_PulseOximeter.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <OakOLED.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_SpO2Calculator.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <PulseOximeter.h>
#include "MAX30100_PulseOximeter.h"



//AQqgPM5mCyacwG3I2-w-36CgamVABgri
//F8-DggpnG08Kbe0oFcCwkBkmfABhpX_n

PulseOximeter pox;
OakOLED oled;
float BPM, SpO2;

uint32_t tsLastReport = 0;

char auth[] = "F8-DggpnG08Kbe0oFcCwkBkmfABhpX_n";             // Authentication Token Sent by Blynk

char ssid[] = "Varad's G82";        //WiFi SSID

char pass[] = "123456789";        //WiFi Password


const unsigned char bitmap[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x1F, 0x80, 0x0F, 0xE0, 0x1F, 0x80, 0x0F, 0xE0, 0x1F, 0x80,
  0x0F, 0xE0, 0x1F, 0x80, 0x1F, 0x80, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0,
  0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0,
  0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00 
};

void onBeatDetected()
{
  Serial.println("Beat Detected!");
  oled.drawBitmap(0, 0, bitmap, 16, 16, 1);
  oled.display();
}

#define REPORTING_PERIOD_MS 1000 // Define the reporting period in milliseconds

void setup()
{
  Serial.begin(115200);
  oled.begin();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(0, 0);
  oled.println("Initializing pulse oximeter..");
  oled.display();

  pinMode(16, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  if (!pox.begin()) {
  Serial.println("FAILED");
  while (1);
} else {
  Serial.println("Pulse Oximeter is initialized.");
}

}


void loop()
{
  pox.update();
  Blynk.run();

  float BPM = pox.getHeartRate();
  float SpO2 = pox.getSpO2();

  static uint32_t tsLastReport = 0; // Define tsLastReport as static

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
     Serial.print("Heart rate:");

    Serial.print(BPM);

    Serial.print(" SpO2:");

    Serial.print(SpO2);

    Serial.println(" %");

    Blynk.virtualWrite(V1, BPM);

    Blynk.virtualWrite(V2, SpO2);

    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setTextColor(1);

    oled.setCursor(0, 16);

    oled.println(pox.getHeartRate());



    oled.setTextSize(1);

    oled.setTextColor(1);

    oled.setCursor(0, 0);

    oled.println("Heart BPM");



    oled.setTextSize(1);

    oled.setTextColor(1);

    oled.setCursor(0, 30);

    oled.println("SpO2");



    oled.setTextSize(1);

    oled.setTextColor(1);

    oled.setCursor(0, 45);

    oled.println(pox.getSpO2());
    oled.display();

    tsLastReport = millis();
  }
}
