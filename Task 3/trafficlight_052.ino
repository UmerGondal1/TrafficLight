
#include <Wire.h>

// Variables for the pedestrian light
const int pedRedPin = 12;
const int pedGreenPin = 13;

void setup() {
    Wire.begin(8); // Join I2C bus with address #8
    Wire.onReceive(receiveEvent); // Register event handler
    pinMode(pedRedPin, OUTPUT);
    pinMode(pedGreenPin, OUTPUT);
    digitalWrite(pedRedPin, HIGH); // Start with the red light on
}

void loop() {
    
}

void receiveEvent(int howMany) {
    String command = "";
    while (Wire.available()) {
        char c = Wire.read();
        command += c;
    }

    if (command == "activate") {
        digitalWrite(pedRedPin, LOW);
        digitalWrite(pedGreenPin, HIGH);
    } else if (command == "deactivate") {
        digitalWrite(pedGreenPin, LOW);
        digitalWrite(pedRedPin, HIGH);
    }
}
