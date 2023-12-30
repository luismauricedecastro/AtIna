#include <SoftwareSerial.h>
#include <SyRenSimplified.h>
#include <RPLidar.h>
#include <Servo.h>

Servo tilt_servo; //12V
Servo pan_servo; //5V

//Neck Servo Constants
int PAN_MAX = 90;
int PAN_MIDDLE = 80;
int PAN_MIN = 70;

int TILT_UP = 130;
int TILT_INIT = 140;
int TILT_LOW = 150;

//NOTAS
//Primero se debe conectar el Arduino al Pi
//Luego Flask web browser

const bool LIDAR_ENABLED = false;

RPLidar lidar;

//Pins donde estan conectados los motores de las ruedas
SoftwareSerial serial_left(NOT_A_PIN, 10);
SoftwareSerial serial_right(NOT_A_PIN, 11);

SyRenSimplified sr_left(serial_left);
SyRenSimplified sr_right(serial_right);

//Tiempo de espera en cada movimiento de las ruedas
const int DELAY = 20;

//PWM pin  donde esta conectado el LIDAR y controla su velocidad
#define RPLIDAR_MOTOR 3

//MM de distancia en donde los motores se detienen
#define LIDAR_SAFETY 400

 //distancia en mm defectada por el LIDAR
float distance;

//angulo defectado por el LIDAR
 // 0 = justo al frente
 // 0-45 y 315-360 es alrededor del frente
float angle;

void setup()
{ 
  //Inicializar LED
  pinMode(13, OUTPUT);

  tilt_servo.attach(8);
  pan_servo.attach(9);

  tilt_servo.write(TILT_INIT);
  pan_servo.write(PAN_MIDDLE);
    
  if (LIDAR_ENABLED)
  {
    //Inicializar Lidar
    lidar.begin(Serial1);

    //Configurar  pin mode del Lidar
    pinMode(RPLIDAR_MOTOR, OUTPUT);
  }

  //Iniciar comunicacion para recibir mensajes del Raspberry PI
  Serial.begin(57600);

  while (!Serial) {}

  //iniciar comunicacion serial con los motores
  serial_left.begin(9600);
  serial_right.begin(9600);

  //Iniciar LIDAR
  leer_lidar();
}

void loop()
{
  //PROCESAR MENSAJES DEL PUERTO SERIAL enviados por el Raspberry PI
  if (Serial.available())
  {
    //mensaje recibido

    String message = Serial.readStringUntil('\n');
    int speed = 0;

    String command = message.substring(0, 1);

    int msg_len = message.length();

    if (msg_len == 2)
    {
      String speed_msg = message.substring(1);

      if (speed_msg.equals("S"))
      {
        speed = 20;
      }
      else if (speed_msg.equals("M"))
      {
        speed = 40;
      }
      else if (speed_msg.equals("F"))
      {
        speed = 60;
      } 

      //speed = speed_msg.toInt();
    }

    if (command == "L")
    {
      turn_left(speed);
    }
    else if (command == "R")
    {
      turn_right(speed);
    }
    else if (command == "F")
    {
      forward(speed);
    }
    else if (command == "B")
    {
      back(speed);
    }
  }
}

////////////////////////////////
//COMANDOS LIDAR
////////////////////////////////

//Obtine informacion del LIDAR
void leer_lidar() {
  if (!LIDAR_ENABLED)
  {
    return;
  }

  if (IS_OK(lidar.waitPoint())) {
    distance = lidar.getCurrentPoint().distance; //distance value in mm unit
    angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    //bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    //byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

  } else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       
       // start motor rotating at max allowed speed
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}

//Indica si el LIDAR detecta una distancia segura
bool distancia_segura() {
  if (!LIDAR_ENABLED)
  {
    return true;
  }

  if (distance > 0 && distance < LIDAR_SAFETY)
  {
    if (
      (angle >= 0 && angle <= 55) || (angle >= 305 && angle <= 360)
    )
    {
      //Hay un obstaculo, aborta
      return false;
    }
  }

  return true;
}

void detener_lidar() {
  analogWrite(RPLIDAR_MOTOR, 0);
}

////////////////////////////////
//COMANDOS PARA MOVER RUEDAS
////////////////////////////////

void forward(int max_speed){ 

  int power;

/*
  //Iniciar solo si se puede leer el LIDAR
  bool es_seguro = true;
  int intentos = 0;

  //Lee 20 puntos y confirma que no haya obstaculos antes de iniciar
  while (intentos <= 20)
  {
    leer_lidar();

    if (distance > 0 && !distancia_segura())
    {
      //Se encontró un obstaculo
      es_seguro = false;
      break;
    }

    intentos++;
  }

  if (es_seguro)
  {
    blink();
  }
  else {
    blink();
    blink();
    return;
  }
*/

  //Acelerar
  for (power = 0; power <= max_speed; power ++)
  {
    //Leer Lidar
    leer_lidar();

    //Verificar distancia segura
    if (!distancia_segura())
    {
      //Detener rapido
      desacelerar(power, 10);
      
      blink();
      blink();
      blink();
      return;
    }

    //Mover motores
    sr_left.motor(power);
    sr_right.motor(power);
    delay(DELAY);
  }

  //Desacelerar
  desacelerar(power, DELAY);
}

void desacelerar(int velocidad_inicial, int rapidez) {
  for (int power = velocidad_inicial; power >= 0; power --)
  {
    sr_left.motor(power);
    sr_right.motor(power);
    delay(rapidez);
  }
}

void back(int max_speed){ 
  //Acelerar
  for (int power = 0; power <= max_speed; power ++)
  {
    sr_left.motor(-power);
    sr_right.motor(-power);
    delay(DELAY);
  }
  
  //Desacelerar
  for (int power = max_speed; power >= 0; power --)
  {
    sr_left.motor(-power);
    sr_right.motor(-power);
    delay(DELAY);
  }
}

void turn_right(int max_speed){  
  //Acelerar
  for (int power = 0; power <= max_speed; power ++)
  {
    sr_left.motor(-power);
    sr_right.motor(power);
    delay(DELAY);
  }
  
  //Desacelerar
  for (int power = max_speed; power >= 0; power --)
  {
    sr_left.motor(-power);
    sr_right.motor(power);
    delay(DELAY);
  }
}

void turn_left(int max_speed){  
  //Acelerar
  for (int power = 0; power <= max_speed; power ++)
  {
    sr_left.motor(power);
    sr_right.motor(-power);
    delay(DELAY);
  }
  
  //Desacelerar
  for (int power = max_speed; power >= 0; power --)
  {
    sr_left.motor(power);
    sr_right.motor(-power);
    delay(DELAY);
  }
}

////////////////////////////////
//COMANDOS LEDS
////////////////////////////////

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);           
  digitalWrite(LED_BUILTIN, LOW); 
  delay(100);  
}

////////////////////////////////
//NECK SERVO
////////////////////////////////

void yes_and_no() {

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

  for (pos = TILT_INIT; pos <= TILT_LOW; pos += 1) {
    tilt_servo.write(pos);
    delay(15); 
  }

  for (pos = TILT_LOW; pos >= TILT_INIT; pos -= 1) { 
    tilt_servo.write(pos);
    delay(15); 
  }
}

void left() {
  int pos;

  for (pos = PAN_MIDDLE; pos <= PAN_MAX; pos += 1) {
    pan_servo.write(pos);
    delay(15); 
  }
  
  for (pos = PAN_MAX; pos >= PAN_MIDDLE; pos -= 1) { 
    pan_servo.write(pos);
    delay(15); 
  }
}

void right() {
  int pos;
  
  for (pos = PAN_MIDDLE; pos >= PAN_MIN; pos -= 1) {
    pan_servo.write(pos);
    delay(15); 
  }
  
  for (pos = PAN_MIN; pos <= PAN_MIDDLE; pos += 1) { 
    pan_servo.write(pos);
    delay(15); 
  }
}
