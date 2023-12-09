//==== NODEMCU ESP 8266 AMICA =====
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//#define led 5//D1
//yang udah kepake pin: D2, D4, D5, D6, D7 D1, D3, SD3, D7, D8, RX
//JANGAN PAKAI PIN SD2, CMD KARENA JADI EROR

//Relay
//#define relay_1 16 //D0
//#define relay_2 10 //SD3
//#define relay_3 5 //D1

//Input relay
//#define relayInput_1 0 //D3
//#define relayInput_2 15 //D8
//#define relayInput_3 3 //RX


//Baca relay
int input_R1;
int input_R2;
int input_R3;

//Komunikasi SPI
RF24 radio(2,4);

//variabel millis
unsigned long tb1 = 0;
unsigned long tb2 = 0;
unsigned long tb3 = 0;
unsigned long tb4 = 0;

unsigned long tn1;
unsigned long tn2;
unsigned long tn3;
unsigned long tn4;

const byte address[] = "node1";

void setup() {
  Serial.begin(9600);
  //pinMode(led, OUTPUT);
  //pinMode(relay_1, OUTPUT);
  //pinMode(relay_2, OUTPUT);
  //pinMode(relay_3, OUTPUT);
  //pinMode(relayInput_1, INPUT);
  //pinMode(relayInput_2, INPUT);
  //pinMode(relayInput_3, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Power_1, Energy_1, Voltage_1, Current_1, Power_2, Energy_2, Voltage_2, Current_2, deltaP");  
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_1MBPS);
  radio.startListening();
}

struct datapack {
//PZEM 1
float pzem1_power;
float pzem1_energy;
float pzem1_voltage;
float pzem1_current;
//float pzem1_frequency;
//float pzem1_pf;

//PZEM 2
float pzem2_power;
float pzem2_energy;
float pzem2_voltage;
float pzem2_current;
//float pzem2_frequency;
//float pzem2_pf;
//Supervised system
//float deltaPower;//Delta power
};
datapack data;

void loop() {
  float power_1, energy_1, voltage_1, current_1, frequency_1, pf_1;
  float power_2, energy_2, voltage_2, current_2, frequency_2, pf_2, deltaP;
tn1=millis();
if(tn1-tb1>10000){
  if (radio.available()){
    radio.read(&data, sizeof(data));
    //========= READING MESSAGE =========
    //========= PZEM 1 =========
    //POWER
    Serial.print(",");
    //Serial.print("Power_1: "); 
    Serial.print(data.pzem1_power);
    Serial.print(",");

    //ENERGY
    //Serial.print("Energy_1: "); 
    Serial.print(data.pzem1_energy);
    Serial.print(",");

    //VOLTAGE
    //Serial.print("Voltage_1: "); 
    Serial.print(data.pzem1_voltage);
    Serial.print(",");

    //CURRENT
    //Serial.print("Current_1: "); 
    Serial.print(data.pzem1_current);
    Serial.print(",");

    //FREQUENCY
    //Serial.print("Frequency_1: "); Serial.println(data.pzem1_frequency);

    //POWER FACTOR
    //Serial.print("PF_1: "); Serial.println(data.pzem1_pf);

    //========= PZEM 2 =========

    //Serial.print("=========================================="); Serial.println();
    //POWER
    //Serial.print("Power_2: "); 
    Serial.print(data.pzem2_power);
    Serial.print(",");

    //ENERGY
    //Serial.print("Energy_2: "); 
    Serial.print(data.pzem2_energy);
    Serial.print(",");

    //VOLTAGE
    //Serial.print("Voltage_2: "); 
    Serial.print(data.pzem2_voltage);
    Serial.print(",");

    //CURRENT
    //Serial.print("Current_2: "); 
    Serial.print(data.pzem2_current);
    Serial.print(",");

    //FREQUENCY
    //Serial.print("Frequency_2: "); Serial.println(data.pzem2_frequency);

    //POWER FACTOR
    //Serial.print("PF_2: "); Serial.println(data.pzem2_pf);

    //DELTA POWER (SUPERVISED SYSTEM)
    deltaP = data.pzem1_power - data.pzem2_power;
    //Serial.print("Delta Power: "); 
    Serial.println(deltaP);
    
    //Serial.print("=========================================="); Serial.println();
/*
//================  SISTEM SUPERVISI  =======================
//power_1 = beban (yg harus selalu lebih besar)
//power_2 = PV (yg harus selalu lebih kecil)

//------- MEMATIKAN RELAY ---------
//Mematikan 1
if (input_R1==0 && input_R2==0 && input_R3==0 && deltaP < 0){
digitalWrite(relay_1, HIGH);
digitalWrite(relay_2, LOW);
digitalWrite(relay_3, LOW);
//Serial.println("fase 1: relay 1 nyala & PV 1 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
} 


//Mematikan 2
else if (input_R1==1 && input_R2==0 && input_R3==0 && deltaP < 0){
digitalWrite(relay_1, HIGH);
digitalWrite(relay_2, HIGH);
digitalWrite(relay_3, LOW);
//Serial.println("fase 2: relay 1,2 nyala & PV 1,2 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}
//Mematikan 3
else if (input_R1==1 && input_R2==1 && input_R3==0 && deltaP < 0){
digitalWrite(relay_1, HIGH);
digitalWrite(relay_2, HIGH);
digitalWrite(relay_3, HIGH);
//Serial.println("fase 3: relay 1,2,3 nyala & PV 1,2,3 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}

//------- MENYALAKAN RELAY ---------
//Menyalakan 1
else if (input_R1==1 && input_R2==0 && input_R3==0 && deltaP > 0 && deltaP > 95){
digitalWrite(relay_1, LOW);
digitalWrite(relay_2, LOW);
digitalWrite(relay_3, LOW);
//Serial.println("fase 4: relay 1,2,3 mati & PV 1,2,3 nyala (Menyalakan)");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}
//Menyalakan 2
else if (input_R1==1 && input_R2==1 && input_R3==0 && deltaP > 0 && deltaP > 95){
digitalWrite(relay_1, HIGH);
digitalWrite(relay_2, LOW);
digitalWrite(relay_3, LOW);
//Serial.println("fase 5: relay 1 nyala & PV 1 mati (Menyalakan)");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}
//Menyalakan 3
else if (input_R1==1 && input_R2==1 && input_R3==1 && deltaP > 0 && deltaP > 95){
digitalWrite(relay_1, HIGH);
digitalWrite(relay_2, HIGH);
digitalWrite(relay_3, LOW);
//Serial.println("fase 6: relay 1,2 nyala & PV 1,2 mati (Menyalakan)");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}

//Jika diluar kondisi diatas
else{
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.println(input_R3);
}
/*
   //======= LED SIGNAL DITERIMA =========
     digitalWrite(led, HIGH);
     delay(500);
     digitalWrite(led, LOW);
     delay(500);
*/
  }
  else{
    Serial.println("Tidak terhubung...");
  }
  tb1=tn1;
}
}
