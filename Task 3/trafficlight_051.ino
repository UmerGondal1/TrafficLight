#include <Wire.h>

// Variables for the traffic light
const int trafficRedPin = 9;
const int trafficYellowPin = 10;
const int trafficGreenPin = 11;

// Button pin for pedestrian crossing
const int buttonPin = 2;

// Timing durations
const unsigned long greenDuration = 5000;
const unsigned long yellowDuration = 5000;
const unsigned long redDuration = 20000;

// Debounce timing
const unsigned long debounceDelay = 50;

// TrafficLight class definition
class TrafficLight {
private:
    int redPin, yellowPin, greenPin;
    unsigned long previousMillis;
    bool pedestrianWaiting;
    bool wasGreenBefore;
    enum TrafficState { RED, YELLOW, GREEN };
    TrafficState currentState;

public:
    TrafficLight(int rPin, int yPin, int gPin)
        : redPin(rPin), yellowPin(yPin), greenPin(gPin),
          previousMillis(0), pedestrianWaiting(false),
          wasGreenBefore(true), currentState(GREEN) {
        pinMode(redPin, OUTPUT);
        pinMode(yellowPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        digitalWrite(greenPin, HIGH);
    }

    void setPedestrianWaiting(bool waiting) {
        pedestrianWaiting = waiting;
    }

    void update(unsigned long currentMillis) {
        switch (currentState) {
            case GREEN:
                handleGreenState(currentMillis);
                break;
            case YELLOW:
                handleYellowState(currentMillis);
                break;
            case RED:
                handleRedState(currentMillis);
                break;
        }
    }

private:
    void handleGreenState(unsigned long currentMillis) {
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        if (currentMillis - previousMillis >= greenDuration) {
            previousMillis = currentMillis;
            digitalWrite(greenPin, LOW);
            if (pedestrianWaiting) {
                currentState = YELLOW;
                wasGreenBefore = true;
            } else {
                previousMillis = millis();
                digitalWrite(greenPin, HIGH);
            }
        }
    }

    void handleYellowState(unsigned long currentMillis) {
        digitalWrite(yellowPin, HIGH);
        if (currentMillis - previousMillis >= yellowDuration) {
            previousMillis = currentMillis;
            digitalWrite(yellowPin, LOW);
            currentState = wasGreenBefore ? RED : GREEN;
        }
    }

    void handleRedState(unsigned long currentMillis) {
        digitalWrite(redPin, HIGH);
        Wire.beginTransmission(8); // Address of the slave
        Wire.write("activate"); // Command to activate the pedestrian light
        Wire.endTransmission();
        if (currentMillis - previousMillis >= redDuration) {
            previousMillis = currentMillis;
            digitalWrite(redPin, LOW);
            Wire.beginTransmission(8);
            Wire.write("deactivate"); // Command to deactivate the pedestrian light
            Wire.endTransmission();
            pedestrianWaiting = false;
            currentState = YELLOW;
            wasGreenBefore = false;
        }
    }
};

// Main function and global variables
TrafficLight trafficTrafficLight(trafficRedPin, trafficYellowPin, trafficGreenPin);
unsigned long lastDebounceTime = 0;

void setup() {
    Wire.begin(); // Start I2C communication
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    unsigned long currentMillis = millis();
    trafficTrafficLight.update(currentMillis);

    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && (currentMillis - lastDebounceTime) > debounceDelay) {
        trafficTrafficLight.setPedestrianWaiting(true);
        lastDebounceTime = currentMillis;
    }
}
