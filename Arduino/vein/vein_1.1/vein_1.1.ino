#include <AccelStepper.h>

// Define the stepper motor connections
AccelStepper stepper(AccelStepper::FULL4WIRE, A4, A3, A2, A1);

const long STEPS_PER_REVOLUTION = 200;  // Define based on your stepper motor's specifications
const float simFrequency = 0.25;  // Reduced frequency to increase period to 4 seconds
const int maxSteps = 50;  // Max steps to simulate the expansion and contraction

unsigned long previousTime = 0;  // Stores the last time the motor was updated
const float updateInterval = 20;  // Time in milliseconds to update the motor position

void setup() {
  stepper.setMaxSpeed(1000);  // Set max speed
  Serial.begin(9600);  // Initialize serial for visualization
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= updateInterval) {
    float elapsedTime = float(currentTime) / 1000.0;  // Time in seconds
    float cycleTime = fmod(elapsedTime, 1.0 / simFrequency);  // Adjusted cycle time to reflect the new frequency

    int position = 0;
    float phaseDuration = 1.0 / simFrequency / 4; // Adjust all phase timings according to the new frequency
    if (cycleTime < phaseDuration) {  // a-wave
      position = maxSteps * sin(PI * cycleTime / phaseDuration);
    } else if (cycleTime < 2 * phaseDuration) {  // x descent
      position = maxSteps * (1 - 2 * (cycleTime - phaseDuration) / phaseDuration);
    } else if (cycleTime < 3 * phaseDuration) {  // c-wave followed by v-wave
      position = maxSteps * sin(PI * (cycleTime - 2 * phaseDuration) / phaseDuration);
    } else {  // y descent
      position = maxSteps * (1 - 2 * (cycleTime - 3 * phaseDuration) / phaseDuration);
    }

    stepper.moveTo(position);
    stepper.run();
    Serial.println(position);  // Output position for visualization

    previousTime = currentTime;
  }
}
