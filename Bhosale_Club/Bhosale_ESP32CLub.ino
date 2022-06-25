/*
 * Program to measure gas in ppm using MQ sensor
 * Program by: B.Aswinth Raj
 * Website: www.circuitdigest.com
 * Dated: 28-12-2017
 */
#include <MCP3202.h>

float channel1 =0;
float channel2 = 0; 

MCP3202 adc = MCP3202(5); //Parameter passed is the CS pin number. Change according to requirement.

#define MQ_RL 47  //The value of resistor RL is 47K
#define MQ_m -0.263 //Enter calculated Slope 
#define MQ_b 0.42 //Enter calculated intercept
#define MQ_Ro 35.48 //Enter found Ro value
#define MQ_sensor 32  //A1 //Sensor is connected to A4

#define TGS_RL 27  //The value of resistor RL is 47K
#define TGS_m -0.204 //0.92372844Enter calculated Slope 
#define TGS_b -0.079 //-0.291578925Enter calculated intercept
#define TGS_Ro 64.88 //Enter found Ro value
#define TGS_sensor 33 //A0 //Sensor is connected to A4

void setup() {
Serial.begin(9600);
}

void loop() {
//...........................................................MQ-137..................................................................................
  float MQ_VRL; //Voltage drop across the MQ sensor
  float MQ_Rs; //Sensor resistance at gas concentration 
  float MQ_ratio; //Define variable for ratio
  float x,y = 0;
  //x= adc.readChannel(0);
  //y = adc.readChannel(1);
  x = analogRead(TGS_sensor);
  Serial.println(x);
  //Serial.println(y); 
  MQ_VRL = analogRead(MQ_sensor)*(5.0/4095.0); //Measure the voltage drop and convert to 0-5V
  MQ_Rs = ((5.0*MQ_RL)/MQ_VRL)-MQ_RL; //Use formula to get Rs value
  MQ_ratio = MQ_Rs/MQ_Ro;  // find ratio Rs/Ro
 
  float MQ_ppm = pow(10, ((log10(MQ_ratio)-MQ_b)/MQ_m)); //use formula to calculate ppm
  Serial.print("MQ137_PPM:");
  Serial.println(MQ_ppm);
  //delay(2000);
//..........................................................TGS-2602..................................................................................
  float TGS_VRL; //Voltage drop across the MQ sensor
  float TGS_Rs; //Sensor resistance at gas concentration 
  float TGS_ratio; //Define variable for ratio
   
  TGS_VRL = analogRead(TGS_sensor)*(5.0/4095.0); //Measure the voltage drop and convert to 0-5V
  TGS_Rs = ((5.0*TGS_RL)/TGS_VRL)-TGS_RL; //Use formula to get Rs value
  TGS_ratio = TGS_Rs/TGS_Ro;  // find ratio Rs/Ro
 
  float TGS_ppm = pow(10, ((log10(TGS_ratio)-TGS_b)/TGS_m)); //use formula to calculate ppm
  Serial.print("TGS_PPM:");
  Serial.println(TGS_ppm);
  Serial.println();

  delay(2000);
}
