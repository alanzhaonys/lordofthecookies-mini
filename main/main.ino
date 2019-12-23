//
// Lord Of The Cookies Mini Arduino Sketch
//
// Author: Alan Zhao
// Date: 11/10/2019
//

#define pressed HIGH
#define unpressed LOW

//
// Start pin definitions
//

// Actuator pins
const int actuatorEnablePin = 9;
const int actuatorIn1Pin = 8;
const int actuatorIn2Pin = 7;

// Dispense pin
const int dispensePin = A1;

// Actuator limit switches pins
const int actuatorForwardPin = A2;
const int actuatorBackwardPin = A3;

// Buzzer pin
const int buzzer = A4;

// LED trigger pin
const int ledTriggerPin = A5;

// Other configurations
const int actuatorMotorSpeed = 255;
const int maxMovingSecs = 5;

//
// Start functions
//

// Read actuator forward sensor value 3 times for realible reading, due to interfernces from motor
bool actuatorBackwardPinPressed() {
  if (digitalRead(actuatorBackwardPin) == pressed) {
    if (digitalRead(actuatorBackwardPin) == pressed) {
      if (digitalRead(actuatorBackwardPin) == pressed) {
        return true;
      }
    }
  }
  return false;
}

// Read actuator aackward sensor value 3 times for realible reading, due to interfernces from motor
bool actuatorForwardPinPressed() {
  if (digitalRead(actuatorForwardPin) == pressed) {
    if (digitalRead(actuatorForwardPin) == pressed) {
      if (digitalRead(actuatorForwardPin) == pressed) {
        return true;
      }
    }
  }
  return false;
}


// Read dispense sensor value 2 times for realible reading
bool dispensePinPressed() {
  if (digitalRead(dispensePin) == pressed) {
    if (digitalRead(dispensePin) == pressed) {
      beep(1);
      return true;
    }
  }
  return false;
}

// Stop actuator motor
void stopActuator() {
  analogWrite(actuatorEnablePin, 0);
  digitalWrite(actuatorIn1Pin, LOW);
  digitalWrite(actuatorIn2Pin, LOW);
}

// Move actuator motor backward
void actuatorBackward() {
  analogWrite(actuatorEnablePin, actuatorMotorSpeed);
  digitalWrite(actuatorIn1Pin, HIGH);
  digitalWrite(actuatorIn2Pin, LOW);
}

// Move actuator motor forward until the end or max moving secs is reached
void actuatorForward() {
  analogWrite(actuatorEnablePin, actuatorMotorSpeed);
  digitalWrite(actuatorIn1Pin, LOW);
  digitalWrite(actuatorIn2Pin, HIGH);
}

// Move actuator forward until end is reached
void moveActuatorForward() {
  unsigned long startTime = millis();
  bool running = false;

  while (!actuatorForwardPinPressed()) {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    if (elapsedTime >= maxMovingSecs * 1000) {
      break;
    }

    if (!running) {
      actuatorForward();
      running = true;
    }
  }
  stopActuator();
}

// Move actuator motor backward until the end or max moving secs is reached
void moveActuatorBackward() {
  unsigned long startTime = millis();
  bool running = false;

  while (!actuatorBackwardPinPressed()) {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    if (elapsedTime >= maxMovingSecs * 1000) {
      break;
    }

    if (!running) {
      actuatorBackward();
      running = true;
    }
  }
  stopActuator();
}

// Move actuator forward for specified seconds
void moveActuatorForwardFor(float seconds) {
  unsigned long startTime = millis();
  bool running = false;

  while (!actuatorForwardPinPressed()) {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    if (elapsedTime >= seconds * 1000) {
      break;
    }

    if (!running) {
      actuatorForward();
      running = true;
    }
  }
  stopActuator();
}

// Move actuator backward for specified seconds
void moveActuatorBackwardFor(float seconds) {
  unsigned long startTime = millis();
  bool running = false;

  while (!actuatorBackwardPinPressed()) {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - startTime;
    if (elapsedTime >= seconds * 1000) {
      break;
    }

    if (!running) {
      actuatorBackward();
      running = true;
    }
  }
  stopActuator();
}

// Make beep sound for specified counts
void beep(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
}

void setup() {

  // Start serial connection
  Serial.begin(9600);

  // Initialize actuator pins
  pinMode(actuatorEnablePin, OUTPUT);
  pinMode(actuatorIn1Pin, OUTPUT);
  pinMode(actuatorIn2Pin, OUTPUT);

  // Initialize actuator limit switch pins
  // Not connected = HIGH
  // NO pin = HIGH (when not pressed), LOW (when pressed)
  // NC pin = LOW (when not pressed), HIGH (when pressed)
  pinMode(actuatorForwardPin, INPUT);
  pinMode(actuatorBackwardPin, INPUT);

  // Initialize LED trigger pin
  pinMode(ledTriggerPin, OUTPUT);

  // Stop LED
  //digitalWrite(ledTriggerPin, LOW);

  // Initialize buzzer pin
  pinMode(buzzer, OUTPUT);

  moveActuatorBackward();

  beep(2);
  //Serial.write("Starting...");
}

void loop() {

  //Serial.println((String)"Forward pin: "+digitalRead(actuatorForwardPin));
  //Serial.println((String)"Backward pin: "+digitalRead(actuatorBackwardPin));

  // Test
  //moveActuatorForward();
  //moveActuatorBackward();

  while (dispensePinPressed() == pressed) {
    // Flash LED
    digitalWrite(ledTriggerPin, HIGH);

    // Move actuator forward
    moveActuatorForward();
    delay(2000);

    // Move actuator backward
    moveActuatorBackward();
    
    // Stop LED
    digitalWrite(ledTriggerPin, LOW);
  }
}
