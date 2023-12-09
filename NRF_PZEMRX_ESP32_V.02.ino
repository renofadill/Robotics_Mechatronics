//==== NODEMCU ESP32 =====
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <ThingerESP32.h>

#define led 2//D2
//yang udah kepake pin: D25, D33, D32, D13, D27, D26, D22, D21, D19, D18, D23
//JANGAN PAKAI PIN SD2, CMD KARENA JADI EROR

//Relay
#define relay_1 25 //D25
#define relay_2 33 //D33
#define relay_3 32 //D32

//Input relay
#define relayInput_1 13 //D13
#define relayInput_2 27 //D27
#define relayInput_3 26 //D26

//Thinger.io configuration
#define USERNAME "renofadill"
#define DEVICE_ID "ESP32_SS_v02"
#define DEVICE_CREDENTIAL "&AY1ie@PEdSXa8f?"
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//WIFI
const char* ssid = "04F2G_135cb0_plus";
const char* password = "wlaneca34f";

//Baca relay
int input_R1;
int input_R2;
int input_R3;
int input_total1;
int input_total2;
int input_total3;

//Power PV
float ppv_1, ppv_2, ppv_3;
//Komunikasi SPI
//RF24 radio(2,4);//esp8266
RF24 radio(22,21);//esp32 (CE, CSN)
//PINS: D21(CSN), D23(MOSI), D22(CE), D18(SCK), D19(MISO)  
//
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

//variabel PZEM 1 & 2
float power_1, energy_1, voltage_1, current_1, frequency_1, pf_1;
float power_2, energy_2, voltage_2, current_2, frequency_2, pf_2, deltaP;
float pv_3, pv_2, pv_1, out_pv_3, out_pv_2, out_pv_1;
  
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
  Serial.print("RESETTIMER");
  Serial.println();
  Serial.println(", Power_1, Energy_1, Voltage_1, Current_1, Power_2, Energy_2, Voltage_2, Current_2, Delta P, relay_1, relay_2, relay_3, pv_3, pv_2, pv_1");
  EEPROM.begin(512);
  //pinMode(led, OUTPUT);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relayInput_1, INPUT);
  pinMode(relayInput_2, INPUT);
  pinMode(relayInput_3, INPUT);
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  //WIFI THINGER.io
  WiFi.begin(ssid, password);
  thing.add_wifi(ssid, password);
  thing["pzem"]>>[](pson & out){
    out["pzem1_power"] = data.pzem1_power;
    out["pzem1_energy"] = data.pzem1_energy;
    out["pzem1_voltage"] = data.pzem1_voltage;
    out["pzem1_current"] = data.pzem1_current;
    out["pzem2_power"] = data.pzem2_power;
    out["pzem2_energy"] = data.pzem2_energy;
    out["pzem2_voltage"] = data.pzem2_voltage;
    out["pzem2_current"] = data.pzem2_current;
    out["deltaP"] = deltaP; 
    out["pv_1"] = input_R1;
    out["pv_2"] = input_R2;
    out["pv_3"] = input_R3;
    out["out_1"] = EEPROM.readFloat(3);
    out["out_2"] = EEPROM.readFloat(2);
    out["out_3"] = EEPROM.readFloat(1);
  };
}

void loop() {
  thing.handle();
tn1=millis();
if(tn1-tb1>10000){
  if (radio.available()){
    radio.read(&data, sizeof(data));
    //========= READING MESSAGE =========
    //========= PZEM 1 =========
    //POWER
    Serial.print(",");
    //Serial.print("Power_1: "); 
    //data.pzem1_power = power_1; 
    Serial.print(data.pzem1_power);
    
    //ENERGY
    Serial.print(",");
    //Serial.print("Energy_1: "); 
    //data.pzem1_energy = energy_1;
    Serial.print(data.pzem1_energy);
    
    //VOLTAGE
    Serial.print(",");
    //Serial.print("Voltage_1: "); 
    //data.pzem1_voltage = voltage_1;
    Serial.print(data.pzem1_voltage);
    
    //CURRENT
    Serial.print(",");
    //Serial.print("Current_1: "); 
    //data.pzem1_current = current_1;
    Serial.print(data.pzem1_current);


    //========= PZEM 2 =========
    //Serial.print("=========================================="); Serial.println();
    //POWER
    Serial.print(",");
    //Serial.print("Power_2: "); 
    //data.pzem2_power = power_2; 
    Serial.print(data.pzem2_power);
    
    //ENERGY
    Serial.print(",");
    //Serial.print("Energy_2: "); 
    //data.pzem2_energy = energy_2;
    Serial.print(data.pzem2_energy);

    //VOLTAGE
    Serial.print(",");
    //Serial.print("Voltage_2: "); 
    //data.pzem2_voltage = voltage_2;
    Serial.print(data.pzem2_voltage);

    //CURRENT
    Serial.print(",");
    //Serial.print("Current_2: "); 
    //data.pzem2_current = current_2;
    Serial.print(data.pzem2_current);

    //DELTA POWER (SUPERVISED SYSTEM)
    deltaP = data.pzem1_power - data.pzem2_power;
    //Serial.print("Delta Power: "); 
    Serial.print(",");
    Serial.print(deltaP);

    //divP = data.pzem2_power/3;
    //Serial.print("Divided: "); Serial.println(divP);
    
    //Serial.print("=========================================="); Serial.println();

//================  SISTEM SUPERVISI  =======================
//power_1 = beban (yg harus selalu lebih besar)
//power_2 = PV (yg harus selalu lebih kecil)
//NC = TERHUBUNG; NO = TERPUTUS
//------- MEMATIKAN RELAY ---------
//Mematikan 1
if (input_R1==1 && input_R2==1 && input_R3==1 && deltaP < 0){
digitalWrite(relay_1, LOW);//RELAY BESAR = NO: PV PUTUS (RELAY KECIL = 0)
digitalWrite(relay_2, HIGH);//RELAY BESAR = NC: PV NYAMBUNG (RELAY KECIL = 1)
digitalWrite(relay_3, HIGH);//RELAY BESAR = NC: bPV NYAMBUNG (RELAY KECIL = 1)
//Serial.println("fase 1: relay KECIL 1 NC & PV 1 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//SAVE DATA P1-P3 BEFORE PV CUT OFF
pv_3 = data.pzem2_power;
EEPROM.writeFloat(1,pv_3);
EEPROM.commit();
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
} 


//Mematikan 2
else if (input_R1==0 && input_R2==1 && input_R3==1 && deltaP < 0){
digitalWrite(relay_1, LOW);
digitalWrite(relay_2, LOW);
digitalWrite(relay_3, HIGH);
//Serial.println("fase 2: relay KECIL 1,2 NC & PV 1,2 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//SAVE DATA P1-P3 BEFORE PV CUT OFF
pv_2 = data.pzem2_power;
EEPROM.writeFloat(2,pv_2);
EEPROM.commit();
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
}
//Mematikan 3
else if (input_R1==0 && input_R2==0 && input_R3==1 && deltaP < 0){
digitalWrite(relay_1, LOW);
digitalWrite(relay_2, LOW);
digitalWrite(relay_3, LOW);
//Serial.println("fase 3: relay KECIL 1,2,3 NC & PV 1,2,3 mati");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//SAVE DATA P1-P3 BEFORE PV CUT OFF
pv_1 = data.pzem2_power;
EEPROM.writeFloat(3,pv_1);
EEPROM.commit();
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);

}

//------- MENYALAKAN RELAY ---------
//Menyalakan 3
else if (input_R1==0 && input_R2==1 && input_R3==1 && deltaP > 0 && data.pzem2_power > EEPROM.readFloat(1)){
digitalWrite(relay_1, HIGH);//RELAY BESAR = NC: PV NYALA
digitalWrite(relay_2, HIGH);//RELAY BESAR = NC: PV NYALA
digitalWrite(relay_3, HIGH);//RELAY BESAR = NC: PV NYALA
//Serial.println("fase 4: relay KECIL 1,2,3 NO & PV 1,2,3 NYALA");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//READ MEMORY IN EEPROM
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
}
//Menyalakan 2
else if (input_R1==0 && input_R2==0 && input_R3==1 && deltaP > 0 && data.pzem2_power > EEPROM.readFloat(2)){
digitalWrite(relay_1, LOW);//RELAY BESAR = NC: PV NYAMBUNG (RELAY KECIL = 1)
digitalWrite(relay_2, HIGH);//RELAY BESAR = NC: PV NYAMBUNG (RELAY KECIL = 1)
digitalWrite(relay_3, HIGH);//RELAY BESAR = NO: PV PUTUS (RELAY KECIL = 0)
//Serial.println("fase 6: relay KECIL 1,2 NO & PV 1,2 NYALA");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//READ MEMORY IN EEPROM
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
}
//Menyalakan 1
else if (input_R1==0 && input_R2==0 && input_R3==0 && deltaP > 0 && data.pzem2_power > EEPROM.readFloat(3)){
digitalWrite(relay_1, LOW);//RELAY BESAR = NC: PV NYAMBUNG (RELAY KECIL = 1)
digitalWrite(relay_2, LOW);//RELAY BESAR = NO: PV PUTUS (RELAY KECIL = 0)
digitalWrite(relay_3, HIGH);////RELAY BESAR = NO: PV PUTUS (RELAY KECIL = 0)
//Serial.println("fase 6: relay KECIL 1 NO & PV 1 NYALA");
//baca relay
input_R1 = digitalRead (relayInput_1);
input_R2 = digitalRead (relayInput_2);
input_R3 = digitalRead (relayInput_3);
Serial.print(",");
Serial.print(input_R1);
Serial.print(",");
Serial.print(input_R2);
Serial.print(",");
Serial.print(input_R3);

//READ MEMORY IN EEPROM
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
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
Serial.print(input_R3);

//SAVE MEMORY IN EEPROM
pv_3 = data.pzem2_power;
EEPROM.writeFloat(1,pv_3);
//EEPROM.commit();
pv_2 = data.pzem2_power;
EEPROM.writeFloat(2,pv_2);
//EEPROM.commit();
pv_1 = data.pzem2_power;
EEPROM.writeFloat(3,pv_1);
EEPROM.commit();
//READ MEMORY IN EEPROM
out_pv_3 = EEPROM.readFloat(1);
out_pv_2 = EEPROM.readFloat(2);
out_pv_1 = EEPROM.readFloat(3);
Serial.print(",");
Serial.print(out_pv_3);
Serial.print(",");
Serial.print(out_pv_2);
Serial.print(",");
Serial.println(out_pv_1);
}

//     ======= LED SIGNAL DITERIMA =========
     digitalWrite(led, HIGH);
     delay(500);
     digitalWrite(led, LOW);
     delay(500);

  }
  else{
    Serial.println("Tidak terhubung...");
  }
  tb1=tn1;
}
}
