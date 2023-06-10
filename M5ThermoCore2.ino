#include <M5Core2.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

SHT3X sht30;
Adafruit_BMP280 bme;

void setup() {
  M5.begin();
  Wire.begin();

  M5.Lcd.setBrightness(10);

  while (!bme.begin(0x76)){
    Serial.println("Could not find a 0x76");
    M5.Lcd.println("Could not find a 0x76");
  }

  M5.Lcd.clear(BLACK);
  M5.Lcd.println("ENV Unit test...");
}

void loop() {
  float pressure = bme.readPressure();
  if(sht30.get()==0){
    float tmp = sht30.cTemp;
    float hum = sht30.humidity;
    
    Serial.printf("Temperatura: %2.2f*C",tmp);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("Temp: %2.1f",tmp);
  }


  delay(100);
}