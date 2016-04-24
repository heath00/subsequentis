void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  SerialUSB.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A0);
  SerialUSB.println(x);
  delay(10);
}
