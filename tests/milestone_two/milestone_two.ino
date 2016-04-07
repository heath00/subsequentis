//pin assignments
#define BUTTONONE 9
#define BUTTONTWO 10
#define LEDONE 8
#define LEDTWO 6
#define POT A0

//other constants
#define TIMEOUT 60000

//time variable
unsigned long startTime;

void setup() {
  pinMode(BUTTONONE,INPUT);
  pinMode(BUTTONTWO,INPUT);
  pinMode(LEDONE,OUTPUT);
  pinMode(LEDTWO,OUTPUT);
  startTime=millis();
}


void loop() {
  while (millis()-startTime < TIMEOUT){
    analogWrite(LEDONE,191); //191 ~= 75% duty cycle
    analogWrite(LEDTWO,191);
    while (analogRead(POT) > 930 && (millis()-startTime < TIMEOUT)){ //930 ~= 3.00V
      digitalWrite(LEDONE,HIGH);
      digitalWrite(LEDTWO,LOW);
      delay(50);
      digitalWrite(LEDONE,LOW);
      digitalWrite(LEDTWO,HIGH);
      delay(50);
      }
      
   if (!digitalRead(BUTTONONE)){
    analogWrite(LEDONE,64); //64 ~= 25% duty cycle
    analogWrite(LEDTWO,64);
    delay(1000);
    digitalWrite(LEDONE,HIGH);
    digitalWrite(LEDTWO,LOW);                                             
    delay(1000);
   }

   else if (!digitalRead(BUTTONTWO)){
    analogWrite(LEDONE,64); //64 ~= 25% duty cycle
    analogWrite(LEDTWO,64);
    delay(1000);
    digitalWrite(LEDONE,LOW);
    digitalWrite(LEDTWO,HIGH);
    delay(1000);
   }
  }
  digitalWrite(LEDONE,LOW);
  digitalWrite(LEDTWO,LOW);
}
