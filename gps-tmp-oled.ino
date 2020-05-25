# AtMega328pu,GPS,temperture on an oled display
# Bernd
# may 2020 
# 
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


static const int RXPin = 4, TXPin = 3;    //input from Neo-6M
static const uint32_t GPSBaud = 9600;
static char yearstring[15];
static float fYear;
int volt;
int tempPin = 1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;
//LiquidCrystal_I2C lcd(0x27, 16, 2);
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup()
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  ss.begin( 9600 );
  milli_delay(1000);
}

void loop()
{ 
  u8x8.setCursor(0,0);
  if (gps.satellites.isUpdated()) {
    u8x8.print("Satelieten: ");
    u8x8.print(gps.satellites.value());
    //u8x8.print(" ");
  } else {
    u8x8.println("Nosat :'(");
  }
  u8x8.setCursor(0, 1);
  if (gps.altitude.isUpdated()) {
    u8x8.print("Hoogte: ");
    u8x8.print(gps.altitude.meters(), 1);
    u8x8.print(" m.");
  } else {
    u8x8.print("AX");
  }
  u8x8.setCursor(0, 2);
  if (gps.location.isUpdated()) {
    u8x8.print("Long: ");
    u8x8.print(gps.location.lng());
    u8x8.setCursor(0, 3);
    u8x8.print("Lat:  ");
    u8x8.print(gps.location.lat());
  } else {
    u8x8.print("Waar ben ik?");
  }
  
    if (gps.date.isValid()& gps.time.isValid()) {
    u8x8.setCursor(0, 4);
    u8x8.print(gps.date.day());
    u8x8.print("-");
    u8x8.print(gps.date.month());
    u8x8.print("-");
    fYear = gps.date.year();
    dtostrf(fYear, 4, 0, yearstring);
    u8x8.print(yearstring[2]);
    u8x8.print(yearstring[3]);
    u8x8.print("  ");
    //u8x8.setCursor(0, 5);
    u8x8.print(gps.time.hour() + 1);
    u8x8.print(":");
    u8x8.print(gps.time.minute());
    u8x8.print(":");
    u8x8.print(gps.time.second());
  }
  
  volt = analogRead(tempPin);
  //float mv =  volt * (5000/1024);
  //float cel = mv / 10;
  
  float mv = (volt / 1024.0)*5000;
  float cel = mv / 10;
  
  u8x8.setCursor(0, 6);  
  u8x8.print("volt: ");
  u8x8.print(volt);
  u8x8.setCursor(0, 7);
  u8x8.print("Temp: ");
  u8x8.print(cel);
  u8x8.print(" C");
  
  delay(3000);

  milli_delay(1000);
}

static void milli_delay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
