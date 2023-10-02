int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int brightness = 0;
int pwmPin = 15;
void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(16, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  brightness = map(sensorValue, 0, 1023, 0, 100);
  Serial.println(brightness);
  analogWrite(pwmPin,  brightness);
  delay(50);
//  // turn the ledPin on
  
}
