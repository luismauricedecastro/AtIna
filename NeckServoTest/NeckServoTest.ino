#include <Servo.h>

Servo tilt_servo; //12V
Servo pan_servo; //5V

int MAX = 90;
int MIDDLE = 80;
int MIN = 70;

int TILTE_UP = 130;
int TILTE_INIT = 140;
int TILTE_LOW = 150;

void setup() {
  pinMode(13, OUTPUT);

  tilt_servo.attach(8);
  pan_servo.attach(9);

  tilt_servo.write(TILTE_INIT);
  pan_servo.write(MIDDLE);
}

void loop() {

  say_yes();

  blink();
  blink();
  blink();

  say_no();

  blink();
  blink();
  blink();
}

void say_yes() {
  up_down();
  up_down();
}

void say_no(){
  left();
  right();

  left();
  right();
}

void up_down() {
  int pos;

  for (pos = TILTE_INIT; pos <= TILTE_LOW; pos += 1) {
    tilt_servo.write(pos);
    delay(15); 
  }

  for (pos = TILTE_LOW; pos >= TILTE_INIT; pos -= 1) { 
    tilt_servo.write(pos);
    delay(15); 
  }
}

void left() {
  int pos;

  for (pos = MIDDLE; pos <= MAX; pos += 1) {
    pan_servo.write(pos);
    delay(15); 
  }
  
  for (pos = MAX; pos >= MIDDLE; pos -= 1) { 
    pan_servo.write(pos);
    delay(15); 
  }
}

void right() {
  int pos;
  
  for (pos = MIDDLE; pos >= MIN; pos -= 1) {
    pan_servo.write(pos);
    delay(15); 
  }
  
  for (pos = MIN; pos <= MIDDLE; pos += 1) { 
    pan_servo.write(pos);
    delay(15); 
  }
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

