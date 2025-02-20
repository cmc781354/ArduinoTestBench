#include <AccelStepper.h>

#define STEP_PIN 4
#define DIR_PIN 5
#define EN_PIN 6

AccelStepper artery(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); //create driver object

const long STEP_PER_REVOLUTION = 200; // define this based on your stepper motor's specifications
const uint8_t BPM = 60;              // set the heart rate of the pump
const float frequency = BPM / 60;      // convert BPM into Hz

unsigned long previousMillis = 0; // stores last time update
long interval = 1500; // interval at which to run (milliseconds)

// State machine for the motor's direction and duration, names reference systole and diastole
enum MotorState {
  sys_rise,
  sys_fall,
  dia_rise,
  dia_fall
};

MotorState currentMotorState = sys_rise;

void setup() {
  artery.setMaxSpeed(250 / ((1 / frequency) * 0.375)); // Set max speed as position to travel / time to travel
  artery.setAcceleration(2000);
  artery.move(250);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentMotorState) {
    case sys_rise:
      if (currentMillis - previousMillis >= ((1000 / frequency) * 0.375) + 50) {
        artery.setMaxSpeed(125 / (float) ((1 / frequency) * 0.125)); // Change max speed 
        artery.move(-125); // Change direction to CCW
        currentMotorState = sys_fall;
        previousMillis = currentMillis; // Remember the switch time
        //interval = 3000; // Next interval for 1 second
      }
      break;

    case sys_fall:
      if (currentMillis - previousMillis >= ((1000 / frequency) * 0.125) + 100) {
        artery.setMaxSpeed(25 / (float) ((1 / frequency) * 0.167)); // Change max speed 
        artery.move(25); // Change direction to CW
        currentMotorState = dia_rise;
        previousMillis = currentMillis;
        //interval = 3000; // Next interval for 1 second
      }
      break;

    case dia_rise:
      if (currentMillis - previousMillis >= ((1000 / frequency) * 0.167) + 50) {
        artery.setMaxSpeed(150 / (float) ((1 / frequency) * 0.333)); // Change max speed 
        artery.move(-150); // Change direction to CCW
        currentMotorState = dia_fall;
        previousMillis = currentMillis;
        //interval = 3000; // Next interval for 5 seconds
      }
      break;

    case dia_fall:
      if (currentMillis - previousMillis >= ((1000 / frequency) * 0.333) + 100) {
        artery.setMaxSpeed(250 / (float) ((1 / frequency) * 0.375)); // Change max speed 
        artery.move(250); // Change direction to CW
        currentMotorState = sys_rise; // Loop back to the first state
        previousMillis = currentMillis;
        //interval = 3000; // Next interval back to 3 seconds
      }
      break;
  }

  artery.run();
}
