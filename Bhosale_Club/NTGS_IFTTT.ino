#include <MCP3202.h>
#include <WiFi.h>

WiFiClient client;

// Replace with your SSID and Password
//const char* ssid     = "TGS_Network";
//const char* password = "AmoghSadhu";
const char* ssid     = "moto one f";
const char* password = "sanchit@98";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/gas_readings/with/key/gKrofWrzw1-JzHoLdIEQhqU4WxoEvgDFQVSBjrSj3OU";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

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
#define TGS_Ro 26.30 //64.88 //Enter found Ro value
#define TGS_sensor 33 //A0 //Sensor is connected to A4

void setup() {
Serial.begin(115200);

adc.begin();
  //setup wifi
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    
Serial.println("SETUP DONE");
}

void loop() {
//...........................................................MQ-137..................................................................................
  float MQ_VRL; //Voltage drop across the MQ sensor
  float MQ_Rs; //Sensor resistance at gas concentration 
  float MQ_ratio; //Define variable for ratio
  float x,y = 0;
  Serial.println("Reading channels");
  x= adc.readChannel(0);
  //y = adc.readChannel(1);
  //x = analogRead(TGS_sensor);
  Serial.print("TGS: "); 
  Serial.println(x);
  
//..........................................................TGS-2602..................................................................................
  float TGS_VRL; //Voltage drop across the MQ sensor
  float TGS_Rs; //Sensor resistance at gas concentration 
  float TGS_ratio; //Define variable for ratio
   
  TGS_VRL = x*(5.0/4095.0); //Measure the voltage drop and convert to 0-5V
  TGS_Rs = ((5.0*TGS_RL)/TGS_VRL)-TGS_RL; //Use formula to get Rs value
  TGS_ratio = TGS_Rs/TGS_Ro;  // find ratio Rs/Ro
 
  float TGS_ppm = pow(10, ((log10(TGS_ratio)-TGS_b)/TGS_m)); //use formula to calculate ppm
  Serial.print("TGS_PPM:");
  Serial.println(TGS_ppm, 4);
  Serial.println();

  //delay(2000);

  //...................................IFTTT Service...................................................  
// Make an HTTP request to the IFTTT web service
 
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  //NH3 and H2S readings in ppm
  String jsonObject = String("{\"value1\":\"") + TGS_ppm + "\"}";
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  delay(300000);

  if(!!!client.available()) {
    Serial.println("No response...");
  }
  
  //Serial.println("\nclosing connection");
  //client.stop(); 

}
