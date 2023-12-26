#include <RPLidar.h>

// You need to create an driver instance 
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal 

#define LIDAR_SAFETY 400

float distance = 0;
float angle = 0;

void setup() {
  pinMode(13, OUTPUT);

  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(Serial1);

  Serial.begin(9600);
  
  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);           
  digitalWrite(LED_BUILTIN, LOW); 
  delay(100);  
}

void loop() {

  leer_lidar();

  if (!distancia_segura())
  {
    Serial.print(angle);
    Serial.print(" --- ");
    Serial.println(distance);
  }
}

//Obtine informacion del LIDAR
void leer_lidar() {
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
