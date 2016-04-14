
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9
#define TRANS A0


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

int original = analogRead(TRANS);//read transistor for baseline
digitalWrite(LEFTDIR, HIGH);
analogWrite(LEFTSPD, 50);
analogWrite(RIGHTSPD, 50);
delay(500);//turn counterclockwise for .5 sec
analogWrite(LEFTSPD, LOW);
analogWrite(RIGHTSPD, LOW);
delay(500);
int next = analogRead(TRANS);//read transistor again

//check to see if new reading is significantly higher
//if so, move toward cup
if (next > (original + 50)) {
  digitalWrite(LEFTDIR, LOW);
  analogWrite(LEFTSPD, 100);
  analogWrite(RIGHTSPD, 100);
  delay(500);
}

}
