#include "BluetoothSerial.h"

BluetoothSerial bt;

//drive pins
// Front/Rear Left/Right Forwards/Reverse
int pinFLF = 14;
int pinFLR = 27;
int pinFRF = 32;
int pinFRR = 33;
int pinRLF = 13;
int pinRLR = 12;
int pinRRF = 25;
int pinRRR = 26;

//speeds
int speedX = 0;
int speedY = 0;
int speedR = 0;

char currentCommand;
int stickNum = 0;
int tempdriveX = 0;
int tempdriveY = 0;
int tempdriveR = 0;
int tempdriveAngle = 0;
int tempdriveSpeed = 0;
int isNegative = 1;



void setup() {
  Serial.begin(115200);
  bt.begin();
  pinMode(pinFLF,OUTPUT);
  pinMode(pinFLR,OUTPUT);
  pinMode(pinFRF,OUTPUT);
  pinMode(pinFRR,OUTPUT);
  pinMode(pinRLF,OUTPUT);
  pinMode(pinRLR,OUTPUT);
  pinMode(pinRRF,OUTPUT);
  pinMode(pinRRR,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  checkCommands();
}

void set(){
  if(currentCommand=='Y'){//XY sticks
  tempdriveY = tempdriveY * isNegative;
      if (stickNum==1){
        speedR = tempdriveX;
        speedY = tempdriveY;
        setDrive();
      }else if (stickNum==2){
        speedX = tempdriveX;
        setDrive();
      }
  }else if (currentCommand == 'A'){

  }
}

void setDrive(){
  int fl = -speedY + speedX + speedR;
  int fr = -speedY - speedX - speedR;
  int rl = -speedY - speedX + speedR;
  int rr = -speedY + speedX - speedR;
  
  int max = 0;
  if (abs(fl)>max){max = abs(fl);}
  if (abs(fr)>max){max = abs(fr);}
  if (abs(rl)>max){max = abs(rl);}
  if (abs(rr)>max){max = abs(rr);}

  if(max>255){
    fl=fl*255/max;
    fr=fr*255/max;
    rl=rl*255/max;
    rr=rr*255/max;
  }

  if(fl>0){
    analogWrite(pinFLR,0);
    analogWrite(pinFLF,fl);
  }else{
    analogWrite(pinFLF,0);
    analogWrite(pinFLR,-fl);
  }
  if(fr>0){
    analogWrite(pinFRR,0);
    analogWrite(pinFRF,fr);
  }else{
    analogWrite(pinFRF,0);
    analogWrite(pinFRR,-fr);
  }
  if(rl>0){
    analogWrite(pinRLR,0);
    analogWrite(pinRLF,rl);
  }else{
    analogWrite(pinRLF,0);
    analogWrite(pinRLR,-rl);
  }
  if(rr>0){
    analogWrite(pinRRR,0);
    analogWrite(pinRRF,rr);
  }else{
    analogWrite(pinRRF,0);
    analogWrite(pinRRR,-rr);
  }
}

void testDrive(){
  // FL > FR > RL > RR
  analogWrite(pinFLR,0);
  analogWrite(pinFLF,255);
  delay(500);
  analogWrite(pinFLF,0);
  analogWrite(pinFLR,255);
  delay(500);
  analogWrite(pinFLR,0);

  analogWrite(pinFRR,0);
  analogWrite(pinFRF,255);
  delay(500);
  analogWrite(pinFRF,0);
  analogWrite(pinFRR,255);
  delay(500);
  analogWrite(pinFRR,0);

  analogWrite(pinRLR,0);
  analogWrite(pinRLF,255);
  delay(500);
  analogWrite(pinRLF,0);
  analogWrite(pinRLR,255);
  delay(500);
  analogWrite(pinRLR,0);

  analogWrite(pinRRR,0);
  analogWrite(pinRRF,255);
  delay(500);
  analogWrite(pinRRF,0);
  analogWrite(pinRRR,255);
  delay(500);
  analogWrite(pinRRR,0);

}

void checkCommands() {
  while (bt.available() > 0) {
    char nextChar = (char)bt.read();
    Serial.print(nextChar);
    if (nextChar == '?') {
    
    } else if (nextChar == 'S') {
      stickNum = 0;
      currentCommand = 'S';
    } else if (nextChar == 'R') {
      tempdriveSpeed = 0;
      currentCommand = 'R';
    } else if (nextChar == 'A') {
      tempdriveAngle = 0;
      currentCommand = 'A';
    } else if (nextChar == 'X') {
      tempdriveX = 0;
      isNegative = 1;
      currentCommand = 'X';
    } else if (nextChar == 'Y') {
      tempdriveX = tempdriveX * isNegative;
      tempdriveY = 0;
      isNegative = 1;
      currentCommand = 'Y';
    } else if (nextChar == '-') {
      isNegative = -1;
    } else if (nextChar == 'T') {
      testDrive();
    } else if (nextChar == '!') {
      set();
      Serial.println(' ');
    } else {
      if (nextChar < 48 || nextChar > 57) { nextChar = 48; }
      switch (currentCommand) {
        case 'S':
          stickNum = stickNum * 10 + nextChar - 48;
          break;
        case 'R':
          tempdriveSpeed = tempdriveSpeed * 10 + nextChar - 48;
          break;
        case 'A':
          tempdriveAngle = tempdriveAngle * 10 + nextChar - 48;
          break;
        case 'X':
        tempdriveX = tempdriveX * 10 + nextChar - 48;
          break;
        case 'Y':
        tempdriveY = tempdriveY * 10 + nextChar - 48;
          break;
      }
    }
  }
}