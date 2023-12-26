#include <Servo.h>

Servo myservo;

void setup() {
  pan_servo.attach(9);
}

void loop() {
  int pos;
  for (pos = 60; pos <= 70; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    pan_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
  for (pos = 70; pos >= 60; pos -= 1) { // goes from 180 degrees to 0 degrees
    pan_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
