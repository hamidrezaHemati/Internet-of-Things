#include "ESP8266WiFi.h"

void setup(){
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  scanAvailableNetworks();
  connectToNetwork("HAMIDREZA", "22558930h");
}

void loop(){
  
}

void connectToNetwork(String net_name, String net_pass){
  delay(1000);
  WiFi.begin(net_name, net_pass);
  Serial.print(String("Connecting to " + net_name ));
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address");
  Serial.println(WiFi.localIP());
}

void scanAvailableNetworks(){
  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  Serial.println();

  delay(5000);
}
