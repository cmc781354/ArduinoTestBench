#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);

const long STEP_PER_REVOLUTION = 200; // define this based on your stepper motor's specifications

unsigned long previousMillis = 0; // stores last time update
long interval = 2000; // interval at which to run (milliseconds)

// State machine for the motor's direction and duration
enum MotorState {
  CW_3_SEC,
  CCW_1_SEC,
  CW_1_SEC,
  CCW_5_SEC
};

MotorState currentMotorState = CW_3_SEC;

void setup() {
  stepper.setMaxSpeed(1000); // Set max speed
  stepper.setSpeed(1000);    // Start with CW direction
}

void loop() {
  // unsigned long currentMillis = millis();

  // switch (currentMotorState) {
  //   case CW_3_SEC:
  //     if (currentMillis - previousMillis >= 2000) { // 3 seconds
  //       stepper.setSpeed(1000); // Change direction to CCW
  //       currentMotorState = CCW_1_SEC;
  //       previousMillis = currentMillis; // Remember the switch time
  //       interval = 1000; // Next interval for 1 second
  //     }
  //     break;

  //   case CCW_1_SEC:
  //     if (currentMillis - previousMillis >= interval) {
  //       stepper.setSpeed(-1000); // Change direction to CW
  //       currentMotorState = CW_1_SEC;
  //       previousMillis = currentMillis;
  //       interval = 1000; // Next interval for 1 second
  //     }
  //     break;

  //   case CW_1_SEC:
  //     if (currentMillis - previousMillis >= interval) {
  //       stepper.setSpeed(1000); // Change direction to CCW
  //       currentMotorState = CCW_5_SEC;
  //       previousMillis = currentMillis;
  //       interval = 2000; // Next interval for 5 seconds
  //     }
  //     break;

  //   case CCW_5_SEC:
  //     if (currentMillis - previousMillis >= interval) {
  //       stepper.setSpeed(-1000); // Change direction to CW
  //       currentMotorState = CW_3_SEC; // Loop back to the first state
  //       previousMillis = currentMillis;
  //       interval = 3000; // Next interval back to 3 seconds
  //     }
  //     break;
  // }

  stepper.runSpeed();
}
