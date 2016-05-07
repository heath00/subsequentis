
//define pins
#define RIGHTSPD 6
#define RIGHTDIR 7
#define LEFTSPD 8
#define LEFTDIR 9
#define TRANSR A0
#define TRANSL A1
#define LEDR A3
#define LEDL A2
#define LASERR 2
#define LASERL 3
#define THRESHLASER 20
#define THRESHLED 75

int fallSafeR;
int fallSafeL;//variables for staying on table


//initialize pins
void setup() {
  pinMode(RIGHTSPD,OUTPUT);
  pinMode(RIGHTDIR,OUTPUT);
  digitalWrite(RIGHTDIR,HIGH);//right motor setup
  pinMode(LEFTSPD, OUTPUT);
  pinMode(LEFTDIR, OUTPUT);
  digitalWrite(LEFTDIR, LOW);//left motor setup
  pinMode(LASERR, OUTPUT);
  digitalWrite(LASERR, HIGH);
  pinMode(LASERL, OUTPUT);
  digitalWrite(LASERL, HIGH);//laser setup
  pinMode(LEDR, INPUT);
  pinMode(LEDL, INPUT);

  fallSafeL = analogRead(LEDL);
  fallSafeR = analogRead(LEDR);

  int startChoice = random(2); //generates a random number (either 0 or 1)

  if(startChoice == 0) {
    rightStartEvent();
  }
  else {
    leftStartEvent();
  }
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

//saves subsequentis from falling
void lifeSaveEvent () {
  analogWrite(RIGHTSPD, 0);
  analogWrite(LEFTSPD, 0);
}

void rightStartEvent() {
  //will hold the code for the start event moving right
}

void leftStartEvent() {
  //will hold the code for the start event moving right
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
   while (contR >= (baseR - THRESHLASER) && contL >= (baseL - THRESHLASER)) {
    digitalWrite(LEFTDIR, LOW);
    analogWrite(LEFTSPD, 100);
    analogWrite(RIGHTSPD, 100);
    delay(500);//should be removed
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }

  //cup is only detected by right, so veer toward it and read
  while (!(contL >= (baseL - THRESHLASER)) && contR >= (baseR - THRESHLASER)) {
    veerRight();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }
  //cup is only detected by left, veer left, read
  while(!(contR >= (baseR - THRESHLASER)) && contL >= (baseL - THRESHLASER)) {
    veerLeft();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
  }

  return;
}




void loop() {
  int nextFallSafeR = analogRead(LEDR);//get new readings from the table sensors
  int nextFallSafeL = analogRead(LEDL);

while(nextFallSafeR > (fallSafeR - THRESHLED) && nextFallSafeL > (fallSafeL - THRESHLED)) {


  int originalR = analogRead(TRANSR);//read laser transistors for baseline
  int originalL = analogRead(TRANSL);
  digitalWrite(LEFTDIR, HIGH);
  analogWrite(LEFTSPD, 75);
  analogWrite(RIGHTSPD, 75);
  //turn counterclockwise
  int nextR = analogRead(TRANSR);//read laser transistors again
  int nextL = analogRead(TRANSL);

  if (nextR > (originalR + THRESHLASER) || nextL > (originalL + THRESHLASER)) {
    attackCup();
  }



  nextFallSafeR = analogRead(LEDR);
  nextFallSafeL = analogRead(LEDL);

}

lifeSaveEvent();







}
