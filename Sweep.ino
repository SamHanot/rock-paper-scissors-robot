#include <Servo.h>

//fingers
Servo thumb;
Servo indexF;
Servo middleF;
Servo ring;
Servo pinky;


//Leds
const int RedLed = 12;
const int YelLed = 8;
const int GreLed = 3;
const int ButPin = 2;

//Button
int buttonState = 0;

//servo positions
int posT = 0;
int posI = 0;
int posM = 0;
int posR = 0;
int posP = 0;

//random number
int r = 0;

//Sensor
const int EchoPin = 6;
const int TrigPin = 4;
float duration, distance;

//Variable for the game mode: False -> random , True -> vision
bool gamemode = false; 

void setup() {
  thumb.attach(17);
  indexF.attach(16);
  middleF.attach(15);
  ring.attach(14);
  pinky.attach(13);
  Serial.begin(9600);

  pinMode(RedLed, OUTPUT);
  pinMode(YelLed, OUTPUT);
  pinMode(GreLed, OUTPUT);
  pinMode(ButPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(ButPin), changeState, RISING);

  randomSeed(analogRead(0));

  pinMode(TrigPin, OUTPUT);  
	pinMode(EchoPin, INPUT);  


}

void loop() {

  launchGame();
  digitalWrite(GreLed, LOW);
  digitalWrite(RedLed, LOW);
  digitalWrite(YelLed, LOW);
  
}

void scissors() {
  while (posI <= 180 || posM <= 180 || posT >= 50 || posR >= 0 || posP >= 0) {
    if (posI <= 180) {
      posI += 1;
      indexF.write(posI);
    }
    if (posM <= 180) {
      posM += 1;
      middleF.write(posM);
    }
    if (posT >= 50) {
      posT -= 1;
      thumb.write(posT);
    }
    if (posR >= 0) {
      posR -= 1;
      ring.write(posR);
    }
     if (posP >= 0) {
      posP -= 1;
      pinky.write(posP);
    }
    delay(1);
  }
  Serial.println("scissors");
}

void rock() {
  while (posI >= 0 || posM >= 0 || posT >= 50 || posR >= 0 || posP >= 0) {
    if (posI >= 0) {
      posI -= 1;
      indexF.write(posI);
    }
    if (posM >= 0) {
      posM -= 1;
      middleF.write(posM);
    }
    if (posT >= 50) {
      posT -= 1;
      thumb.write(posT);
    }
     if (posR >= 0) {
      posR -= 1;
      ring.write(posR);
    }
     if (posP >= 0) {
      posP -= 1;
      pinky.write(posP);
    }
    delay(1);
    
  }
  Serial.println("rock");
}

void paper() {
  while (posI <= 180 || posM <= 180 || posT <= 180 || posR <= 180 || posP <= 180) {
    if (posI <= 180) {
      posI += 1;
      indexF.write(posI);
    }
    if (posM <= 180) {
      posM += 1;
      middleF.write(posM);
    }
    if (posT <= 180) {
      posT += 1;
      thumb.write(posT);
    }
    if (posR <= 180) {
      posR += 1;
      ring.write(posR);
    }
    if (posP  <= 180) {
      posP += 1;
      pinky.write(posP);
    }

    delay(1);
  }
  Serial.println("paper");
}

void round() {
  r = random(0, 3);
  switch (r) {
    case 0:
      paper();
      break;
    case 1:
      rock();
      break;
    case 2:
      scissors();
      break;
  }
}

void launchGame(){
  if(!gamemode){
    digitalWrite(TrigPin, LOW);  
	  delayMicroseconds(2);  
	  digitalWrite(TrigPin, HIGH);  
	  delayMicroseconds(10);  
	  digitalWrite(TrigPin, LOW);  
    duration = pulseIn(EchoPin, HIGH); 
    distance = (duration*.0343)/2;

    if (distance <= 10){
      digitalWrite(RedLed, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                 // wait for a second
      digitalWrite(YelLed, HIGH);  // turn the LED off by making the voltage LOW
      delay(1000);                 // wait for a second
      digitalWrite(GreLed, HIGH);
      round();
    }
  }
  else {
    visionGame();
  }
}

void visionGame(){
    int move = Serial.readString().toInt();
    if(move != 0){
      digitalWrite(GreLed, HIGH);
      move -= 1;
      switch (move) {
      case 0:
        paper();
        break;
      case 1:
        rock();
        break;
      case 2:
        scissors();
        break;
      }
      move = 0;
    }
}

void changeState(){
  gamemode = !gamemode;
}
