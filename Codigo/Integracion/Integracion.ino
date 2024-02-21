/*
  Integracion codigos sensor, motores, turbina
  21/feb/2024
*/

#include "Sensor.h"

//OBJETOS
Sensor sensor();

void setup() {

  Serial.begin(115200);

  //Calibracion sensor linea
  sensor.calibracion();
}

void loop() {
  float salida = sensor.Lectura_Sensor();
  Serial.println(Salida);
}
