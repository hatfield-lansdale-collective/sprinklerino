
// Pin constants:
#define MOISTURE_SENSOR A0
#define ENABLE_KNOB A1
#define DISABLE_KNOB A2
#define RELAY D7

// Range of the analog sensor's output.
#define DRY 580
#define WET 235

// Dead zone to leave to account for edge problems on the knob.
#define KNOB_DEAD 25

// aliases for pump states, to make the code more readable.
#define PUMP_ON LOW
#define PUMP_OFF HIGH

#define SAMPLE_FREQ 2 // Hz; number of times to check per second.

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, PUMP_OFF);
}

int readMoisture() {
  return map(analogRead(MOISTURE_SENSOR), WET, DRY, 100, 0);
}

int readKnob(uint8_t knob) {
  return map(analogRead(knob), KNOB_DEAD, 1023 - KNOB_DEAD, 0, 100);
}

void report(int moisture, int min_moist, int max_moist, uint8_t pump_state) {
  Serial.print("Moisture: "); Serial.print(moisture); Serial.println("%");
  Serial.print("Pump turns on below: "); Serial.print(min_moist); Serial.println("%");
  Serial.print("Pump turns off above: "); Serial.print(max_moise); Serial.println("%");
  Serial.print("Pump is currently: "); Serial.println(pump_state == PUMP_ON ? "ON" : "OFF");
  Serial.print("-----");
}

void loop() {
  // Read in our values
  int moisture = readMoisture();
  int knob1 = readKnob(ENABLE_KNOB);
  int knob2 = readKnob(DISABLE_KNOB);
  int min_moist = min(knob1, knob2);
  int max_moist = max(knob1, knob2);

  // Update the pump
  uint8_t pump_state;
  if (moisture <= min_moist) {
    pump_state = PUMP_ON;
  } else if (moisture >= max_moist) {
    pump_state = PUMP_OFF;
  }
  digitalWrite(RELAY, pump_state);

  // Report status to serial console for debugging.
  report(moisture, min_moist, max_moist, pump_state);

  delay(1000 / SAMPLE_FREQ);
}
