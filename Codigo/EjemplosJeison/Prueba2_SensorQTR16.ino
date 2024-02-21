#include <QTRSensors16.h>
#define NUM_SENSORS             16  // numero de sensores usados
#define NUM_SAMPLES_PER_SENSOR  1  // numero de muestras
#define IN_PIN             A4  // PIN de entrada del multiplexor
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,7);  // RX,TX
int linea = 0;              //  0 para lineas negra, 1 para lineas blancas
int flanco_color = 0;      // aumenta o disminuye el valor del sensado
int en_linea = 500;         //valor al que considerara si el sensor esta en linea o no
int ruido =  en_linea;         //valor al cual el valor del sensor es considerado como ruido

QTRSensorsMux qtra((unsigned char[]) {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, (unsigned char) IN_PIN, (unsigned char[]) {
  A3, A2, A1, A0} );

unsigned int sensorValues[NUM_SENSORS];

float  Lectura_Sensor(void);  



void setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH); 
  for (uint16_t i = 0; i < 500; i++){ qtra.calibrate();}
  digitalWrite(1, LOW); 
  delay(2000);
  mySerial.begin(9600);
}

void loop() {
 float pos = Lectura_Sensor();
 mySerial.println(pos);
 delay(100);
 
}

float Lectura_Sensor(void) {                                      
 
float position = qtra.readLine(sensorValues, QTR_EMITTERS_ON, linea, flanco_color, en_linea, ruido ); // leemos posicion de la linea en la variable position
return position;
}
