int led1 = 10;
int led2 = 11;
int buzz = 6;

//Sensor IR
const int pinIR = 9;
void setup(){
 pinMode(pinIR, INPUT);
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 pinMode(buzz, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  int bacaSensor = digitalRead(pinIR);
  if(bacaSensor == 1){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    digitalWrite(buzz,HIGH);
    delay(1000);
  }
  if(bacaSensor == 0){
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    digitalWrite(buzz,LOW);
    delay(1000);
  }
}
