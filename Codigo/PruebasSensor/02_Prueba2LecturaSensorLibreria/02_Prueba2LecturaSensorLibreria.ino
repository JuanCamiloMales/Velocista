/*
  Prueba sensor de Línea de 16 canales
  13/feb/2024
  
  tarjeta esp32 wemos s2 mini. 
  Se modifica la Libreria QTRsensor
*/

//LIBRERIAS
#include <QTRSensors.h>

//VARIABES  
const uint8_t sensorCount  = 4;                     //4 pines de lectura; en la librería se realiza la operacion pow(2,sensorCount)
const uint8_t readingCount = pow(2,sensorCount);    //número de lectura a obtener en un ciclo (16 sensores=2^{4})
const uint8_t signalPin    = 12;                    //pin 12 recibe la lectura; en librería se tiene en cuenta una resolución de 13-bits
uint16_t      sensorValues[readingCount];
float         Salida=0.0;

//FUNCIONES
float  Lectura_Sensor(void);    

//OBJETOS
QTRSensors qtr;

//--------------------------------------------------SETUP()
void setup() {
  Serial.begin(115200);
  pinMode(15,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(5,OUTPUT);
  
  
  qtr.setTypeAnalog(signalPin);                                //Se indica el pin INPUT de lectura analógica o digital del sensor
  qtr.setSensorPins((const uint8_t[]){11,9,7,5}, sensorCount); //array de pines de lectura ordenados desde LSB hacia MSB
  delay(200);

  digitalWrite(15, HIGH); 
  for (uint16_t i = 0; i < 200; i++){ qtr.calibrate();  }
  digitalWrite(15, LOW); 
  delay(1000); 
   
}

//--------------------------------------------------LOOP()
void loop() {
  Salida     = Lectura_Sensor(); 
  delay(500);
}

float Lectura_Sensor(void) {                                             
  Salida = (qtr.readLineBlack(sensorValues)/7550.0)-1.0;
  Serial.println(Salida);
  return Salida;                                               // retorno la variable de salidad del proceso normalizada entre 0-1, al olgoritmo de control
}
