#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//jarak_1 = horizontal (ke tebing)
//jarak_2 = vertical (ke bawah)

//PROJECT LONGSOR
//millis
int tn1;
int tb1;

LiquidCrystal_I2C lcd (0x27,16,2);

//ultrasonic 1
#define trigPin_1 6 //pin D6 
#define echoPin_1 7 //pin D7
long jarak_1;
long durasi_1;

//ultrasonic 2
#define trigPin_2  8 //pin D8
#define echoPin_2  9 //pin D9
long jarak_2;
long durasi_2;

//Aktuator atau Output
int ledRed = 4; //Merah
int ledGreen = 3; //Hijau
int buzzer = 5;

//vibration sensor
#define sensorVib 10 //pin D10
int vibrator;

void setup() {
  lcd.init();
  lcd.setCursor(0,0); lcd.print("STATUS LONGSOR");
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(sensorVib, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
 lcd.backlight();
 digitalWrite(trigPin_1, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin_1, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin_1, LOW);
 durasi_1 = pulseIn(echoPin_1, HIGH);
 jarak_1 = durasi_1*0.034/2;
 Serial.print("Jarak Ultrasonic 1 = "); Serial.println(jarak_1);

 //Ultrasonic 2
 digitalWrite(trigPin_2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin_2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin_2, LOW);
 durasi_2 = pulseIn(echoPin_2, HIGH);
 jarak_2 = durasi_2*0.034/2;
 Serial.print("Jarak Ultrasonic 2 = "); Serial.println(jarak_2);

 //Vibration Sensor
 vibrator = digitalRead(sensorVib);
 Serial.print("Status Getaran = ");Serial.println(vibrator);

 //Kondisi 1: TIDAK LONGSOR
 if(jarak_1 > 30 && jarak_1 < 38 && jarak_2 > 15 && vibrator == 0){
  digitalWrite(buzzer, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
  lcd.setCursor(1,1); lcd.print("     AMAN      ");
  Serial.println("===AMAN===");
 }

//Kondisi 2: LONGSOR (SIAGA 1)
else if(jarak_1 < 31 && jarak_2 > 15 && vibrator == 1){
  tone(buzzer, 100);
  delay(500);  
  tone(buzzer, 800);
  delay(500);
  noTone(buzzer);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
  lcd.setCursor(1,1); lcd.print("     SIAGA 1      ");
  Serial.println("=== SIAGA 1 ! ! !===");
}

//Kondisi 3: LONGSOR (SIAGA 2)
else if(jarak_1 < 31 && jarak_2 > 16 && vibrator == 1){
  tone(buzzer, 200);
  delay(300);  
  tone(buzzer, 200);
  delay(300);
  noTone(buzzer);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
  lcd.setCursor(1,1); lcd.print("     SIAGA 2      ");
  Serial.println("=== SIAGA 2 ! ! !===");
}

//Kondisi 4: DILUAR KONDISI DI ATAS
else{
  digitalWrite(buzzer, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
  lcd.setCursor(1,1); lcd.print("     AMAN      ");
  Serial.println("=== STATUS AMAN ===");
 }
}
