#include <ArduinoJson.h>

//Gas Sensor Pins
//#define F2600 A1
//#define MQ135 A1
//#define F2602 A0

const int F2602 = 33;

//Gas Sensor Load Resistance (RL)
#define RL_MQ135 2
#define RL_F2602 22

/*2602 GASSES*/
float NH3_A = 0.92372844;    
float NH3_B = -0.291578925;

float H2S_A = 0.38881036;
float H2S_B = -0.35010059;

float VOC_A = 0.3220722;
float VOC_B = -0.6007520;

void setup() 
{
Serial.begin(115200); // opens serial port, sets data rate 9600 bps
}

void loop() 
{
  //Refer to the documentation mentioned on this page to understand this better

  float VRL_F2602; 
  float Rs_F2602; 
  float Ro_F2602 = 64.88;
  float ratio_F2602;

  Serial.println(analogRead(F2602));
  VRL_F2602 = analogRead(F2602)*(5.0/4095.0); 
  Rs_F2602 = ((5.0/VRL_F2602)-1)*(RL_F2602); 
  ratio_F2602 = Rs_F2602/Ro_F2602;

  float ppm_VOC = VOC_A * pow(ratio_F2602, VOC_B);
  float ppm_H2S = H2S_A * pow(ratio_F2602, H2S_B);  
  float ppm_NH3 = NH3_A * pow(ratio_F2602, NH3_B);
  
 //Refer to the author's website: https://arduinojson.org/ and read the documentation to understand ArduinoJSON better
  DynamicJsonBuffer jBuffer; 
  JsonObject& root = jBuffer.createObject();

  root["NH3"] = ppm_NH3;
  root["H2S"] = ppm_H2S;

  
  root.prettyPrintTo(Serial);
  Serial.println("");

//Minimum delay required for ThingSpeak to update is 16 seconds 
delay(5000);
}
