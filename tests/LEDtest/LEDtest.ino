
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9
#define TRANSR A0
#define TRANSL A1
#define LASERR 2
#define LASERL 3
#define THRESH 75
#define LEDR A3
#define LEDL A2

int originalR;
int originalL;

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
  originalR = analogRead(LEDR);//read transistors for baseline
  originalL = analogRead(LEDL);
}

//veers subsequentis left
void veerLeft() {
  digitalWrite(RIGHTDIR, HIGH);
  digitalWrite(LEFTDIR, LOW);
  analogWrite(RIGHTSPD, 150);
  analogWrite(LEFTSPD, 100);
}

//veers subsequentis right
void veerRight() {
  digitalWrite(RIGHTDIR, HIGH);
  digitalWrite(LEFTDIR, LOW);
  analogWrite(LEFTSPD, 150);
  analogWrite(RIGHTSPD, 100);
}




//basic go to cup function
void attackCup() {
  analogWrite(LEFTSPD, 0);
  analogWrite(RIGHTSPD, 0);
  int baseR = analogRead(TRANSR);//base reading
  int contR = analogRead(TRANSR);//reading that will be updated
  int baseL = analogRead(TRANSL);
  int contL = analogRead(TRANSL);

  //loop continues while cup is still detected by both modules
   while (contR >= (baseR - THRESH) && contL >= (baseL - THRESH)) {
    digitalWrite(LEFTDIR, LOW);
    analogWrite(LEFTSPD, 100);
    analogWrite(RIGHTSPD, 100);
    delay(500);//should be removed
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }

  //cup is only detected by right, so veer toward it and read
  while (!(contL >= (baseL - THRESH)) && contR >= (baseR - THRESH)) {
    veerRight();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }
  //cup is only detected by left, veer left, read
  while(!(contR >= (baseR - THRESH)) && contL >= (baseL - THRESH)) {
    veerLeft();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }

  return;
}




void loop() {

	digitalWrite(LEFTDIR, LOW);
	digitalWrite(RIGHTDIR,HIGH);


	analogWrite(LEFTSPD, 100);
	analogWrite(RIGHTSPD, 100);
	//turn counterclockwise
	int nextR = analogRead(LEDR);//read transistors again
	int nextL = analogRead(LEDL);

	if (nextR < (originalR - THRESH) || nextL < (originalL - THRESH)) {
  		digitalWrite(LEFTDIR, HIGH);
  		digitalWrite(RIGHTDIR, LOW);
  		delay (2000);
	}






}
