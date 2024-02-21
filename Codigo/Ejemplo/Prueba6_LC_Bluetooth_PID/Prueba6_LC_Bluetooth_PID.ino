//*1.4,0.0,0.03,120.0
//*1.7,0.0,0.04,100.0

#include <QTRSensors.h>
QTRSensors qtr;

// Declaracion de Sub-Funciones
float  Lectura_Sensor(void);                                      
float  Lectura_Referencia(void);
float  Controlador(float, float);
void   Esfuerzo_Control(float);
unsigned long int   Tiempo_Muestreo(unsigned long int);//,unsigned int);           // Función para garantizar un tiempo de muestreo constante
void   Sintonia_Bluetooth(void);

// Declaracion de variables del lazo cerrado
unsigned long int Tm = 9.0;                                            //tiempo de muestreo en mili segundos
float Referencia=0.0, Control=0.0, Kp = 2.5, Ti = 0.0, Td = 0.095; 
float Salida=0.0, Error=0.0, Error_ant=0.0;                       //variables de control
float offset = 1.0, Vmax = 0.0;
char caracter; String datos;   //  sintonizacion bluetooth
int d1, d2, d3,d4;                //  sintonizacion bluetooth
String S_Kp, S_Ti, S_Td, S_Vmax;       //  sintonizacion bluetooth
unsigned long int Tinicio = 0;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

const int  PWM5_D = 5;                                             // Definición Pin 6 PWM Motor Derecho
const int  PWM11_IZ = 11;                                           // Definición Pin 5 PWM Motor Izquierdo 

void setup() {                                                  //funcion de configuracion
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13,OUTPUT);
  
  qtr.setTypeAnalog(); qtr.setEmitterPin(12);
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount); 
  delay(200);

  digitalWrite(13, HIGH); 
  for (uint16_t i = 0; i < 200; i++){ qtr.calibrate();  }
  digitalWrite(13, LOW); 
  delay(1000);  
}

void loop(){                                                   //ciclo principal
  if(Serial.available()) { Sintonia_Bluetooth();} 
  Tinicio    = millis();                                        // toma el valor en milisengundos
  Referencia = Lectura_Referencia();                            // funcion que recive el valor de referencia desde  MATLAB por comunicacion serial
  Salida     = Lectura_Sensor();                                // funcion de lectura de la variable salida del  proceso
  Control    = Controlador(Referencia,Salida);                  // funcion de la ley de control 
  Esfuerzo_Control(Control);                                    // funcion encargada de enviar el esfuerzo de control
  Tm = Tiempo_Muestreo(Tinicio);               
}

float Lectura_Referencia(void) {                               //retorna el valor de la referencia o set point deseado
  Referencia = 0.0;                                  //la comunicacion se puede hacer desde Matlab o pueder ser una seÃ±al  diferente   
  return Referencia;
}

float Lectura_Sensor(void) {                                             
  Salida = (qtr.readLineBlack(sensorValues)/3500.0) - 1.0;
  //Serial.println(Salida);
  return Salida;                                               // retorno la variable de salidad del proceso normalizada entre 0-1, al olgoritmo de control
}

float Controlador(float Referencia, float Salida) {                           // Funcion para la ley de control
  float E_derivativo;
  float E_integral;
  float Control;

  Error_ant       = Error; 
  Error           = Referencia - Salida;
  //if(abs(Error)< 0.01){  Error = 0.0;}
  E_integral     = E_integral + ((Error*(Tm/1000.0)) + ((Tm/1000.0)*(Error - Error_ant))/2.0);
  if(E_integral> 100.0){ E_integral=  100.0;}
  if(E_integral<-100.0){ E_integral= -100.0;}
  E_derivativo   = (Error - Error_ant)/(Tm/1000.0);
  //Control         = Kp*( Error + Ti*E_integral + Td*E_derivativo );
 Control         = Kp*( Error + Ti*E_integral + Td*E_derivativo );
 
  if(Control > 1.5){  Control =  1.5;}
  if(Control < -1.5){ Control = -1.5;}
  //Serial.println(Control); 
  return Control;
}

void Esfuerzo_Control(float Control) {                            //envia el esfuerzo de control en forma de PWM
  float s1 , s2;

  s1  = (offset - Control);  
  s2  = (offset + Control);
  //Serial.print(s1); Serial.print("     "); Serial.println(s2);
  analogWrite(PWM5_D,   constrain(abs(s1), 0.0, 1.0)* Vmax);
  analogWrite(PWM11_IZ, constrain(abs(s2), 0.0, 1.0)* Vmax);

   if( s1 <= 0.0 ){// Motor Derecho
    digitalWrite(6,LOW);
    digitalWrite(4,HIGH);}
  else{
      digitalWrite(4,LOW);
      digitalWrite(6,HIGH);}                   
   
  
  if( s2 <= 0.0 ){ //Motor Izquierdo
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);}
  else{
      digitalWrite(9,LOW);
      digitalWrite(8,HIGH);} 
  
  } 

unsigned long int Tiempo_Muestreo(unsigned long int Tinicio){//, unsigned int Tm){ // Funcion que asegura que el tiempo de muestreo sea el mismo siempre
  //while ((millis() - Tinicio) < Tm);
  //Serial.println(millis() - Tinicio);
  unsigned long int T =millis()-Tinicio;
  return  T;
  }

void Sintonia_Bluetooth(void){
  caracter = Serial.read();  
  datos = datos + caracter; 
  if (caracter == '*') {
      d1    = datos.indexOf(',');
      S_Kp  = datos.substring(0, d1);
      d2    = datos.indexOf(',', d1+1);
      S_Ti  = datos.substring(d1+1, d2);
      d3    = datos.indexOf(',', d2+1);
      S_Td  = datos.substring(d2+1, d3);
      d4    = datos.indexOf(',', d3+1);
      S_Vmax  = datos.substring(d3+1, d4);
            
      datos = "";
      Kp = S_Kp.toFloat();  Ti = S_Ti.toFloat();   Td = S_Td.toFloat();  Vmax = S_Vmax.toFloat();    
  }
  }  
