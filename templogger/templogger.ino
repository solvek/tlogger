#define USE_DHT

#include <RFduinoBLE.h>

#ifdef USE_DHT
#include "DHT.h"
#define DHTPIN 6
DHT dht(DHTPIN, DHT22);
#endif

#define INTERVAL 2000

void setup() {
  Serial.begin(9600);
  #ifdef USE_DHT
  dht.begin();
  delay(10000);
  dht.readHumidity();
  #endif

  RFduinoBLE.deviceName = "Temperature Logger";
  RFduinoBLE.begin();
}

float humidity, temperature;

void loop() {
  delay(INTERVAL);

  while(!RFduinoBLE.radioActive);
  while(RFduinoBLE.radioActive);

  Serial.println("Reading data");
  humidity = readHumidity();
  Serial.println("Humidity read");
  temperature = readTemperature();
  Serial.println("Temperature read");
  
  Serial.print("Humidity: "); 
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(temperature);
  Serial.println(" *C ");

  Serial.println("Sending data");
  RFduinoBLE.sendFloat(temperature);
  Serial.println("Data sent");
}

float readTemperature(){
  #ifdef USE_DHT
  float t= dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }
  return t;
  #else
  return RFduino_temperature(CELSIUS);
  #endif
}

float readHumidity(){
  #ifdef USE_DHT
  float h = dht.readHumidity();
  if (isnan(h)) return 0;
  return h;
  #else
  return 0;
  #endif  
}

