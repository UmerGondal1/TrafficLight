// Define LED pins
const int redCarLED = 5;
const int yellowCarLED = 4;
const int greenCarLED = 3;
const int redPedestrianLED = 9;
const int greenPedestrianLED = 8;
const int buttonPin = 2;

// Define timing variables
unsigned long previousMillis = 0;
unsigned long lastButtonPress = 0;
const long debounceDelay = 50;
const long carGreenTime = 5000;
const long carYellowTime = 2000;
const long pedestrianGreenTime = 5000;
const long carRedTime = 3000; // Fixed duration for CAR_RED before transitioning

// Track current state
enum TrafficState {CAR_GREEN, CAR_YELLOW, CAR_RED, PEDESTRIAN_GREEN};
TrafficState currentState = CAR_GREEN;

// Track pedestrian request
bool pedestrianRequest = false;

void setup() {
  pinMode(redCarLED, OUTPUT);
  pinMode(yellowCarLED, OUTPUT);
  pinMode(greenCarLED, OUTPUT);
  pinMode(redPedestrianLED, OUTPUT);
  pinMode(greenPedestrianLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize LEDs
  digitalWrite(redCarLED, LOW);
  digitalWrite(yellowCarLED, LOW);
  digitalWrite(greenCarLED, HIGH); // Cars start green
  digitalWrite(redPedestrianLED, HIGH); // Pedestrians start red
  digitalWrite(greenPedestrianLED, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check button press with debounce
  if (digitalRead(buttonPin) == LOW && (currentMillis - lastButtonPress >= debounceDelay)) {
    lastButtonPress = currentMillis;
    pedestrianRequest = true;
  }

  switch (currentState) {
    case CAR_GREEN:
      if (currentMillis - previousMillis >= carGreenTime || pedestrianRequest) {
        previousMillis = currentMillis;
        currentState = CAR_YELLOW;
        digitalWrite(greenCarLED, LOW);
        digitalWrite(yellowCarLED, HIGH);
      }
      break;

    case CAR_YELLOW:
      if (currentMillis - previousMillis >= carYellowTime) {
        previousMillis = currentMillis;
        currentState = CAR_RED;
        digitalWrite(yellowCarLED, LOW);
        digitalWrite(redCarLED, HIGH);
      }
      break;

    case CAR_RED:
      if (pedestrianRequest) {
        // Transition to pedestrian green if request exists
        previousMillis = currentMillis;
        pedestrianRequest = false;
        currentState = PEDESTRIAN_GREEN;
        digitalWrite(greenPedestrianLED, HIGH);
        digitalWrite(redPedestrianLED, LOW);
      } else if (currentMillis - previousMillis >= carRedTime) {
        // No pedestrian request, transition back to car green
        previousMillis = currentMillis;
        currentState = CAR_GREEN;
        digitalWrite(redCarLED, LOW);
        digitalWrite(greenCarLED, HIGH);
      }
      break;

    case PEDESTRIAN_GREEN:
      if (currentMillis - previousMillis >= pedestrianGreenTime) {
        previousMillis = currentMillis;
        currentState = CAR_GREEN;
        digitalWrite(greenPedestrianLED, LOW);
        digitalWrite(redPedestrianLED, HIGH);
        digitalWrite(redCarLED, LOW);
        digitalWrite(greenCarLED, HIGH);
      }
      break;
  }
}