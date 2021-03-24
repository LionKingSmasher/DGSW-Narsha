#define X_DIR 21
#define X_STOP 18
#define X_STEP 15

#define XYE_ENABLE 14

void setup() {
  Serial.begin(9600);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STOP, INPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(XYE_ENABLE, OUTPUT);
  
  digitalWrite(X_DIR, HIGH);
  digitalWrite(XYE_ENABLE, LOW);
}

//void loop() {
//  int in_value = digitalRead(X_STOP);
//  if(in_value == 0)
//  while(
//  {
//    digitalWrite(X_STEP, HIGH);
//    delayMicroseconds(200);
//    digitalWrite(X_STEP, LOW);
//    delayMicroseconds(200);
//  }
//}

unsigned long curr_micros = 0;
unsigned long pre_micros = 0;
char step_toggle = 0;
char motor_stop = 0;

void loop(){
  curr_micros = micros();

  if(curr_micros - pre_micros > 200)
  {
    pre_micros = curr_micros;
    if(motor_stop==0)
    {
      if(step_toggle == 0){
        step_toggle = 1;
        digitalWrite(X_STEP, HIGH);
      }
      else if (step_toggle == 1){
        step_toggle = 0;
        digitalWrite(X_STEP, LOW);
      }
    }
  }
  
  int in_value = digitalRead(X_STOP);
  if(in_value == 1)
  {
    motor_stop = 1;
  }
  else motor_stop = 0;
}
