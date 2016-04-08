
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9


//initialize pins
void setup() {
  pinMode(RIGHTSPD,OUTPUT);
  pinMode(RIGHTDIR,OUTPUT);
  digitalWrite(RIGHTDIR,HIGH);
  pinMode(LEFTSPD, OUTPUT);
  pinMode(LEFTDIR, OUTPUT);
  digitalWrite(LEFTDIR, LOW);
}

void loop() {
  digitalWrite(LEFTDIR, LOW);
  analogWrite(RIGHTSPD,255);
  analogWrite(LEFTSPD,255);
  delay(1000); //  full speed forward for one second
  digitalWrite(RIGHTSPD,LOW);
  digitalWrite(LEFTSPD,LOW);
  delay(1000); // pause for one second
  digitalWrite(LEFTDIR,HIGH); 
  analogWrite(RIGHTSPD,100);
  analogWrite(LEFTSPD,100);
  delay(500); // counterclockwise for 500ms
  analogWrite(RIGHTSPD, LOW);
  analogWrite(LEFTSPD, LOW);
  delay(5000); // pause for 5s
}
