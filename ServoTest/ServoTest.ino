#include <Servo.h>

Servo pan_servo;

void setup() {
  pan_servo.attach(9);
}

void loop() {
  int pos;
  for (pos = 50; pos <= 80; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    pan_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }

  delay(1000);
  
  for (pos = 80; pos >= 50; pos -= 1) { // goes from 180 degrees to 0 degrees
    pan_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }

  delay(1000);
}
