#include <NewPing.h>
#include <DHT.h>
#include "WiFi.h"
 
#define TRIGGER_PIN 12
#define ECHO_PIN 14 
#define MAX_DISTANCE 200

#define DHTPIN 13
#define DHTTYPE DHT11

const char* ssid = "IoT";
const char* password = "1t3s0IoT18";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -5*3600;
const int   daylightOffset_sec = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
DHT dht1(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Test de sensores:");
  dht1.begin();

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("Conectado a internet, mi IP es:");
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop() 
{
  delay(5000); // lectura cada 5 segundos
  Serial.println("");
  Serial.println("********************************************");
  printLocalTime();
  leerdht1();
  Serial.println("********************************************");
  Serial.println("");
}

void leerdht1() 
{
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  while (isnan(t1) || isnan(h1)) 
  {
    Serial.println("Lectura fallida en el sensor DHT11, repitiendo lectura...");
    delay(3000); // esperar 3 seg
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
  }

  Serial.print(sonar.ping_cm());
  Serial.println("cm");

  float h = dht1.readHumidity();
  float t = dht1.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
  }


  float hic = dht1.computeHeatIndex(t,h,false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("% ,Temperature: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println("°C");
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
