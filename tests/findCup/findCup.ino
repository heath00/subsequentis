
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
#define WHIPSPD 4
#define WHIPDIR 5


//define thresholds
#define THRESHLASER 15
#define THRESHLED 750

//define speeds
#define FORWARDSPD 255
#define BACKWARDSPD 50
#define ROTATESPD 75
#define VEERDIFF 20
#define WEAPONSPD 255



//global variables
int fallSafeR;
int fallSafeL;
int nextFallSafeR;
int nextFallSafeL;//variables for staying on table

int originalR;
int originalL;//will be initital readings from laser, updated each time main loop runs

bool rightSawLast = false;

int oldTime = 0;
int newTime = 0;//variables to prevent spinning in place more than 4 times in a row


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

  randomSeed(analogRead(TRANSR));

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
  digitalWrite(LEFTDIR, HIGH);
  digitalWrite(RIGHTDIR, LOW);
  delay(100);//stop subsequentis, reverse motor direction, brief pause


  //right led detected table edge
  if (nextFallSafeR < (fallSafeR - THRESHLED)) {
    digitalWrite(LEFTSPD, BACKWARDSPD - VEERDIFF);
    digitalWrite(RIGHTSPD, BACKWARDSPD);
    delay(500);
    rightSawLast = false;//veer back and away and begin scanning left
  }
  //left led detected table edge
  else if (nextFallSafeL > (fallSafeL - THRESHLED)) {
    digitalWrite(RIGHTSPD, BACKWARDSPD - VEERDIFF);
    digitalWrite(LEFTSPD, BACKWARDSPD);
    delay(500);
    rightSawLast = true;//veer back and away from edge and begin scanning right
  }

  return;

}

void spinBreaker() {
  digitalWrite(LEFTDIR, LOW);
  digitalWrite(RIGHTDIR, HIGH);
  int i;
  for (i = 0; i < 4  && nextFallSafeR > (fallSafeR - THRESHLED) && nextFallSafeL > (fallSafeL - THRESHLED); i++) {
    digitalWrite(LEFTSPD, FORWARDSPD);
    digitalWrite(RIGHTSPD, FORWARDSPD);
    delay(500);
    nextFallSafeR = analogRead(LEDR);
    nextFallSafeL = analogRead(LEDL);
  }

  if(i != 4) {
    lifeSaveEvent();
  }

}

void startEvent(int choice) {
  //start event moving right
  if (choice == 1) {
    analogWrite(LEFTSPD, 235 - (2 * VEERDIFF));
    analogWrite(RIGHTSPD, 235);
    delay(1200);
    digitalWrite(RIGHTSPD,0);
    digitalWrite(LEFTSPD, 0);
    delay(100);
    rightSawLast = true;
  }
  else if (choice == 2) {
    analogWrite(LEFTSPD, 235);
    analogWrite(RIGHTSPD, 235 - (2 * VEERDIFF));
    delay(1200);
    digitalWrite(RIGHTSPD,0);
    digitalWrite(LEFTSPD, 0);
    delay(100);
  }
  
}





//go to cup function
void attackCup() {
  analogWrite(LEFTSPD, 0);
  analogWrite(RIGHTSPD, 0);
  //analogWrite(WHIPSPD, WEAPONSPD);//turn on the weapon
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

  oldTime = newTime;//update old time

  return;
}




void loop() {
  int nextFallSafeR = analogRead(LEDR);//get new readings from the table sensors
  int nextFallSafeL = analogRead(LEDL);
  //analogWrite(WHIPSPD, 0);//turn off the weapon

while(nextFallSafeR > (fallSafeR - THRESHLED) && nextFallSafeL > (fallSafeL - THRESHLED)) {

  newTime = millis();

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

  if(newTime > (oldTime + 24000)) {
    oldTime = newTime;
    spinBreaker();
  }


  nextFallSafeR = analogRead(LEDR);
  nextFallSafeL = analogRead(LEDL);

}


lifeSaveEvent();
}
