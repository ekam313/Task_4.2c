#include <Wire.h>
#include <BH1750.h>

// Pin Definitions
#define SWITCH_PIN 2           // Slider switch (middle pin)
#define LED1_PIN 4             // Toggled by slider switch
#define LED2_PIN 5             // Toggled by light sensor interrupt
#define LIGHT_OUT_PIN 8        // Output pin to simulate light interrupt
#define LIGHT_INT_PIN 3        // Connected to LIGHT_OUT_PIN via resistor
#define LIGHT_THRESHOLD 1000   // Lux threshold

BH1750 lightMeter;
volatile bool led1State = false;
volatile bool led2State = false;

// ISR for slider switch
void handleSwitchToggle() {
  led1State = !led1State;
  digitalWrite(LED1_PIN, led1State);
  Serial.println("Slider Interrupt: LED1 Toggled");
}

// ISR triggered via light intensity condition (simulated interrupt)
void handleLightInterrupt() {
  led2State = !led2State;
  digitalWrite(LED2_PIN, led2State);
  Serial.println("Simulated Light Interrupt: LED2 Toggled");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin();

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LIGHT_OUT_PIN, OUTPUT);
  pinMode(LIGHT_INT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleSwitchToggle, CHANGE); //here i used CHANGE so when the position of switch change the led toggle(ON/OFF) according to it
  attachInterrupt(digitalPinToInterrupt(LIGHT_INT_PIN), handleLightInterrupt, RISING); // here RISING is used to make changes of LED2(ON/OFF) only when lux is above threshold

  Serial.println("System Ready. Monitoring...");
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light Intensity: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Simulate interrupt via digital output
  if (lux > LIGHT_THRESHOLD) {
    digitalWrite(LIGHT_OUT_PIN, HIGH); // Trigger interrupt
  } else {
    digitalWrite(LIGHT_OUT_PIN, LOW);  // Clear signal
  }

  delay(1000);
}
