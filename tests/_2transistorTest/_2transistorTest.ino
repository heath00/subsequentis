#define TRANSL A1
#define TRANSR A0

void setup() {
    pinMode(TRANSL, INPUT);
    pinMode(TRANSR, INPUT);
    SerialUSB.begin(9600);
}

void loop() {
  int left = analogRead(TRANSL);
  int right = analogRead(TRANSR);
  SerialUSB.println(left + "                " + right);
  
  
  // put your main code here, to run repeatedly:

}
