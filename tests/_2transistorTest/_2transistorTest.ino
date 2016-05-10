#define TRANSL A1
#define TRANSR A0


void setup() {
    pinMode(TRANSL, INPUT);
    pinMode(TRANSR, INPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    SerialUSB.begin(9600);
}

void loop() {
  int left = analogRead(TRANSL);
  int right = analogRead(TRANSR);
  SerialUSB.println(left);
  SerialUSB.println(right);
  
  
  // put your main code here, to run repeatedly:

}
