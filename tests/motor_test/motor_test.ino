
/* afafafaf*/
void setup() {
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);

}

void loop() {
  analogWrite(6,123);
  delay(1000);
  digitalWrite(6,LOW);
  delay(10000);

}
