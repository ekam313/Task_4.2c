#include <Wire.h>
#include <BH1750.h>

#define SWITCH_PIN 2     // ON/OFF switch
#define LIGHT_SENSOR_PIN A0 // Analog light sensor pin
#define LED1_PIN 4       // LED controlled by switch
#define LED2_PIN 5       // LED controlled by light sensor
#define LIGHT_THRESHOLD 100  // Adjust this threshold based on ambient light

BH1750 lightMeter;
volatile bool led1State = false; // LED1 toggle state
volatile bool led2State = false; // LED2 toggle state

// Interrupt Service Routine for ON/OFF switch
void handleSwitchToggle() {
    led1State = !led1State;
    digitalWrite(LED1_PIN, led1State);
    Serial.println("Switch Interrupt: LED1 Toggled");
}

// Interrupt check for light sensor
void checkLightSensor() {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light Intensity: ");
    Serial.print(lux);
    Serial.println(" lx");

    if (lux < LIGHT_THRESHOLD) {
        if (!led2State) {
            led2State = true;
            digitalWrite(LED2_PIN, HIGH);
            Serial.println("Light Sensor: LED2 ON (Dark)");
        }
    } else {
        if (led2State) {
            led2State = false;
            digitalWrite(LED2_PIN, LOW);
            Serial.println("Light Sensor: LED2 OFF (Bright)");
        }
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    lightMeter.begin();

    pinMode(SWITCH_PIN, INPUT_PULLUP);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    // Attach interrupt for switch
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleSwitchToggle, FALLING);
}

void loop() {
    checkLightSensor(); // Continuously check light sensor
    delay(5000);
}


