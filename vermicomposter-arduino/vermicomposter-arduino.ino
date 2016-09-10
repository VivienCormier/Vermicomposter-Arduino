
#include <OneWire.h>
#include <DallasTemperature.h>

// Condensation sensor
#define COND_DATA_PIN 2   // Data pin of the condensation sensor
#define COND_PWD_PIN 10   // Power pin of the condenstion sensor

// Humidity box leve 1
#define HUMD1_DATA_PIN A0   // Data pin of the humidity sensor
#define HUMD1_PWD_PIN 7   // Power pin of the humidity sensor

// Top Fan params
#define FANPIN 11

// Fan box level 1
#define FAN1_PWD_PIN 12

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 8

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

bool forceFirstLoop = true;

unsigned long previousMillis = 0; // last time update
unsigned long interval = 15 * 60 * 1000L; // interval at which to do something (milliseconds)

void setup() {
  Serial.begin(9600);
  pinMode(COND_DATA_PIN, INPUT);
  pinMode(COND_PWD_PIN, OUTPUT);
  pinMode(HUMD1_PWD_PIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  sensors.begin();
}

void loop() {
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval || forceFirstLoop) {
     previousMillis = currentMillis;  

     forceFirstLoop = false;

    // Check the condensation and turn on the fan if needed
    bool condensationIsDetected = condensationDetected();

    // Activate temperature sensor
    sensors.requestTemperatures();

    // Temperature level 1
    float tempLevel1 = temperatureLevel1();

    // Humidity level 1
    float humdLevel1 = humidityLevel1();

    // Temperature level 2
    float tempLevel2 = temperatureLevel2();

    activateFanTopBox(condensationIsDetected || tempLevel2 > 26.0);
    activateFanLevel1(tempLevel1>26.0);

    Serial.print("Condensation detected : ");
    if (condensationIsDetected) {
      Serial.print("YES");
    } else {
      Serial.print("NO");
    }
    Serial.print(" | Temp level 1 : ");
    Serial.print(tempLevel1);
    Serial.print("°C");
    Serial.print(" | Humd level 1 : ");
    Serial.print(humdLevel1);
    Serial.print(" | Temp level 2 : ");
    Serial.println(tempLevel2);
  }
  
}

void activateFanTopBox(boolean activate) {
  if (activate) {
    analogWrite(FANPIN, 255);
  } else {
    analogWrite(FANPIN, 0);
  }
}

void activateFanLevel1(boolean activate) {
  if (activate) {
    analogWrite(FAN1_PWD_PIN, 255);
  } else {
    analogWrite(FAN1_PWD_PIN, 0);
  }
}

// Turn on the condensation sensor and 
bool condensationDetected() {
  analogWrite(COND_PWD_PIN, 255);
  delay(1000);
  bool detected = digitalRead(COND_DATA_PIN) == LOW;  // LOW = Condensation
  analogWrite(COND_PWD_PIN, 0);
  return detected;
}

float temperatureLevel1() {
   return sensors.getTempCByIndex(1);
}

float temperatureLevel2() {
   return sensors.getTempCByIndex(0);
}

// Turn on the condensation sensor and 
float humidityLevel1() {
  analogWrite(HUMD1_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD1_DATA_PIN);
//  humidity = 100 * humidity / 18;
//  delay(2000);
  analogWrite(HUMD1_PWD_PIN, 0);
  return humidity;
}

