#include <math.h>
/*
void setup() {
  pinMode(27, OUTPUT);
  DDRA = 0xff; //PA0~7까지 모두 출력으로 설정(0x11111111)
  //D27은 Beeper을 의미
}

void loop() {
  PORTA |= (1 << 4);
  //D27은 PA4와 동일
  //digitalWrite(27, HIGH); //비프음 내기
  delayMicroseconds(1275); //1초 대기
  PORTA &= ~(1 << 4);
  //digitalWrite(27, LOW); //비프음 안 내기
  delayMicroseconds(1275); //1초 대기
}
*/

#define XY_EN 14
#define X_STEP 15  //PD7
#define X_DIR 21 //PC5
#define Y_DIR 23 //PC7
#define Y_STEP 22 //PC6
#define X_STOP 18

void setup(){
  Serial.begin(9600);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  
  pinMode(XY_EN, OUTPUT);

  digitalWrite(Y_DIR, HIGH);
  digitalWrite(X_DIR, HIGH);
  digitalWrite(XY_EN, LOW);
}

static void X_MOVE(int delay__, double __length){ //delay__가 적을수록 빨라짐
  int len = __length * 80;
  for(int i = 0; i < len; i++){
//    digitalWrite(Y_STEP, HIGH);
    digitalWrite(X_STEP, HIGH);
    delayMicroseconds(delay__);
//    digitalWrite(Y_STEP, LOW);
    digitalWrite(X_STEP, LOW);
    delayMicroseconds(delay__);
  }
}

static void Cross_MOVE(int __delay, int __length){
  int len = __length * 80;
  
  for(int i = 0; i < len; i++){
    digitalWrite(Y_STEP, HIGH);
    digitalWrite(X_STEP, HIGH);
    delayMicroseconds(__delay);
    digitalWrite(Y_STEP, LOW);
    digitalWrite(X_STEP, LOW);
    delayMicroseconds(__delay);
  }
}

static void Y_MOVE(int delay__, int __length){
  int len = __length * 80;
  for(int i = 0; i < len; i++){
//    digitalWrite(Y_STEP, HIGH);
    digitalWrite(Y_STEP, HIGH);
    delayMicroseconds(delay__);
//    digitalWrite(Y_STEP, LOW);
    digitalWrite(Y_STEP, LOW);
    delayMicroseconds(delay__);
  }
}

void loop(){
  int in_data = digitalRead(X_STOP);
  while(in_data);
  X_MOVE(200, 10);
  /*
  if(Serial.available()>0){
    int len = Serial.parseInt();
    Serial.print(len);
    Serial.println("mm");
    Y_MOVE(300, len);
    digitalWrite(Y_DIR, LOW);
    X_MOVE(300, len);
    digitalWrite(X_DIR, LOW);
    Cross_MOVE(300, len);
    digitalWrite(X_DIR, HIGH);
    digitalWrite(Y_DIR, HIGH);
  }
  */
}
