
//NOTE :
//Semua posisi servo terlebih dahulu di kalibrasi ke posisi 90 derajat

#include <Servo.h>

Servo myservo; //servo1 = servo atas bawah
Servo myservo2;//servo2 = servo naik turun
Servo myservo3;//servo3 = servo capitan
Servo myservo4;//servo4 = servo berputar


int pos = 0;  
int pos2 = 0; 
int pos3 = 0; 
int pos4 = 0; 
void setup() {
  myservo.attach(6); 
  myservo2.attach(9); 
  myservo3.attach(5);
    myservo4.attach(3);
}

void loop() {
  for (pos = 10; pos <= 45; pos += 1) { 
 
    myservo.write(pos);             
    delay(15);                      
  }
  for (pos = 45; pos >= 10; pos -= 1) {
    myservo.write(pos);             
    delay(15);                     
  }


  for (pos2 = 90; pos2 <= 120; pos2 += 1) { 
  
    myservo2.write(pos2);              
    delay(15);                       
  }
  for (pos2 = 120; pos2 >= 90; pos2 -= 1) { 
    myservo2.write(pos2);              
    delay(15);                     
  }

 for (pos3 = 150; pos3 <= 170; pos3 += 1) { 
    // in steps of 1 degree
    myservo3.write(pos3);              
    delay(15);                      
  }
  for (pos3 = 170; pos3 >= 150; pos3 -= 1) { 
    myservo3.write(pos3);             
    delay(15);                       
  }


 for (pos4 = 90; pos4 <= 120; pos4 += 1) { 
    
    myservo4.write(pos4);             
    delay(15);                       
  }
  for (pos4 = 120; pos4 >= 90; pos4 -= 1) { 
    myservo4.write(pos4);              
    delay(15);                       
  }


  
}
