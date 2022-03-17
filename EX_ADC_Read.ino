#include <MCP3202.h>

//D13 - CK
//D12 - DOUT
//D11 - DIN
//D10 - CS

// You can also refer to this website!
// Thanks to baturman for making this schematic.
// https://easyeda.com/baturman/arduino-uno-r3-mcp3202-wiring-diagram

/**
 *  This code shall return the voltage value in terms of a decimal value.
 *  4096 means the voltage is equal to Vdd, while 0 means voltage is Vss.
 */

double VRL_F2602;

int Vdd = 5; //change according to your Vdd value.

MCP3202 adc = MCP3202(10); //Parameter passed is the CS pin number. Change according to requirement.

float channel1 = 0;
float channel2 = 0;
void setup() {
  // put your setup code here, to run once:
  adc.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  double tgs2602_value = adc.readChannel(1);
 
  VRL_F2602 = tgs2602_value*(5/4096.0); //4095
  //delay(500);
  Serial.print("Tgs2602_value= ");
  Serial.println(tgs2602_value);
  Serial.print("Voltage:");
  Serial.println(VRL_F2602);
  //Serial.println();
  delay(500);

}
