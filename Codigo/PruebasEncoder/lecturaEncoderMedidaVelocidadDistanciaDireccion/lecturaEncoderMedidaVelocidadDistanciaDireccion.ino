/*
  Prueba de encoder para motor N20 usando ESP32 LOLIN S2 mini
      - se asume un deltaT dado por la lectura A.
      - se calcula la velocidad del motor
      - se calcula la distancia del motor
      - prueba de motor de 3000 RPM con relación de 1:10
      
  03-Diciembre-2023
  Sofia D. 
*/

//LIBRERIAS

//PINES 
#define bin1        34
#define bin2        21
#define pwmB        17
#define pinEncoderA 39   //Pin de interrupción
#define pinEncoderB 40   //Pin de interrupción
#define pinLed      15

//CONSTANTES
float pi    = 3.1415926535897932384626433832795;
float PPR   = 60.0; //pulsos por segundo dado por relacion*CPR de encoder
float radio = 0.0115;   //Radio en metros de la rueda de 23mm de diametro

//VARIABLE
volatile float  ISRCounter = 0.0;    //conteo de interrupciones
float           pulsos     = 0.0;    //Número de pulsos contados
float           relPPR     = 0.0;    //relación pulsos/PPR
unsigned long   tiempo=0;
float           deltaTAnterior=0.0, deltaT=0.0;
float           vel=0.0, dis=0.0;
long            contadorDis=0,      contadorPos=0;
float           Vdistancias[500];
float           VvelocidadesM1[500];
//boolean         banGiroAnterior=false, banGiro=false;


#define canal1 0
const int  frecuencia = 5000;
const byte resolucion = 10;
int        pmwMotor=pow(2,resolucion)-1;

//FUNCIONES
void motor(int);
void lecturaEncoderA();
//void lecturaEncoderB();

//------------------
//                  SETUP()
//------------------
void setup() {
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  
  //configuración de motor
  pinMode(bin1,OUTPUT);
  pinMode(bin2,OUTPUT); 
  ledcSetup(canal1, frecuencia, resolucion);
  ledcAttachPin(pwmB, canal1);

  //configuración de encoder
  pinMode(pinEncoderA,INPUT); 
  pinMode(pinEncoderB,INPUT); 
  attachInterrupt(digitalPinToInterrupt(pinEncoderA), lecturaEncoderA, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(pinEncoderB), lecturaEncoderB, CHANGE);

  delay(100);
  motor(1);
  tiempo=millis();
}

//------------------
//                  SETUP()
//------------------
void loop() {   
  pulsos=ISRCounter;
  //Serial.println(pulsos);
  if(pulsos>=PPR /*|| banGiro!=banGiroAnterior*/){
    //if(deltaT!=deltaTAnterior)
    deltaT=(float)(millis()-tiempo);
    tiempo=millis();
    relPPR=pulsos/PPR;
    //vel=(2*pi)/deltaT;
    vel=(relPPR*60000)/deltaT;
    dis=dis+(2*pi*radio*relPPR);
    
    //Serial.print("relacion: ");
    //Serial.print(relPPR);
    //Serial.print("   ¿Giro Adelante?: ");
    //Serial.print(banGiro);
    //Serial.print("   delta T: ");
    //Serial.print(deltaT);
    //Serial.print("   vel: ");
    Serial.println(vel);
    //Serial.print("[RPM]");
    /*Serial.print("   distancia: ");
    Serial.print(dis);
    Serial.println(" [m]");
    Serial.println();*/

    ISRCounter = 0.0;
    pulsos = 0.0;
    deltaT=0.0;
        
  }
  
}

//FUNCIONES
void motor(int vel){
  digitalWrite(bin1,HIGH);
  digitalWrite(bin2,LOW );
  //ledcWrite(canal1, vel);
  ledcWrite(canal1, (int)(vel*pmwMotor));
};

void lecturaEncoderA(){
  ISRCounter++;
  //direccion de Giro
  /*
  if(digitalRead(pinEncoderA) && !digitalRead(pinEncoderB)){
    banGiro=false;
  }
  if(!digitalRead(pinEncoderA) && digitalRead(pinEncoderB)){
    banGiro=true;
  }
  */
  
}
