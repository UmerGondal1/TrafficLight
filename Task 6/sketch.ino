// Define LED pins for D1, D2, and D3 traffic lights
const int redCarLED1 = 5;    // For D1 Red
const int yellowCarLED1 = 4; // For D1 Yellow
const int greenCarLED1 = 3;  // For D1 Green

const int redCarLED2 = 12;   // For D2 Red
const int yellowCarLED2 = 11; // For D2 Yellow
const int greenCarLED2 = 10; // For D2 Green

const int redCarLED3 = 7;    // For D3 Red
const int yellowCarLED3 = 6; // For D3 Yellow
const int greenCarLED3 = 13; // For D3 Green

const int redPedestrianLED = 9;    // Pedestrian Red
const int greenPedestrianLED = 8;  // Pedestrian Green
const int buttonPin = 2;           // Pedestrian request button

// Timing variables
unsigned long previousMillis = 0;
unsigned long lastButtonPress = 0;
const long debounceDelay = 50;
const long carGreenTime = 5000;
const long carYellowTime = 2000;
const long carRedTime = 3000;
const long pedestrianGreenTime = 5000;

// Track current state
enum TrafficState {CAR_GREEN, CAR_YELLOW, CAR_RED, D3_YELLOW, PEDESTRIAN_GREEN};
TrafficState currentState = CAR_GREEN;

// Pedestrian request flag
bool pedestrianRequest = false;

void setup() {
  pinMode(redCarLED1, OUTPUT);
  pinMode(yellowCarLED1, OUTPUT);
  pinMode(greenCarLED1, OUTPUT);

  pinMode(redCarLED2, OUTPUT);
  pinMode(yellowCarLED2, OUTPUT);
  pinMode(greenCarLED2, OUTPUT);

  pinMode(redCarLED3, OUTPUT);
  pinMode(yellowCarLED3, OUTPUT);
  pinMode(greenCarLED3, OUTPUT);

  pinMode(redPedestrianLED, OUTPUT);
  pinMode(greenPedestrianLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize LEDs: D1 and D2 green, D3 red
  initializeLights();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check pedestrian button press with debounce
  if (digitalRead(buttonPin) == LOW && (currentMillis - lastButtonPress >= debounceDelay)) {
    lastButtonPress = currentMillis;
    pedestrianRequest = true; // Mark that pedestrian button is pressed
  }

  // Check for immediate pedestrian request
  if (pedestrianRequest && currentState != PEDESTRIAN_GREEN) {
    triggerPedestrianGreen(); // Immediately handle pedestrian request
    pedestrianRequest = false;
    return; // Skip the rest of the loop
  }

  // Main state machine
  switch (currentState) {
    case CAR_GREEN:
      if (currentMillis - previousMillis >= carGreenTime) {
        previousMillis = currentMillis;
        currentState = CAR_YELLOW;
        setCarYellow();
      }
      break;

    case CAR_YELLOW:
      if (currentMillis - previousMillis >= carYellowTime) {
        previousMillis = currentMillis;
        currentState = CAR_RED;
        setCarRed();
      }
      break;

    case CAR_RED:
      if (currentMillis - previousMillis >= carRedTime) {
        previousMillis = currentMillis;
        currentState = D3_YELLOW;
        setD3Yellow();
      }
      break;

    case D3_YELLOW:
      if (currentMillis - previousMillis >= carYellowTime) {
        previousMillis = currentMillis;
        currentState = CAR_GREEN;
        setCarGreen();
      }
      break;

    case PEDESTRIAN_GREEN:
      if (currentMillis - previousMillis >= pedestrianGreenTime) {
        previousMillis = currentMillis;
        currentState = CAR_GREEN;
        resetFromPedestrian();
      }
      break;
  }
}

// Immediately trigger pedestrian green state
void triggerPedestrianGreen() {
  // Stop all car traffic (turn all red)
  digitalWrite(redCarLED1, HIGH);
  digitalWrite(yellowCarLED1, LOW);
  digitalWrite(greenCarLED1, LOW);

  digitalWrite(redCarLED2, HIGH);
  digitalWrite(yellowCarLED2, LOW);
  digitalWrite(greenCarLED2, LOW);

  digitalWrite(redCarLED3, HIGH);
  digitalWrite(yellowCarLED3, LOW);
  digitalWrite(greenCarLED3, LOW);

  // Activate pedestrian green light
  digitalWrite(redPedestrianLED, LOW);
  digitalWrite(greenPedestrianLED, HIGH);

  currentState = PEDESTRIAN_GREEN;
  previousMillis = millis();
}

// Initialize lights to default state
void initializeLights() {
  digitalWrite(redCarLED1, LOW);
  digitalWrite(yellowCarLED1, LOW);
  digitalWrite(greenCarLED1, HIGH);

  digitalWrite(redCarLED2, LOW);
  digitalWrite(yellowCarLED2, LOW);
  digitalWrite(greenCarLED2, HIGH);

  digitalWrite(redCarLED3, HIGH);
  digitalWrite(yellowCarLED3, LOW);
  digitalWrite(greenCarLED3, LOW);

  digitalWrite(redPedestrianLED, HIGH);
  digitalWrite(greenPedestrianLED, LOW);
}

// Set D1/D2 yellow
void setCarYellow() {
  digitalWrite(greenCarLED1, LOW);
  digitalWrite(greenCarLED2, LOW);
  digitalWrite(yellowCarLED1, HIGH);
  digitalWrite(yellowCarLED2, HIGH);
}

// Set D1/D2 red, D3 green
void setCarRed() {
  digitalWrite(yellowCarLED1, LOW);
  digitalWrite(yellowCarLED2, LOW);
  digitalWrite(redCarLED1, HIGH);
  digitalWrite(redCarLED2, HIGH);

  digitalWrite(redCarLED3, LOW);
  digitalWrite(greenCarLED3, HIGH);
}

// Set D3 yellow
void setD3Yellow() {
  digitalWrite(greenCarLED3, LOW);
  digitalWrite(yellowCarLED3, HIGH);
}

// Reset from pedestrian state back to CAR_GREEN
void resetFromPedestrian() {
  digitalWrite(greenPedestrianLED, LOW);
  digitalWrite(redPedestrianLED, HIGH);
  initializeLights();
}

// Set D1/D2 green, D3 red
void setCarGreen() {
  digitalWrite(redCarLED1, LOW);
  digitalWrite(redCarLED2, LOW);
  digitalWrite(greenCarLED1, HIGH);
  digitalWrite(greenCarLED2, HIGH);

  digitalWrite(yellowCarLED3, LOW);
  digitalWrite(redCarLED3, HIGH);
}
