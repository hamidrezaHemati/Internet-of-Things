#include <string.h>
#include <stdlib.h>

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const char* ssid = "HAMIDREZA";
const char* password = "22558930h";
const char* mqtt_server = "192.168.1.7";

int water_level_led = D1;
int led_blink = D0;
int tempreture_level_led = D2;


WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
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
  WiFi.mode(WIFI_STA);
  WiFi.begin(net_name, net_pass);
  Serial.print(String("Connecting to " + net_name ));
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  // EXTRACT the information
  char payload_str[length];  //water level and water tempreture
  memcpy(payload_str, payload, length);  ///byte to string
  char * token = strtok(payload_str, " ");  ///tokenize
  int i = 0;
  int desired_water_level, desired_water_temperature;
  while( token != NULL ) {
      if (i == 0)
        desired_water_level = atoi(token);
      if (i == 1)
        desired_water_temperature = atoi(token);
      token = strtok(NULL, " ");
      i++;
  }

  user_desired_water_level = desired_water_level*20;

  Serial.print("user desired water level: ");
  Serial.println( user_desired_water_level);
  Serial.print("user desired water temprutur: ");
  Serial.println(desired_water_temperature);
  
  // Handle water temperature
  int led_tempreture_value = map(desired_water_temperature, 1, 100, 1, 1023);
  analogWrite(tempreture_level_led, led_tempreture_value);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.subscribe("house/bath");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      // Wait 3 seconds before retrying
      delay(3000);
    }
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
  // Initialize serial 
  Serial.begin(115200);

  //WiFi and MQTT
  connect_to_wifi(ssid, password);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (rfid.PICC_IsNewCardPresent()) {
    int offset = 0;
    if (rfid.PICC_ReadCardSerial()) {
//      Serial.print("Tag UID:");
      for (byte i = 0; i < rfid.uid.size; i++) {
//        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
//        Serial.print(rfid.uid.uidByte[i], HEX);
        offset += snprintf(msg + offset, MSG_BUFFER_SIZE - offset, ":%x\0", rfid.uid.uidByte[i]);
      }
      Serial.println();
      
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("house/hardwareBath", msg);      
      
      rfid.PICC_HaltA();
    }
  }

  water_level = analogRead(water_level_sensor);
  percentage = map(water_level, 0, 600, 0, 100);  
  Serial.print("water level PERCENTAGE: ");
  Serial.println(percentage);
  display_water_level();
  delay(200);
}
