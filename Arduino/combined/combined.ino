#include <AccelStepper.h>

// Stepper motor 1 setup
AccelStepper stepper1(AccelStepper::FULL4WIRE, A3, A2, A1, A0);
unsigned long previousMillis1 = 0;
long interval1 = 2000;  // Each state lasts for 1 second

// Stepper motor 2 setup
AccelStepper stepper2(AccelStepper::FULL4WIRE, 7, 6, 5, 4);
unsigned long previousMillis2 = 0;
long interval2 = 1750;  // State duration for CW and CCW

const long STEP_PER_REVOLUTION = 100; // Define based on stepper motor's specifications

// State machine for the motor's direction and duration
enum MotorState1 {
  MOTOR1_CCW,
  MOTOR1_CW
};

enum MotorState2 {
  MOTOR2_CW,
  MOTOR2_PAUSE,
  MOTOR2_CWW
};

MotorState1 currentMotorState1 = MOTOR1_CCW;
MotorState2 currentMotorState2 = MOTOR2_CW;

void setup() {
  // Setup for stepper 1
  stepper1.setMaxSpeed(500);
  stepper1.setSpeed(-500);  // Start CCW

  // Setup for stepper 2
  stepper2.setMaxSpeed(500);
  stepper2.setSpeed(-500);  // Start CCW
}

void loop() {
  unsigned long currentMillis = millis();

  // Logic for stepper 1
  switch (currentMotorState1) {
    case MOTOR1_CCW:
      if (currentMillis - previousMillis1 >= interval1) {
        stepper1.setSpeed(500);  // Switch to CW
        currentMotorState1 = MOTOR1_CW;
        previousMillis1 = currentMillis;
      }
      break;
    case MOTOR1_CW:
      if (currentMillis - previousMillis1 >= interval1) {
        stepper1.setSpeed(-500);  // Switch to CCW
        currentMotorState1 = MOTOR1_CCW;
        previousMillis1 = currentMillis;
      }
      break;
  }

  // Logic for stepper 2
  switch (currentMotorState2) {
    case MOTOR2_CW:
      if (currentMillis - previousMillis2 >= interval2) {
        stepper2.setSpeed(0);  // Pause
        currentMotorState2 = MOTOR2_PAUSE;
        previousMillis2 = currentMillis;
        interval2 = 800;  // Next state lasts for 0.4 seconds
      }
      break;
    case MOTOR2_PAUSE:
      if (currentMillis - previousMillis2 >= interval2) {
        stepper2.setSpeed(500);  // Move CW
        currentMotorState2 = MOTOR2_CWW;
        previousMillis2 = currentMillis;
        interval2 = 1600;  // Next state lasts for 0.8 seconds
      }
      break;
    case MOTOR2_CWW:
      if (currentMillis - previousMillis2 >= interval2) {
        stepper2.setSpeed(-500);  // Move CCW
        currentMotorState2 = MOTOR2_CCW;
        previousMillis2 = currentMillis;
        interval2 = 1750;  // Reset to initial state duration
      }
      break;
  }

  // Run the steppers at the set speeds
  stepper1.runSpeed();
  stepper2.runSpeed();
}
