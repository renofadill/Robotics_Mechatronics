#include <ThingerESP8266.h>
#include <PZEM004T.h>
#include <ESP8266WiFi.h>

PZEM004T pzem(12, 13); //12 = D6 (RX=ESP8266), 13 = D7 (TX=ESP8266)
IPAddress ip(192,168,1,1);

unsigned long tb1 = 0;
unsigned long tn1;

//Thinger.io configuration
#define USERNAME "renofadill"
#define DEVICE_ID "ESP8266_SS_v01"
#define DEVICE_CREDENTIAL "oZZ7F4V8nu815Mg6"
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//WIFI
const char* ssid = "04F2G_135cb0_plus";
const char* password = "wlaneca34f";
//PZEM INITIAL
float power_1, energy_1, voltage_1, current_1;

void setup() {
 Serial.begin(9600);
 pzem.setAddress(ip);
 Serial.print("RESETTIMER");
 Serial.println();
 Serial.println(",Power_1, Energy_1, Current_1, Voltage_1");
 WiFi.begin(ssid, password);
  thing.add_wifi(ssid, password);
  thing["pzemPLN"]>>[](pson & out){
    out["pzem1_power"] = power_1;
    out["pzem1_energy"] = energy_1;
    out["pzem1_voltage"] = voltage_1;
    out["pzem1_current"] = current_1;
};
}

void loop() {
  thing.handle();
  tn1=millis();
  if(tn1-tb1>10000){
    //POWER
    Serial.print(",");
    power_1 = pzem.power(ip);
    Serial.print(power_1);

    //ENERGY
    Serial.print(",");
    energy_1 = pzem.energy(ip);
    Serial.print(energy_1);

    //VOLTAGE
    Serial.print(",");
    voltage_1 = pzem.voltage(ip);
    Serial.print(voltage_1);

    //CURRENT
    Serial.print(",");
    current_1 = pzem.current(ip);
    Serial.println(current_1);

    tb1=tn1;
  }
}
