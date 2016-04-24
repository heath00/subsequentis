
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9
#define TRANSR A0
#define TRANSL A1
#define LASERR 2
#define LASERL 3


//initialize pins
void setup() {
  pinMode(RIGHTSPD,OUTPUT);
  pinMode(RIGHTDIR,OUTPUT);
  digitalWrite(RIGHTDIR,HIGH);
  pinMode(LEFTSPD, OUTPUT);
  pinMode(LEFTDIR, OUTPUT);
  digitalWrite(LEFTDIR, LOW);
  pinMode(LASERR, OUTPUT);
  digitalWrite(LASERR, HIGH);
  pinMode(LASERL, OUTPUT);
  digitalWrite(LASERL, HIGH);
}

//veers subsequentis left
void veerLeft() {
  analogWrite(RIGHTSPD, 150);
  analogWrite(LEFTSPD, 100);
}

//veers subsequentis right
void veerRight() {
  analogWrite(LEFTSPD, 150);
  analogWrite(RIGHTSPD, 100);
}




//basic go to cup function
void attackCup() {
   int baseR = analogRead(TRANSR);//base reading
   int contR = analogRead(TRANSR);//reading that will be updated
   int baseL = analogRead(TRANSL);
   int contL = analogRead(TRANSL);

  //loop continues while cup is still detected by both modules
   while (contR >= (baseR - 50) && contL >= (baseL - 50)) {
    digitalWrite(LEFTDIR, LOW);
    analogWrite(LEFTSPD, 100);
    analogWrite(RIGHTSPD, 100);
    delay(500);//should be removed
    contR = analogRead(TRANSR);
    contL = analogRead(TRANSL);
  }

  //cup is only detected by right, so veer toward it and read
  if (!(contL >= (baseL - 50))) {
    while (!(contL >= (baseL - 50))) {
      veerRight();
      contL = analogRead(TRANSL);
    }
  }
  //cup is only detected by left, veer left, read
  else if (!(contR >= (baseR - 50))) {
    while(!(contR >= (baseL - 50))) {
      veerLeft();
      contR = analogRead(TRANSR);
    }
  }
}



void loop() {

int originalR = analogRead(TRANSR);//read transistors for baseline
int originalL = analogRead(TRANSL);
digitalWrite(LEFTDIR, HIGH);
analogWrite(LEFTSPD, 50);
analogWrite(RIGHTSPD, 50);
//turn counterclockwise
int nextR = analogRead(TRANSR);//read transistors again
int nextL = analogRead(TRANSL);

if (nextR > (originalR + 50) && !(nextL > (originalL + 50))) {
  //cup is detected by only the right module
  digitalWrite(LEFTDIR, LOW);
  digitalWrite(RIGHTDIR, LOW);//turn clockwise
  int baseL = analogRead(TRANSL);
  int contL = analogRead(TRANSL);
  //turn until cup is detected by left module too
  while (!(contL > (baseL + 50))) {
    analogWrite(LEFTSPD, 50);
    analogWrite(RIGHTSPD, 50);
    contL = analogRead(TRANSL);
  }
  attackCup();
  }
else if (nextL > (originalL + 50) && !(nextR > (originalR + 50))) {
  //cup is detected by only left module
  digitalWrite(LEFTDIR, HIGH);
  digitalWrite(RIGHTDIR, HIGH);//turn counterclockwise
  int baseR = analogRead(TRANSR);
  int contR = analogRead(TRANSR);
  //turn until cup is detected by right module too
  while (!(contR > (baseR + 50))) {
    analogWrite(LEFTSPD, 100);
    analogWrite(RIGHTSPD, 100);
    contR = analogRead(TRANSR);
  }
    attackCup();
}
else if (nextL > (originalL + 50) && nextR > (originalR + 50)) {
  //cup is detected by both modules
  attackCup();
}





}
