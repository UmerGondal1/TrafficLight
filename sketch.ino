const int redPin = 5;
const int yellowPin = 4;
const int greenPin = 3;

enum TrafficState { RED, YELLOW, GREEN };
TrafficState currentState = RED;
bool afterGreen = false;  // Flag to track transition

void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    
    currentState = RED;
    digitalWrite(redPin, HIGH); // Start with Red light
}

void loop() {
    switch (currentState) {
        case RED:
            // Turn on the Red light
            digitalWrite(redPin, HIGH);
            delay(20000); // Stay Red for 20 seconds
            digitalWrite(redPin, LOW); // Turn off Red light
            currentState = YELLOW;
            afterGreen = false; // Reset flag
            break;

        case YELLOW:
            // Turn on the Yellow light
            digitalWrite(yellowPin, HIGH);
            delay(5000); // Stay Yellow for 5 seconds
            digitalWrite(yellowPin, LOW); // Turn off Yellow light
            
            // Transition based on the afterGreen flag
            if (afterGreen) {
                currentState = RED;  // Go back to Red after Green
            } else {
                currentState = GREEN; // Go to Green after Red
                afterGreen = true;    // Set flag for next Yellow transition
            }
            break;

        case GREEN:
            // Turn on the Green light
            digitalWrite(greenPin, HIGH);
            delay(20000); // Stay Green for 20 seconds
            digitalWrite(greenPin, LOW); // Turn off Green light
            currentState = YELLOW; // Transition to Yellow
            break;
    }
}

