#include <AccelStepper.h>
#include <math.h> // Include math library for calculations

// Define the stepper motor connections
AccelStepper stepper(AccelStepper::FULL4WIRE, A4, A3, A2, A1);

const long STEPS_PER_REVOLUTION = 200; // Steps per revolution of the stepper motor
const int MICROSTEPS_PER_STEP = 16; // Number of microsteps per step (if applicable)

// Syringe specifics and volume displacement calculations
const float syringeDiameter = 20.0; // Diameter of the syringe in mm
const float pi = 3.14159;
const float area = pi * (syringeDiameter / 2) * (syringeDiameter / 2); // Cross-sectional area of the syringe
const float volumeToDisplace = 3000.0; // Volume to displace in mm^3 (3 ml)
const float plungerDisplacement = volumeToDisplace / area; // Displacement of the plunger in mm

// Calculate the steps needed for desired plunger displacement
const float mmPerStep = 1.0 / (STEPS_PER_REVOLUTION * MICROSTEPS_PER_STEP); // Assuming direct drive - adjust if using gearing
const float stepsForDisplacement = plungerDisplacement / mmPerStep;

// Motor speed and waveform parameters
float simFrequency = 0.25; // Slower frequency for visualization and realistic simulation
const int maxSteps = int(stepsForDisplacement); // Adjust maxSteps based on calculated steps for displacement

unsigned long previousTime = 0; // Stores the last time the motor was updated
const float updateInterval = 20; // Time in milliseconds to update the motor position

void setup() {
  stepper.setMaxSpeed(500); // Set a realistic max speed
  Serial.begin(9600); // Initialize serial for visualization

  // Optionally read the frequency from serial to adjust simFrequency
  Serial.println("Enter frequency value (Hz):");
}

void loop() {
  unsigned long currentTime = millis();

  // Check if there is any incoming serial data
  if (Serial.available() > 0) {
    simFrequency = Serial.parseFloat(); // Read frequency from serial
    Serial.print("Frequency set to: ");
    Serial.println(simFrequency, 3); // Print the new frequency value
  }

  if (currentTime - previousTime >= updateInterval) {
    float elapsedTime = float(currentTime) / 1000.0; // Time in seconds
    float theta = 2 * PI * simFrequency * elapsedTime; // Angle for the sinusoidal function
    int position = maxSteps * sin(theta); // Calculate the sinusoidal position

    stepper.moveTo(position);
    stepper.run();
    Serial.println(position); // Output position for visualization

    previousTime = currentTime;
  }
}
