#define RIGHTLED A3
#define LEFTLED A2

void setup() {
  // put your setup code here, to run once:
  pinMode(RIGHTLED, INPUT);
  pinMode(LEFTLED, INPUT);
  SerialUSB.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A3);
  int y = analogRead(A2);
  SerialUSB.println(x);
  SerialUSB.println(y);

}
