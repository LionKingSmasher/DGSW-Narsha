#define MEGA2560_LCD 13

void setup() {
  pinMode(MEGA2560_LCD, OUTPUT);
  pinMode(12, OUTPUT);
}

int duty = 1;
int duty_2 = 1;
int count = 0;
int count_variable = 0;
void loop() {
  
//  for(int j = 0; j < 10; j++){
//    for(int i = 0; i< 10; i++){
//      digitalWrite(MEGA2560_LCD, HIGH);
//      delayMicroseconds(duty);
//      digitalWrite(MEGA2560_LCD, LOW);
//      delayMicroseconds(10000 - duty);
//    }
//    duty += 1000;
//    if(duty > 9000) duty = 1000;
//  }

//------------------------------------------
  if(count == 100){
    count = 0;
    digitalWrite(13, HIGH);
  } 
  else if(count == duty_2) digitalWrite(13, LOW);
//------------------------------------------
// 가변
  count_variable++;
  if(count_variable == 200){
    count_variable = 0;
    duty_2 = ++duty;
    if(duty>198) duty = 1;
    if(duty>99) duty_2 = 198 - duty;
  }
//------------------------------------------
  count++;
  delayMicroseconds(100);
//------------------------------------------
}
