#include <Arduino.h>

const int TEMP_PIN = A0;
const int VIB_PIN = A1;
const int RELAY_PIN = 8;
const int BUZZER_PIN = 9;
const float BETA = 3950; 

unsigned long last_send = 0;
bool is_estopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "ESTOP") {
      is_estopped = true;
      digitalWrite(RELAY_PIN, LOW); 
      digitalWrite(BUZZER_PIN, HIGH); 
    } else if (command == "RESET") {
      is_estopped = false;
      digitalWrite(RELAY_PIN, HIGH); 
      digitalWrite(BUZZER_PIN, LOW); 
    }
  }

  if (millis() - last_send > 100) {
    int analogValue = analogRead(TEMP_PIN);
    float temp_c = 1.0 / (log(1.0 / (1023.0 / analogValue - 1.0)) / BETA + 1.0 / 298.15) - 273.15;
    int vib_raw = analogRead(VIB_PIN);
    int vib_percent = map(vib_raw, 0, 1023, 0, 100);
    
    Serial.print("{\"temp\":"); Serial.print(temp_c);
    Serial.print(",\"vibration\":"); Serial.print(vib_percent);
    Serial.print(",\"estop_active\":"); Serial.print(is_estopped ? "true" : "false");
    Serial.println("}");
    
    last_send = millis();
  }
}
