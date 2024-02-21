#include <Servo.h>
//#include <ESC.h>

// valor minimo turbina 50 con servo
// valor maximo turbina 150 con servo
// valor minimo turbina 0 con ESC y servo
// valor maximo turbina 1000 con ESC y servo

//ESC Turbina;
Servo Turbina;

void setup() {
  Turbina.attach(9);
  //Turbina.setSpeed(1000);
  Turbina.writeMicroseconds(1000);
  delay(5000);
  
}

void loop() {
  Turbina.write(30); //150-50
  //Turbina.setSpeed(200);
  delay(3000);
  /*
 Turbina.write(100);
  //Turbina.setSpeed(500);
  delay(3000);*/
}
