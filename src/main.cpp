#include "Arduino.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// Condensation sensor
#define COND_DATA_PIN A3   // Data pin of the condensation sensor
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

// Temp level 1
#define TMP1_DATA_PIN 13

// Temp level 2
#define TMP2_DATA_PIN 8

// Temp level 3
#define TMP3_DATA_PIN 3

OneWire oneWire1(TMP1_DATA_PIN);
DallasTemperature sensorTemp1(&oneWire1);
OneWire oneWire2(TMP2_DATA_PIN);
DallasTemperature sensorTemp2(&oneWire2);
OneWire oneWire3(TMP3_DATA_PIN);
DallasTemperature sensorTemp3(&oneWire3);

bool forceFirstLoop = true;

unsigned long previousMillis = 0; // last time update
unsigned long interval = 15 * 60 * 1000L; // interval at which to do something (milliseconds)

// Turn on the condensation sensor and
float condensationTop() {
  analogWrite(COND_PWD_PIN, 255);
  delay(1000);
  float condensation = analogRead(COND_DATA_PIN);
  analogWrite(COND_PWD_PIN, 0);
  return condensation;
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
   return sensorTemp1.getTempCByIndex(0);
}

float temperatureLevel2() {
   return sensorTemp2.getTempCByIndex(0);
}

float temperatureLevel3() {
   return sensorTemp3.getTempCByIndex(0);
}

float humidityLevel1() {
  analogWrite(HUMD1_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD1_DATA_PIN);
  analogWrite(HUMD1_PWD_PIN, 0);
  return humidity;
}

float humidityLevel2() {
  analogWrite(HUMD2_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD2_DATA_PIN);
  analogWrite(HUMD2_PWD_PIN, 0);
  return humidity;
}

float humidityLevel3() {
  analogWrite(HUMD3_PWD_PIN, 255);
  delay(1000);
  float humidity = analogRead(HUMD3_DATA_PIN);
  analogWrite(HUMD3_PWD_PIN, 0);
  return humidity;
}

void printData(float condensation, float tempLevel1, float tempLevel2, float tempLevel3, float humdLevel1, float humdLevel2, float humdLevel3, boolean fanTopBoxEnabled, boolean fanLevel1Enabled, boolean fanLevel2Enabled, boolean fanLevel3Enabled, String errorMessage) {

    Serial.print("{");

    Serial.print("\"condensation_top\":");
    Serial.print(condensation);
    Serial.print(",");

    if (tempLevel1 != DEVICE_DISCONNECTED_C) {
      Serial.print("\"temp_level_1\":");
      Serial.print(tempLevel1);
      Serial.print(",");
    }

    if (tempLevel2 != DEVICE_DISCONNECTED_C) {
      Serial.print("\"temp_level_2\":");
      Serial.print(tempLevel2);
      Serial.print(",");
    }

    if (tempLevel3 != DEVICE_DISCONNECTED_C) {
      Serial.print("\"temp_level_3\":");
      Serial.print(tempLevel3);
      Serial.print(",");
    }

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

    if (!errorMessage.equals("")) {
      Serial.print("\"error_message\":");
      Serial.print(errorMessage);
      Serial.print(",");
    }

    Serial.println("}");

}

void setup() {
  Serial.begin(9600);
  pinMode(COND_PWD_PIN, OUTPUT);
  pinMode(HUMD1_PWD_PIN, OUTPUT);
  pinMode(HUMD2_PWD_PIN, OUTPUT);
  pinMode(HUMD3_PWD_PIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(FAN1_PWD_PIN, OUTPUT);
  pinMode(FAN2_PWD_PIN, OUTPUT);
  pinMode(FAN3_PWD_PIN, OUTPUT);
  sensorTemp1.begin();
  sensorTemp2.begin();
  sensorTemp3.begin();
}

void loop() {

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval || forceFirstLoop) {
     previousMillis = currentMillis;

    forceFirstLoop = false;

    // Check the condensation and turn on the fan if needed
    float condensation = condensationTop();

    // Check if all temperature sensor are connected
    String errorMessage = "";

    // Activate temperature sensor
    sensorTemp1.requestTemperatures();
    sensorTemp2.requestTemperatures();
    sensorTemp3.requestTemperatures();

    // Temperature level 1
    float tempLevel1 = temperatureLevel1();
    if (tempLevel1 == DEVICE_DISCONNECTED_C) {
      errorMessage += "Temperature sensor level 1 disconnected. ";
    }

    // Humidity level 1
    float humdLevel1 = humidityLevel1();

    // Temperature level 2
    float tempLevel2 = temperatureLevel2();
    if (tempLevel2 == DEVICE_DISCONNECTED_C) {
      errorMessage += "Temperature sensor level 2 disconnected. ";
    }

    // Humidity level 2
    float humdLevel2 = humidityLevel2();

    // Temperature level 3
    float tempLevel3 = temperatureLevel3();
    if (tempLevel3 == DEVICE_DISCONNECTED_C) {
      errorMessage += "Temperature sensor level 3 disconnected. ";
    }

    // Humidity level 3
    float humdLevel3 = humidityLevel3();

    // Active fan top box
    boolean fanTopBoxEnabled = condensation< 600 && tempLevel3>15.0;

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

    printData(condensation, tempLevel1, tempLevel2, tempLevel3, humdLevel1, humdLevel2, humdLevel3, fanTopBoxEnabled, fanLevel1Enabled, fanLevel2Enabled, fanLevel3Enabled, errorMessage);

  }

}
