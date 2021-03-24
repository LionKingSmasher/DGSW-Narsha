void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

int duty = 1;
int duty_1 = 1;
int duty_2 = 1;
int count = 0;
int count_variable_1 = 0;
int count_variable_2 = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(count==100){
    count = 0;
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
  }
  else if(count == duty_1) digitalWrite(13, LOW);
  if(count == duty_2) digitalWrite(12, LOW);

  count_variable_2 = ++count_variable_1;
  if(count_variable_1 == 200){
    count_variable_1 = 0;
    ++duty_1;
    if(duty_1 > 100) duty_1 = 1;
  }
  if(count_variable_2 == 100){
    count_variable_2 = 0;
    ++duty_2;
    if(duty_2 > 100) duty_2 = 1;
  }
  
  count++;
  delayMicroseconds(100);
}
