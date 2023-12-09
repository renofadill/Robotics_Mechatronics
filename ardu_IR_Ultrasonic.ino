#include <Servo.h>

//millis
int tn1;
int tn2;
int tn3;
int tn4;

int tb1;
int tb2;
int tb3;
int tb4;

Servo srv;
int trigPin = 9;
int echoPin = 8;
long jarak;
long durasi;
int led1 = 10;
int led2 = 11;
int buzz = 6;
int sensorIR = 5;
int stat;
void setup() {
  srv.attach(7);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorIR, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop() {  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi*0.034/2;
  Serial.println(jarak);
  stat = digitalRead(sensorIR);
  Serial.println(stat);
  srv.write(0);

  tn1=millis();
  if(tn1-tb1>500){
  if(jarak <= 10){
    srv.write(90);
    delay(500);
    digitalWrite(buzz,HIGH);
  }
  tb1=tn1;
  }
  
  tn2=millis();
  if(tn2-tb2>500){
  if(jarak > 10){
    srv.write(0);
    delay(500);
    digitalWrite(buzz,LOW);
  }
  tb2=tn2;
  }

  tn3=millis();
  if(tn3-tb3>500){
  if(stat == 1){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
  }
  tb3=tn3;
  }

  tn4=millis();
  if(tn4-tb4>500){
  if(stat == 0){
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
  }
  tb4=tn4;
  }
}
