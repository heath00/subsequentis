
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9
#define TRANS A0
#define LASER 2


//initialize pins
void setup() {
  pinMode(RIGHTSPD,OUTPUT);
  pinMode(RIGHTDIR,OUTPUT);
  digitalWrite(RIGHTDIR,HIGH);
  pinMode(LEFTSPD, OUTPUT);
  pinMode(LEFTDIR, OUTPUT);
  digitalWrite(LEFTDIR, LOW);
  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, HIGH);
}

//basic go to cup function
void attackCup() {
   int base = analogRead(TRANS);//base reading
   int cont = analogRead(TRANS);//reading that will be updated

  //loop continues while cup is still detected
   while (cont >= (base - 50)) {
    digitalWrite(LEFTDIR, LOW);
    analogWrite(LEFTSPD, 100);
    analogWrite(RIGHTSPD, 100);
    delay(500);//may be able to play with delay length
    cont = analogRead(TRANS);
  }
}

void loop() {

int original = analogRead(TRANS);//read transistor for baseline
digitalWrite(LEFTDIR, HIGH);
analogWrite(LEFTSPD, 50);
analogWrite(RIGHTSPD, 50);
//turn counterclockwise
int next = analogRead(TRANS);//read transistor again

//check to see if new reading is significantly higher
//if so, move toward cup
if (next > (original + 50)) {
  attackCup();
  }

}
