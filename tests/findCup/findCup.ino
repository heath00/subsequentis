
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

//define thresholds
#define THRESHLASER 20
#define THRESHLED 75

//define speeds
#define FORWARDSPD 150
#define ROTATESPD 75
#define VEERDIFF 20



//global variables
int fallSafeR;
int fallSafeL;
int nextFallSafeR;
int nextFallSafeL;//variables for staying on table

int originalR;
int originalL;//will be initital readings from laser, updated each time main loop runs

bool rightSawLast = false;


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
  pinMode(LEDL, INPUT);//led table sensor setup

  fallSafeL = analogRead(LEDL);
  fallSafeR = analogRead(LEDR);

  int startChoice = random(1, 3);//generates a random number (either 1 or 2)
  startEvent(startChoice);
}

//veers subsequentis left
void veerLeft() {
  digitalWrite(RIGHTDIR, HIGH);
  digitalWrite(LEFTDIR, LOW);
  analogWrite(RIGHTSPD, FORWARDSPD);
  analogWrite(LEFTSPD, FORWARDSPD - VEERDIFF);
}

//veers subsequentis right
void veerRight() {
  digitalWrite(RIGHTDIR, HIGH);
  digitalWrite(LEFTDIR, LOW);
  analogWrite(LEFTSPD, FORWARDSPD);
  analogWrite(RIGHTSPD, FORWARDSPD - VEERDIFF);
}

//saves subsequentis from falling
void lifeSaveEvent () {
  analogWrite(RIGHTSPD, 0);
  analogWrite(LEFTSPD, 0);
}

void startEvent(int choice) {
  //start event moving right
  if (choice == 1) {
    veerLeft();
    delay(2500);
    digitalWrite(RIGHTSPD,0);
    digitalWrite(LEFTSPD, 0);
    delay(100);
  }
  else if (choice == 2) {
    veerRight();
    delay(2500);
    digitalWrite(RIGHTSPD,0);
    digitalWrite(LEFTSPD, 0);
    delay(100);
  }
  
}





//basic go to cup function
void attackCup() {
  analogWrite(LEFTSPD, 0);
  analogWrite(RIGHTSPD, 0);
  int contR = analogRead(TRANSR);//reading that will be updated
  int contL = analogRead(TRANSL);

  //loop continues while cup is still detected by both modules
   while (contR > (originalR + THRESHLASER) && contL > (originalL + THRESHLASER) && nextFallSafeR > (fallSafeR - THRESHLED) && nextFallSafeL > (fallSafeL - THRESHLED)) {
    digitalWrite(LEFTDIR, LOW);
    analogWrite(LEFTSPD, FORWARDSPD);
    analogWrite(RIGHTSPD, FORWARDSPD);
    //delay(500);//should be removed
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
    nextFallSafeR = analogRead(LEDR);
    nextFallSafeL = analogRead(LEDL);
    
  }
  //cup is only detected by right, so veer toward it and read
  while (contR > (originalR + THRESHLASER)) {
    veerRight();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
    rightSawLast = true;
  }
  //cup is only detected by left, veer left, read
  while(contL > (originalL + THRESHLASER)) {
    veerLeft();
    contL = analogRead(TRANSL);
    contR = analogRead(TRANSR);
    rightSawLast = false;
  }

  if (contR > (originalR + THRESHLASER) || contL > (originalL + THRESHLASER)) {
    attackCup();
  }
//
//  if (rightSawLast) {
//    veerRight();
//    delay(500);
//  }
//  else {
//    veerLeft();
//    delay(500);
//  }

  return;
}




void loop() {
  int nextFallSafeR = analogRead(LEDR);//get new readings from the table sensors
  int nextFallSafeL = analogRead(LEDL);

while(nextFallSafeR > (fallSafeR - THRESHLED) && nextFallSafeL > (fallSafeL - THRESHLED)) {

  originalR = analogRead(TRANSR);//read laser transistors for baseline
  originalL = analogRead(TRANSL);
  if (rightSawLast) {
    digitalWrite(LEFTDIR, LOW);
    digitalWrite(RIGHTDIR, LOW);
  }
  else {
    digitalWrite(LEFTDIR, HIGH);
    digitalWrite(RIGHTDIR, HIGH);
  }

  analogWrite(LEFTSPD, ROTATESPD);
  analogWrite(RIGHTSPD, ROTATESPD);
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
