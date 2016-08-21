

// Condensation sensor
#define COND_DATA_PIN 2   // Data pin of the condensation sensor
#define COND_PWD_PIN 10   // Power pin of the condenstion sensor

// Fan params
#define FANPIN 11

unsigned long previousMillis = 0; // last time update
long interval = 15 * 60 * 1000; // interval at which to do something (milliseconds)

void setup() {
  Serial.begin(9600);
  pinMode(COND_DATA_PIN, INPUT);
  pinMode(COND_PWD_PIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
}

void loop() {
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval || currentMillis == 0) {
     previousMillis = currentMillis;  

    // Check the condensation and turn on the fan if needed
    activateFan(condensationDetected());
    
  }
  
}

void activateFan(boolean activate) {
  if (activate) {
    analogWrite(FANPIN, 255);
  } else {
    analogWrite(FANPIN, 0);
  }
}

bool condensationDetected() {
  analogWrite(COND_PWD_PIN, 255);
  delay(1000);
  bool detected = digitalRead(COND_DATA_PIN) == LOW;  // LOW = Condensation
  analogWrite(COND_PWD_PIN, 0);
  return detected;
}

