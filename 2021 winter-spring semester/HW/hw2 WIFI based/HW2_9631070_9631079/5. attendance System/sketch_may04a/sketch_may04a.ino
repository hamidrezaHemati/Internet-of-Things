#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance.


#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char *ssid     = "HAMIDREZA";
const char *password = "22558930h";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int led_pin = D1;
int buzzer_pin = D8;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  
  // Connect to Wi-Fi
  connectToNetwork("HAMIDREZA", "22558930h");

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(16200);

  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);

  digitalWrite(led_pin, LOW);
  tone(buzzer_pin, 0);
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

int ID_authentication(String uid){
  if (uid.substring(1) == "1A 3A 94 80"){
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
    return 1;
  }
  Serial.println(" Access denied");
  delay(3000);
  return 0;
}

int time_authentication(int hour, int minute, int second){
  if(hour <16 && hour >= 15 && minute <=30 && minute >= 1){
    return 1;
  }
  return 0;
}

void buzzer_led(int _state){
  if(_state){
    digitalWrite(led_pin, HIGH);
  }
  else{
    tone(buzzer_pin , 800);
  }
  delay(3000);
  digitalWrite(led_pin, LOW);
  tone(buzzer_pin , 0);
}

void loop() {
  
  timeClient.update();
  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);   
  
   if (rfid.PICC_IsNewCardPresent()) {
     if (rfid.PICC_ReadCardSerial()){
        Serial.print("UID tag :");
        byte letter;
        for (byte i = 0; i < rfid.uid.size; i++) 
        {
           Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
           Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
        Serial.print("Message : ");
        buzzer_led(time_authentication(currentHour, currentMinute, currentSecond));
     }
  }


  Serial.println();
  
 
}
