#include <WiFi.h>

WiFiClient client;

const int F2602 = 33; //define pin number for gas sensor

//Gas Sensor Load Resistance (RL)
#define RL_F2602 22

/*2602 GASSES*/
float NH3_A = 0.92372844;    
float NH3_B = -0.291578925;

float H2S_A = 0.38881036;
float H2S_B = -0.35010059;

float VOC_A = 0.3220722;
float VOC_B = -0.6007520;

// Replace with your SSID and Password
const char* ssid     = "TGS_Network";
const char* password = "AmoghSadhu";
//const char* ssid     = "CAPS";
//const char* password = "caps@2202";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/gas_readings/with/key/nMIop-xe4BQ8qpFciFX9iKkf9EcoDiSPRb8ywxuYfgS";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

void setup() {

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

}

void loop() {
  //Reading NH3 and H2S values in ppm

  float VRL_F2602; 
  float Rs_F2602; 
  float Ro_F2602 = 64.88;
  float ratio_F2602;

  //Serial.println(analogRead(F2602));
  VRL_F2602 = analogRead(F2602)*(3.3/4095.0); //4095
  Rs_F2602 = ((5.0/VRL_F2602)-1)*(RL_F2602); 
  ratio_F2602 = Rs_F2602/Ro_F2602;

  float ppm_H2S = H2S_A * pow(ratio_F2602, H2S_B);  
  float ppm_NH3 = NH3_A * pow(ratio_F2602, NH3_B);
  
  Serial.println(ppm_NH3);
  Serial.println(ppm_H2S);

  

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
  String jsonObject = String("{\"value1\":\"") + ppm_NH3 + "\",\"value2\":\"" + ppm_H2S + "\"}";
                      
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
