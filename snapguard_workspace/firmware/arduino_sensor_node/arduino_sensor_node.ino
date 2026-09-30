// Pin Definitions
const int TEMP_PIN = A0;
const int VIB_PIN = A1;
const int RELAY_PIN = 8;
const int BUZZER_PIN = 9;

unsigned long last_send = 0;
bool is_estopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Normal operation: Relay ON (Machine running), Buzzer OFF
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // 1. Check for incoming commands from Snapdragon PC
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "ESTOP") {
      is_estopped = true;
      digitalWrite(RELAY_PIN, LOW); // Cut machine power
      digitalWrite(BUZZER_PIN, HIGH); // Sound alarm
    } else if (command == "RESET") {
      is_estopped = false;
      digitalWrite(RELAY_PIN, HIGH); // Restore power
      digitalWrite(BUZZER_PIN, LOW); // Silence alarm
    }
  }

  // 2. Read Sensors and transmit JSON telemetry to PC at 10Hz
  if (millis() - last_send > 100) {
    int temp_raw = analogRead(TEMP_PIN);
    float voltage = temp_raw * (5.0 / 1023.0);
    float temp_c = voltage * 100.0; // Assuming LM35 sensor
    
    int vib_raw = analogRead(VIB_PIN);
    
    // Construct JSON string
    Serial.print("{\"temp\":");
    Serial.print(temp_c);
    Serial.print(",\"vibration\":");
    Serial.print(vib_raw);
    Serial.print(",\"estop_active\":");
    Serial.print(is_estopped ? "true" : "false");
    Serial.println("}");
    
    last_send = millis();
  }
}
