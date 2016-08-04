
#include "DHT.h"

// Vermicomposter Params
#define MAX_HUMIDITY 85.0

// DHT11 Params
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

// Fan params
#define FANPIN 11

#define ERROR_HUMIDITY 847593.0

unsigned long previousMillis = 0; // last time update
long interval = 15 * 60 * 1000; // interval at which to do something (milliseconds)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FANPIN, OUTPUT);
}

void loop() {
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval || currentMillis == 0) {
     previousMillis = currentMillis;  

    float humidity = getHumidity();
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    activateFan(humidity > MAX_HUMIDITY && humidity != ERROR_HUMIDITY);
    
  }
  
}

float getHumidity() {

  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return ERROR_HUMIDITY;
  }

//  Serial.print("Humidity: ");
//  Serial.print(humidity);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.println(" *C ");

  return humidity;
  
}

void activateFan(boolean activate) {
  if (activate) {
    analogWrite(FANPIN, 255);
  } else {
    analogWrite(FANPIN, 0);
  }
}

