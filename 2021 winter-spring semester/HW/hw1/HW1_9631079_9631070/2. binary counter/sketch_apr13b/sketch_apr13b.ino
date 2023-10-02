const int led_white = 5;
const int led_blue = 4;
const int led_green = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_white, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);
  Serial.begin(9600);
}

int b0 = 0, b1 = 0, b2 = 0;
const int basis = 2;
int _bit[3];
int _position = 0;
void binary_encoder(int num){
  Serial.print("num: ");
  Serial.print(num);
   _position = basis;
   while(_position >= 0){
      _bit[_position] = num%basis;
      num = num / basis;
      _position--;
   }
   Serial.print(" --> ");
   Serial.print( _bit[0]);
   Serial.print( _bit[1]);
   Serial.print( _bit[2]);
   Serial.println();
   digitalWrite(led_white, _bit[2]);
   digitalWrite(led_blue, _bit[1]);
   digitalWrite(led_green, _bit[0]);
   delay(2000);
}

int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  binary_encoder(counter);
  counter++;
  if(counter == 8)
    counter = 0;

}
