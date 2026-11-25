const int currentPin = A1;
const float shuntResistance = 1.0;
const int motorPin = 3;
const int temperaturePin = A0;
const int greenLedPin = 8;
const int redLedPin = 9;

// Button order: RESET, START, STOP
const int buttonPins[3] = {2, 4, 5};

int lastReading[3] = {HIGH, HIGH, HIGH};
int stableState[3] = {HIGH, HIGH, HIGH};
unsigned long lastChange[3] = {0, 0, 0};

const float tripTemperature = 50.0;
const float resetTemperature = 45.0;

bool temperatureFault = false;
bool motorRunning = false;
unsigned long lastDisplay = 0;

// Returns true once for each press, after 50 ms debounce
bool buttonPressed(int index) {
  int reading = digitalRead(buttonPins[index]);

  if (reading != lastReading[index]) {
    lastChange[index] = millis();
  }
  lastReading[index] = reading;

  if (millis() - lastChange[index] >= 50) {
    if (reading != stableState[index]) {
      stableState[index] = reading;
      return stableState[index] == LOW;
    }
  }

  return false;
}

void setup() {
  
  pinMode(motorPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  digitalWrite(motorPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);
  motorRunning = false;
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
  Serial.println("Ready. Press START.");
}
const float tripCurrentmA = 150.0;
bool currentFault = false;
unsigned long motorStartTime = 0;
const unsigned long startupDelayMs = 500;
void loop() {
  int reading = analogRead(temperaturePin);
  float voltage = reading * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;
  int currentReading = analogRead(currentPin);
  
  float shuntVoltage = currentReading * (5.0 / 1023.0);
  float motorCurrentA = shuntVoltage / shuntResistance;
  float motorCurrentmA = motorCurrentA * 1000.0;
  bool resetPressed = buttonPressed(0);
  bool startPressed = buttonPressed(1);
  buttonPressed(2);

  // STOP takes priority whenever it is held
  bool stopHeld = digitalRead(buttonPins[2]) == LOW;

  if (temperatureC >= tripTemperature) {
    temperatureFault = true;
    motorRunning = false;
      }

  // Keep the Serial R reset option
  while (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'R' || command == 'r') {
      resetPressed = true;
    }
  }

  if (resetPressed) {
    if (temperatureC <= resetTemperature) {
      temperatureFault = false;
      currentFault = false;
      motorRunning = false;
      Serial.println("Reset accepted. Press START.");
    } else {
      Serial.println("Reset blocked: cool to 45 C or below.");
    }
  }
  if (motorRunning &&
    millis() - motorStartTime >= startupDelayMs &&
    motorCurrentmA >= tripCurrentmA) {
  currentFault = true;
  motorRunning = false;
  Serial.println("FAULT: Overcurrent. Motor stopped.");
}
  if (stopHeld || temperatureFault || currentFault) {
  motorRunning = false;
} else if (startPressed && !resetPressed) {
  if (!motorRunning) {
    motorStartTime = millis();
  }
  motorRunning = true;
}

  digitalWrite(motorPin, motorRunning ? HIGH : LOW);
  digitalWrite(greenLedPin, motorRunning ? HIGH : LOW);
  digitalWrite(redLedPin,
             (temperatureFault || currentFault) ? HIGH : LOW);

  if (millis() - lastDisplay >= 500) {
    lastDisplay = millis();

    Serial.print("Temperature: ");
    Serial.print(temperatureC, 1);
    Serial.print(" C | ");
    
    Serial.print(" | Current: ");
    Serial.print(motorCurrentmA, 1);
    Serial.print(" mA");
    if (currentFault) {
      Serial.println("FAULT: Overcurrent. Check motor, then RESET.");
    } else if (temperatureFault) {
      Serial.println("FAULT: Motor OFF. Cool, then RESET.");
    } else if (motorRunning) {
      Serial.println("RUNNING: Motor commanded ON.");
    } else {
      Serial.println("STOPPED: Press START.");
    }
  }
}