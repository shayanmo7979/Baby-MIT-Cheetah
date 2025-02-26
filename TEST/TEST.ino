#include <Servo.h>

// Ultrasonic Sensor Definitions
#define SONIC_TRIG 3
#define SONIC_ECHO 2
#define DIST_THRESHOLD 50  // Distance threshold in cm

// Servo Speed Definitions
#define SERVO_PIN 4
#define SERVO2_PIN 5
#define STOPPED 90   
#define REVERSE_MAX_SPEED 0 
#define REVERSE_SLOW_SPEED 60
#define FORWARD_SLOW_SPEED 120
#define FORWARD_MAX_SPEED 180 

Servo myservo;
Servo myservo2;
float duration_us, distance_cm;

bool isReversing = false;       // Flag to track reversing state

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);           // Onboard LED indicator
  pinMode(SONIC_TRIG, OUTPUT);   // Ultrasonic Trigger pin
  pinMode(SONIC_ECHO, INPUT);    // Ultrasonic Echo pin
  myservo.attach(SERVO_PIN);     // Attach servo to pin 4
  myservo2.attach(SERVO2_PIN);     // Attach servo to pin 4
  Serial.println("ARDUINO TEST:");
  Serial.println("------------------");
  myservo.write(FORWARD_SLOW_SPEED);
  myservo2.write(REVERSE_SLOW_SPEED);
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(SONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONIC_TRIG, LOW);

  // Measure the duration of echo pulse
  duration_us = pulseIn(SONIC_ECHO, HIGH);

  // Calculate distance (speed of sound in air is ~0.034 cm/us, divided by 2 for round trip)
  distance_cm = (duration_us * 0.034) / 2.0;
  Serial.print("Distance: ");
  Serial.println(distance_cm);

  // Check distance threshold
  if (distance_cm < DIST_THRESHOLD && !isReversing) {
    digitalWrite(13, HIGH);  // Turn on LED when object is close
    myservo.write(STOPPED);  // Stop the servo
    myservo2.write(STOPPED);  // Stop the servo
    Serial.println("Obstacle detected! Stopping for 5 seconds...");
    delay(5000);             // Stop for 5 second
    
    // Reverse direction after 1 second
    Serial.println("Reversing...");
    myservo.write(REVERSE_SLOW_SPEED);
    myservo2.write(FORWARD_SLOW_SPEED);
    isReversing = true;      // Set reversing flag to true
  }

  // If distance is safe and servo was reversing, move forward again
  if (distance_cm >= DIST_THRESHOLD && isReversing) {
    digitalWrite(13, LOW);   // Turn off LED when safe
    myservo.write(STOPPED);
    myservo2.write(STOPPED);
    Serial.println("Path clear! Moving forward...");
    delay(1000);
    myservo.write(FORWARD_SLOW_SPEED);
    myservo2.write(FORWARD_SLOW_SPEED);
    isReversing = false;     // Reset reversing flag
  }
  
  delay(100); // Small delay to stabilize sensor readings
}





