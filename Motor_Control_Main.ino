/*
  EROVOUTIKA - Bluetooth DC Motor Control System
  Compatible with Arduino Nano and Uno
  
  This sketch enables wireless control of DC motors via Bluetooth
  using an HC-05 or HC-06 Bluetooth module.
  
  Connections:
  - Bluetooth RX to Arduino TX (pin 1)
  - Bluetooth TX to Arduino RX (pin 0)
  - Motor Driver IN1/IN2 to pins 9, 10
  - Motor Driver IN3/IN4 to pins 5, 6
  - Motor Driver GND to Arduino GND
*/

#include <SoftwareSerial.h>

// ============ PIN CONFIGURATION ============
#define BT_RX 0  // Hardware Serial RX
#define BT_TX 1  // Hardware Serial TX

// Motor 1 pins (L298N Motor Driver)
#define MOTOR1_IN1 9   // PWM pin
#define MOTOR1_IN2 10  // PWM pin

// Motor 2 pins (L298N Motor Driver)
#define MOTOR2_IN3 5   // PWM pin
#define MOTOR2_IN4 6   // PWM pin

// ============ MOTOR CONTROL VARIABLES ============
int motor1Speed = 0;
int motor2Speed = 0;
char command = 'S';  // Stop by default

// ============ SETUP ============
void setup() {
  // Initialize Serial communication with Bluetooth module
  Serial.begin(9600);  // HC-05/HC-06 default baud rate
  
  // Set motor control pins as outputs
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN3, OUTPUT);
  pinMode(MOTOR2_IN4, OUTPUT);
  
  // Initialize all motors to stop
  stopAllMotors();
  
  // Debug message
  Serial.println("Bluetooth Motor Control System Ready");
  Serial.println("Commands: F(forward), B(backward), L(left), R(right), S(stop), 0-9(speed)");
}

// ============ MAIN LOOP ============
void loop() {
  // Check if data is available from Bluetooth
  if (Serial.available() > 0) {
    command = Serial.read();
    
    // Process command
    processCommand(command);
  }
}

// ============ COMMAND PROCESSING ============
void processCommand(char cmd) {
  switch(cmd) {
    case 'F':  // Move Forward
      moveForward(motor1Speed);
      Serial.println("Forward");
      break;
      
    case 'B':  // Move Backward
      moveBackward(motor1Speed);
      Serial.println("Backward");
      break;
      
    case 'L':  // Turn Left
      turnLeft(motor1Speed);
      Serial.println("Left");
      break;
      
    case 'R':  // Turn Right
      turnRight(motor1Speed);
      Serial.println("Right");
      break;
      
    case 'S':  // Stop
      stopAllMotors();
      Serial.println("Stop");
      break;
      
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      // Speed control (0-9 = 0-255)
      motor1Speed = (cmd - '0') * 28;  // Convert 0-9 to 0-252
      motor2Speed = motor1Speed;
      Serial.print("Speed set to: ");
      Serial.println(motor1Speed);
      break;
      
    default:
      Serial.println("Unknown command");
      break;
  }
}

// ============ MOTOR CONTROL FUNCTIONS ============

// Move both motors forward
void moveForward(int speed) {
  analogWrite(MOTOR1_IN1, speed);
  analogWrite(MOTOR1_IN2, 0);
  analogWrite(MOTOR2_IN3, speed);
  analogWrite(MOTOR2_IN4, 0);
}

// Move both motors backward
void moveBackward(int speed) {
  analogWrite(MOTOR1_IN1, 0);
  analogWrite(MOTOR1_IN2, speed);
  analogWrite(MOTOR2_IN3, 0);
  analogWrite(MOTOR2_IN4, speed);
}

// Turn left (right motor forward, left motor backward)
void turnLeft(int speed) {
  analogWrite(MOTOR1_IN1, 0);
  analogWrite(MOTOR1_IN2, speed);
  analogWrite(MOTOR2_IN3, speed);
  analogWrite(MOTOR2_IN4, 0);
}

// Turn right (left motor forward, right motor backward)
void turnRight(int speed) {
  analogWrite(MOTOR1_IN1, speed);
  analogWrite(MOTOR1_IN2, 0);
  analogWrite(MOTOR2_IN3, 0);
  analogWrite(MOTOR2_IN4, speed);
}

// Stop all motors
void stopAllMotors() {
  analogWrite(MOTOR1_IN1, 0);
  analogWrite(MOTOR1_IN2, 0);
  analogWrite(MOTOR2_IN3, 0);
  analogWrite(MOTOR2_IN4, 0);
  motor1Speed = 0;
  motor2Speed = 0;
}
