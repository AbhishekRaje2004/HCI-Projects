#include <Servo.h>

// Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 8
#define MOTOR_LA 4
#define MOTOR_LB 5
#define MOTOR_RA 6
#define MOTOR_RB 7
#define SERVO_PIN 10
#define EMG_PIN A0  // Analog input pin that is connected to EMG OUT

// Constants for operation
const float SOUND_VELOCITY = 58.2;  // Speed of sound divided by 2, in microseconds per cm

// Servo and Ultrasonic sensor setup
Servo myServo;
long duration, distance;
int emgValue = 0;

// Q-Learning parameters
const int stateCount = 3;  // Example: 3 states (0: far, 1: near, 2: very near)
const int actionCount = 4;  // Actions (0: forward, 1: left, 2: right, 3: stop)
float Q[stateCount][actionCount];  // Q-table
float learningRate = 0.1;
float discountFactor = 0.95;
int currentState, newState;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_LA, OUTPUT);
  pinMode(MOTOR_LB, OUTPUT);
  pinMode(MOTOR_RA, OUTPUT);
  pinMode(MOTOR_RB, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EMG_PIN, INPUT);
  myServo.attach(SERVO_PIN);
  initializeQTable();  // Initialize Q-table for Q-learning
}

void loop() {
  emgValue = analogRead(EMG_PIN);  // Read the muscle activity level
  if (emgValue > 512) {  // EMG threshold for triggering emergency stop
    executeEmergencyStop();
  } else {
    measureDistance();
    makeDecision();
  }
  delay(100);
}

void initializeQTable() {
  for (int i = 0; i < stateCount; i++) {
    for (int j = 0; j < actionCount; j++) {
      Q[i][j] = 0.0;  // Initial values in Q-table are set to 0
    }
  }
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / SOUND_VELOCITY;
  Serial.println(distance);
  updateState();
}

void updateState() {
  if (distance > 20) currentState = 0;
  else if (distance > 10) currentState = 1;
  else currentState = 2;
}

void makeDecision() {
  int action = chooseAction(currentState);
  performAction(action);
  updateQTable(currentState, action, computeReward(), currentState);  // Immediate feedback assumption
}

int chooseAction(int state) {
  int bestAction = 0;
  float bestValue = Q[state][0];
  for (int i = 1; i < actionCount; i++) {
    if (Q[state][i] > bestValue) {
      bestAction = i;
      bestValue = Q[state][i];
    }
  }
  return bestAction;
}

void updateQTable(int s, int a, int reward, int new_s) {
  float maxQ = 0;
  for (int i = 0; i < actionCount; i++) {
    if (Q[new_s][i] > maxQ) maxQ = Q[new_s][i];
  }
  Q[s][a] += learningRate * (reward + discountFactor * maxQ - Q[s][a]);
}

int computeReward() {
  // Simplistic reward function
  if (distance > 15) return 1;  // Reward for maintaining a safe distance
  else if (distance < 10) return -1;  // Penalty for getting too close
  return 0;  // Neutral reward
}

void performAction(int action) {
  switch (action) {
    case 0: moveForward(); break;
    case 1: turnLeft(); break;
    case 2: turnRight(); break;
    case 3: moveBackwards();break;
    case 4: stopMotors(); break;
  }
}

void executeEmergencyStop() {
  stopMotors();
  Serial.println("EMG Stop Triggered!");
  delay(1000);  // Delay to ensure the stop is noticed
}

void moveForward() {
    myServo.write(90);
    digitalWrite(MOTOR_RB, HIGH);       // Move Forward
    digitalWrite(MOTOR_RA, LOW);
    digitalWrite(MOTOR_LB, HIGH);                                
    digitalWrite(MOTOR_LA, LOW);          
}

void moveBackwards()
{
    myServo.write(90);
    digitalWrite(MOTOR_RB, LOW);       // Move Backward
    digitalWrite(MOTOR_RA, HIGH);
    digitalWrite(MOTOR_LB, LOW);                                
    digitalWrite(MOTOR_LA, HIGH);          
}

void turnRight() {
  myServo.write(60);  // Slightly turn the servo for a gentle turn
  digitalWrite(MOTOR_RA, HIGH);
  digitalWrite(MOTOR_RB, LOW);
  digitalWrite(MOTOR_LA, LOW);
  digitalWrite(MOTOR_LB, HIGH);
  delay(500);  // Delay for the turn to take effect
}

void turnLeft() {
  myServo.write(120);  // Slightly turn the servo for a gentle turn
  digitalWrite(MOTOR_RA, LOW);
  digitalWrite(MOTOR_RB, HIGH);
  digitalWrite(MOTOR_LA, HIGH);
  digitalWrite(MOTOR_LB, LOW);
  delay(500);  // Delay for the turn to take effect
}

void stopMotors() {
  digitalWrite(MOTOR_RA, LOW);
  digitalWrite(MOTOR_RB, LOW);
  digitalWrite(MOTOR_LA, LOW);
  digitalWrite(MOTOR_LB, LOW);
}
