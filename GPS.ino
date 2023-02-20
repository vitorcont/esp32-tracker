
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL_rAn9AyI"
#define BLYNK_TEMPLATE_NAME "GPS Template"
#define BLYNK_AUTH_TOKEN "1yw2ioo_VrjmcInu3GGsvvzKh8sUqzyx"
#define BLYNK_PRINT Serial

const char* ssid = "conti-2.4";
const char* password = "***";
static const int RXPin = 16, TXPin = 17;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);   
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  Blynk.run();    
  if (gpsSerial.available() && gps.encode(gpsSerial.read())){
    displayInfo();
  }
}

void collectDateTime() {
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid() && gps.time.isValid())
  {
    int day = gps.date.day();
    int month = gps.date.month();
    int year = gps.date.year();
    int hour = gps.time.hour();
    int minutes = gps.time.minute();

    Serial.print(month);
    Serial.print(F("/"));
    Serial.print(day);
    Serial.print(F("/"));
    Serial.print(year);

    Serial.print(F(" "));

    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(hour);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(minutes);
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());

    char datetime[30];

    sprintf(datetime, "%d/%d/%d %d:%d", month, day, year, hour, minutes);

    Blynk.virtualWrite(V8, datetime);


  }
  else
  {
    Serial.print(F("INVALID"));
  }
}

void collectLocation() {
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    double lat = gps.location.lat();
    double lng = gps.location.lng();
    double altitude = gps.altitude.meters();
    double speed = gps.speed.kmph();
    double bearing = gps.course.deg();
    int satelites = gps.satellites.value();

    char strLat[20];
    char strLng[20];
    char strMap[150];

    sprintf(strLat, "%.6f", lat);
    sprintf(strLng, "%.6f", lng);
    sprintf(strMap, "www.google.com/maps/search/%s,%s", strLat, strLng);

    Serial.print(lat, 6);
    Serial.print(F(","));
    Serial.print(lng,6);
    Serial.println();
    Serial.println(strMap);
    Serial.print("Satelites: ");
    Serial.println(satelites);
    Serial.println(speed);
    Serial.println();

    Blynk.virtualWrite(V0, satelites);

    Blynk.virtualWrite(V1, strLat);
    Blynk.virtualWrite(V2, strLng);
    Blynk.virtualWrite(V3, altitude);

    Blynk.virtualWrite(V4, "\n\n");
    Blynk.virtualWrite(V4, strLat);
    Blynk.virtualWrite(V4, strLng);
    Blynk.virtualWrite(V4, strMap);

    Blynk.virtualWrite(V6, strMap);
    Blynk.virtualWrite(V7, speed);

    Blynk.virtualWrite(V9, bearing);

    collectDateTime();
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  
}

void displayInfo() {
  //https://www.google.com/maps/search/-22.747571,-47.298808
  
  collectLocation();
  Serial.println();
}
