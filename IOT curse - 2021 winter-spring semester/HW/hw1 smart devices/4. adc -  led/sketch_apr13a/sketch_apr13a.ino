int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int buzzer = 4;
void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue, 0, 1023, 0, 100);
  Serial.println(sensorValue);
  if(sensorValue > 50){
    digitalWrite(LED_BUILTIN, LOW);
    tone(buzzer, 1000, 500);
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    
  }
//  // turn the ledPin on
  
}
