#include <Servo.h>

Servo srv;
int pinIR = 9;
long stat;
int led1 = 10;
int led2 = 11;
int buzz = 6;

void setup() {
  srv.attach(7);
  pinMode(pinIR, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop() {  
  /*digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi*0.034/2;
  Serial.println(jarak);
  */
  
 stat = digitalRead(pinIR);
  Serial.println(stat);
  srv.write(0);
  if(stat == 0){
    srv.write(90);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    digitalWrite(buzz,HIGH);
    delay(1000);
  }
  if(stat == 1){
    srv.write(0);
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    digitalWrite(buzz,LOW);
    delay(1000);
  }
}
