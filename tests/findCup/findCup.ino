
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
#define LEFTBUTTON 12
#define RIGHTBUTTON 13


//define thresholds
#define THRESHLASER 50
#define THRESHLEDR 150
#define THRESHLEDL 150
#define TIMETHRESH 4000

//define speeds
#define FORWARDSPD 255
#define SCANMOVESPD 150
#define BACKWARDSPD 150
#define ROTATESPD 125
#define VEERDIFF 30
#define WEAPONSPD 200



//global variables
int fallSafeR;
int fallSafeL;
int nextFallSafeR;
int nextFallSafeL;//variables for staying on table

int originalR;
int originalL;//will be initital readings from laser, updated each time main loop runs

bool rightSawLast = false;

int oldTime = 0;//variable to prevent spinning in place

int spinBreakDirectionChoice = 0;



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
  //startEvent(startChoice);
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
  if (nextFallSafeR < (fallSafeR - THRESHLEDR)) {
    analogWrite(LEFTSPD, BACKWARDSPD - VEERDIFF);
    analogWrite(RIGHTSPD, BACKWARDSPD);
    delay(500);
    rightSawLast = false;//veer back and away and begin scanning left
  }
  //left led detected table edge
  else if (nextFallSafeL < (fallSafeL - THRESHLEDL)) {
    analogWrite(RIGHTSPD, BACKWARDSPD - VEERDIFF);
    analogWrite(LEFTSPD, BACKWARDSPD);
    delay(500);
    rightSawLast = true;//veer back and away from edge and begin scanning right
  }

  return;

}

void spinBreaker() {
  spinBreakDirectionChoice++;
  if ((spinBreakDirectionChoice % 4) == 0) {
    digitalWrite(LEFTDIR, HIGH);
    digitalWrite(RIGHTDIR, LOW);
    digitalWrite(LEFTSPD, SCANMOVESPD);
    digitalWrite(RIGHTSPD, SCANMOVESPD);
    delay(250);
    return;
  }
  else {
    digitalWrite(LEFTDIR, LOW);
    digitalWrite(RIGHTDIR, HIGH);
    analogWrite(LEFTSPD, SCANMOVESPD);
    analogWrite(RIGHTSPD, SCANMOVESPD);
  }


  nextFallSafeR = analogRead(LEDR);
  nextFallSafeL = analogRead(LEDL);
  for (int i = 0; i < 100; i++) {
    if(nextFallSafeR < (fallSafeR - THRESHLEDR) || nextFallSafeL < (fallSafeL - THRESHLEDL)) {
      lifeSaveEvent();
      return;
    }
    nextFallSafeR = analogRead(LEDR);
    nextFallSafeL = analogRead(LEDL);
    delay(20);
  }

  return;

}

void startEvent(int choice) {
  //start event moving right
  if (choice == 1) {
    analogWrite(LEFTSPD, 235 - (2 * VEERDIFF));
    analogWrite(RIGHTSPD, 235);
    delay(1200);
    analogWrite(RIGHTSPD,0);
    analogWrite(LEFTSPD, 0);
    delay(100);
    rightSawLast = true;
  }
  else if (choice == 2) {
    analogWrite(LEFTSPD, 235);
    analogWrite(RIGHTSPD, 235 - (2 * VEERDIFF));
    delay(1200);
    analogWrite(RIGHTSPD,0);
    analogWrite(LEFTSPD, 0);
    delay(100);
  }
  return;
}





//go to cup function
void attackCup() {
  analogWrite(LEFTSPD, 0);
  analogWrite(RIGHTSPD, 0);
  analogWrite(WHIPSPD, WEAPONSPD);//turn on the weapon
  int contR = analogRead(TRANSR);//reading that will be updated
  int contL = analogRead(TRANSL);

  //loop continues while cup is still detected by both modules
   while (contR > (originalR + THRESHLASER) && contL > (originalL + THRESHLASER) && nextFallSafeR > (fallSafeR - THRESHLEDR) && nextFallSafeL > (fallSafeL - THRESHLEDL)) {
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

  oldTime = millis();//update old time

  return;
}




void loop() {
  int nextFallSafeR = analogRead(LEDR);//get new readings from the table sensors
  int nextFallSafeL = analogRead(LEDL);


while(nextFallSafeR > (fallSafeR - THRESHLEDR) && nextFallSafeL > (fallSafeL - THRESHLEDL)) {

  analogWrite(WHIPSPD, 0);//turn off the weapon
  originalR = analogRead(TRANSR);//read laser transistors for baseline
  originalL = analogRead(TRANSL);
  
  //button checking
  if(!digitalRead(LEFTBUTTON)) {
    rightSawLast = true;
  }
  else if(!digitalRead(RIGHTBUTTON)) {
    rightSawLast = false;
  }

  //decide spin direction
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

  if(millis() > (oldTime + TIMETHRESH)) {
    spinBreaker();
    oldTime = millis();
  }


  nextFallSafeR = analogRead(LEDR);
  nextFallSafeL = analogRead(LEDL);

}


lifeSaveEvent();
}
