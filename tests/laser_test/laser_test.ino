#define TRANS A0
#define LASER 2

void setup() {
  SerialUSB.begin(9600);
  pinMode(LASER,OUTPUT);
  digitalWrite(LASER,HIGH);

}

void loop() {
  int val = analogRead(TRANS);//read transistor again
  SerialUSB.println(val);
  delay(10);

}
