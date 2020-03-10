#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "D-Link_DIR-600M"; //Your Network SSID
const char* password = "alliswell"; //Your Network Password


SoftwareSerial GPS_SoftSerial(4, 3);

WiFiClient client;
TinyGPSPlus gps;

unsigned long myChannelNumber = 1005865; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "GQI4SYYK4J2M3VMI"; //Your Write API Key



volatile float minutes, seconds;
volatile int degree, secs, mins;

void setup() {
  Serial.begin(9600);
  delay(10);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client); 
  GPS_SoftSerial.begin(9600);
   
}

void loop() {
  smartDelay(1000);
  unsigned long start;
  double lat_val, lng_val, alt_m_val;
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat(); 
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();
  alt_m_val = gps.altitude.meters(); 
  alt_valid = gps.altitude.isValid(); 

  if (!loc_valid)
  {
    Serial.print("Latitude : ");
    Serial.println("*****");
    Serial.print("Longitude : ");
    Serial.println("*****");
    delay(4000);
  }
  else
  {
    float latt = lat_val;
    float longg = lng_val;
    Serial.println("GPS READING: ");
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees : ");
    Serial.println(lat_val, 4);

    DegMinSec(lng_val); 
    Serial.print("Longitude in Decimal Degrees : ");
    Serial.println(lng_val, 4);
    ThingSpeak.setField(1, latt);
    ThingSpeak.setField(2, longg);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(4000);
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available()) 
    gps.encode(GPS_SoftSerial.read());
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
