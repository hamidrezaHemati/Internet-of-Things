#include <string.h>
#include <stdlib.h>

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include "coap_client.h"

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const char* ssid = "HAMIDREZA";
const char* password = "22558930h";

IPAddress ip(192,168,1,7);
int port =5683;

int water_level_led = D1;
int led_blink = D0;
int tempreture_level_led = D2;

WiFiClient espClient;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

coapClient coap;

int user_desired_water_level = 0, water_level, percentage;
int water_level_sensor = A0;

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

void connect_to_wifi(String net_name, String net_pass){
  delay(100);
  WiFi.begin(net_name, net_pass);
  Serial.print(String("Connecting to " + net_name ));
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback_response(coapPacket &packet, IPAddress ip, int port) {
    if (packet.payloadlen != 0) { ///if get response is not empty
      Serial.println("callback");
      char payload_str[packet.payloadlen + 1];
      memcpy(payload_str, packet.payload, packet.payloadlen);
      payload_str[packet.payloadlen] = NULL;
        
      char * token = strtok(payload_str, " ");
      int i = 0;
      int desired_water_temperature;
      while( token != NULL ) {
          if (i == 0)
            user_desired_water_level = atoi(token);
          if (i == 1)
            desired_water_temperature = atoi(token);
          token = strtok(NULL, " ");
          i++;
      }

      Serial.print("user desired water level: ");
      Serial.println( user_desired_water_level);
      Serial.print("user desired water temprutur: ");
      Serial.println(desired_water_temperature);
      
      int led_tempreture_value = map(desired_water_temperature, 1, 100, 1, 1023);
      analogWrite(tempreture_level_led, led_tempreture_value);
    }
}

void display_water_level() {
  analogWrite(water_level_led, percentage);
  if (percentage < user_desired_water_level + 5) {
    digitalWrite(led_blink, HIGH);
    delay(50);
    digitalWrite(led_blink, LOW);
  }
}

void setup() {
  // LED and water sensor
  pinMode(water_level_led, OUTPUT);
  pinMode(led_blink, OUTPUT);
  pinMode(tempreture_level_led, OUTPUT);
  pinMode(water_level_sensor, INPUT);

  //RFID
  SPI.begin();
  rfid.PCD_Init();
  
  Serial.begin(115200);

  //WiFi and coAp
  connect_to_wifi(ssid, password);
  
  coap.response(callback_response);
  coap.start();
}

void loop() {

  coap.loop();

  if (rfid.PICC_IsNewCardPresent()) {
    int offset = 0;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < rfid.uid.size; i++) {
        offset += snprintf(msg + offset, MSG_BUFFER_SIZE - offset, ":%x\0", rfid.uid.uidByte[i]);
      }
      Serial.println();
      
      Serial.print("tagID: ");
      Serial.println(msg);
      coap.put(ip, port, "tagID", msg, strlen(msg));     
      
      rfid.PICC_HaltA();
    }
  }

  water_level = analogRead(water_level_sensor);
  percentage = map(water_level, 0, 600, 0, 100);  
  Serial.print("water level PERCENTAGE: ");
  Serial.println(percentage);
  display_water_level();
  
  coap.get(ip,port,"bath");
  
  delay(200);
}
