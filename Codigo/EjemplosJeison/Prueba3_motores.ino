
#include<OrangutanMotors.h>
OrangutanMotors motors;

void setup() {
motors.setSpeeds(0, 0);  //Motor_derecho, Motor_izquierdo
pinMode(1, OUTPUT); 
digitalWrite(1, HIGH); 
delay(1000);
digitalWrite(1, LOW); 
}

void loop() {
  for(int i =0; i<=150;i++){
    motors.setSpeeds(i, i);   
    delay(100);
    }
    motors.setSpeeds(0, 0);
    delay(4000);

  for(int i =0; i>=-150;i--){
    motors.setSpeeds(i, i);  
    delay(100);
    }
    motors.setSpeeds(0, 0);
    delay(2000);

}
