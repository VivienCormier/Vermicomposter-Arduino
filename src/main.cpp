#include "Arduino.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// Condensation sensor
#define COND_DATA_PIN 2   // Data pin of the condensation sensor
#define COND_PWD_PIN 10   // Power pin of the condenstion sensor

// Humidity box leve 1
#define HUMD1_DATA_PIN A0   // Data pin of the humidity sensor
#define HUMD1_PWD_PIN 7   // Power pin of the humidity sensor

// Humidity box leve 2
#define HUMD2_DATA_PIN A1   // Data pin of the humidity sensor
#define HUMD2_PWD_PIN 6   // Power pin of the humidity sensor

// Humidity box leve 3
#define HUMD3_DATA_PIN A2   // Data pin of the humidity sensor
#define HUMD3_PWD_PIN 4   // Power pin of the humidity sensor

// Top Fan params
#define FANPIN 11

// Fan box level 1
#define FAN1_PWD_PIN 12

// Fan box level 2
#define FAN2_PWD_PIN 9

// Fan box level 3
#define FAN3_PWD_PIN 5

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 8

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

bool forceFirstLoop = true;

unsigned long previousMillis = 0; // last time update
unsigned long interval = 15 * 60 * 1000L; // interval at which to do something (milliseconds)

// Turn on the condensation sensor and
bool condensationDetected() {
  analogWrite(COND_PWD_PIN, 255);
  delay(1000);
  bool detected = digitalRead(COND_DATA_PIN) == LOW;  // LOW = Condensation
  analogWrite(COND_PWD_PIN, 0);
  return detected;
}

void activateFanTopBox(boolean activate) {
  if (activate) {
    analogWrite(FANPIN, 255);
  } else {
    analogWrite(FANPIN, 0);
  }
}

void activateFanLevel3(boolean activate) {
  if (activate) {
    analogWrite(FAN3_PWD_PIN, 255);
  } else {
    analogWrite(FAN3_PWD_PIN, 0);
  }
}

void activateFanLevel2(boolean activate) {
  if (activate) {
    analogWrite(FAN2_PWD_PIN, 255);
  } else {
    analogWrite(FAN2_PWD_PIN, 0);
  }
}

void activateFanLevel1(boolean activate) {
  if (activate) {
    analogWrite(FAN1_PWD_PIN, 255);
  } else {
    analogWrite(FAN1_PWD_PIN, 0);
  }
}

float temperatureLevel1() {
   return sensors.getTempCByIndex(1);
}

float temperatureLevel2() {
   return sensors.getTempCByIndex(0);
}

float temperatureLevel3() {
   return sensors.getTempCByIndex(2);
}

// Turn on the condensation sensor and
float humidityLevel1() {
  analogWrite(HUMD1_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD1_DATA_PIN);
  analogWrite(HUMD1_PWD_PIN, 0);
  return humidity;
}

// Turn on the condensation sensor and
float humidityLevel2() {
  analogWrite(HUMD2_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD2_DATA_PIN);
  analogWrite(HUMD2_PWD_PIN, 0);
  return humidity;
}

// Turn on the condensation sensor and
float humidityLevel3() {
  analogWrite(HUMD3_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD3_DATA_PIN);
  analogWrite(HUMD3_PWD_PIN, 0);
  return humidity;
}

void printData(boolean condensationIsDetected, float tempLevel1, float tempLevel2, float tempLevel3, float humdLevel1, float humdLevel2, float humdLevel3, boolean fanTopBoxEnabled, boolean fanLevel1Enabled, boolean fanLevel2Enabled, boolean fanLevel3Enabled) {

    Serial.print("{");

    Serial.print("\"condensation\":");
    if (condensationIsDetected) {
      Serial.print("true");
    } else {
      Serial.print("false");
    }
    Serial.print(",");

    Serial.print("\"temp_level_1\":");
    Serial.print(tempLevel1);
    Serial.print(",");

    Serial.print("\"temp_level_2\":");
    Serial.print(tempLevel2);
    Serial.print(",");

    Serial.print("\"temp_level_3\":");
    Serial.print(tempLevel3);
    Serial.print(",");

    Serial.print("\"humd_level_1\":");
    Serial.print(humdLevel1);
    Serial.print(",");

    Serial.print("\"humd_level_2\":");
    Serial.print(humdLevel2);
    Serial.print(",");

    Serial.print("\"humd_level_3\":");
    Serial.print(humdLevel3);
    Serial.print(",");

    Serial.print("\"fan_top_box_enabled\":");
    if (fanTopBoxEnabled) {
      Serial.print("true");
    } else {
      Serial.print("false");
    }
    Serial.print(",");

    Serial.print("\"fan_level_1_enabled\":");
    if (fanLevel1Enabled) {
      Serial.print("true");
    } else {
      Serial.print("false");
    }
    Serial.print(",");

    Serial.print("\"fan_level_2_enabled\":");
    if (fanLevel2Enabled) {
      Serial.print("true");
    } else {
      Serial.print("false");
    }
    Serial.print(",");

    Serial.print("\"fan_level_3_enabled\":");
    if (fanLevel3Enabled) {
      Serial.print("true");
    } else {
      Serial.print("false");
    }

    Serial.println("}");

}

void setup() {
  Serial.begin(9600);
  pinMode(COND_DATA_PIN, INPUT);
  pinMode(COND_PWD_PIN, OUTPUT);
  pinMode(HUMD1_PWD_PIN, OUTPUT);
  pinMode(HUMD2_PWD_PIN, OUTPUT);
  pinMode(HUMD3_PWD_PIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(FAN1_PWD_PIN, OUTPUT);
  pinMode(FAN2_PWD_PIN, OUTPUT);
  pinMode(FAN3_PWD_PIN, OUTPUT);
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

    // Humidity level 2
    float humdLevel2 = humidityLevel2();

    // Temperature level 3
    float tempLevel3 = temperatureLevel3();

    // Humidity level 3
    float humdLevel3 = humidityLevel3();

    // Active fan top box
    boolean fanTopBoxEnabled = condensationIsDetected && tempLevel3>15.0;

    // Active fan Level 3
    boolean fanLevel3Enabled = (fanTopBoxEnabled || tempLevel3>26.0  || humdLevel3 < 37.0) && tempLevel3>15.0;

    // Active fan Level 2
    boolean fanLevel2Enabled = (tempLevel2>26.0 || humdLevel2 < 37.0) && tempLevel2>15.0;

    // Active fan Level 1
    boolean fanLevel1Enabled =  (tempLevel1>26.0 || humdLevel1 < 37.0) && tempLevel1>15.0;


    activateFanTopBox(fanTopBoxEnabled);
    activateFanLevel3(fanLevel3Enabled);
    activateFanLevel2(fanLevel2Enabled);
    activateFanLevel1(fanLevel1Enabled);

    printData(condensationIsDetected, tempLevel1, tempLevel2, tempLevel3, humdLevel1, humdLevel2, humdLevel3, fanTopBoxEnabled, fanLevel1Enabled, fanLevel2Enabled, fanLevel3Enabled);

  }

}
