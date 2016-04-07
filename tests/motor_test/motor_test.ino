
/* afafafaf*/
void setup() {
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
}

void loop() {
  analogWrite(6,255);
  analogWrite(8,255);
  delay(1000);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  delay(1000);
  digitalWrite(9,HIGH); 
  analogWrite(6,100);
  analogWrite(8,100);
  delay(500);
  analogWrite(6, LOW);
  analogWrite(8, LOW);
  delay(5000);
}
