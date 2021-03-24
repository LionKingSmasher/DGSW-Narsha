unsigned long premicros = 0;
unsigned long premicros2 = 0;
unsigned long cur_micros = 0;
int count = 0;

void setup(){
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop(){
  cur_micros = micros();
  if(cur_micros - premicros > 100){
    premicros = cur_micros;
    count++;
    if(count == 100){
      count = 0;
      digitalWrite(13, HIGH);
    }
    else if(count == 10){
      digitalWrite(13, LOW);
    }
  }
//  cur_millis = millis();
//  if(cur_millis - premillis > 1000){
//    premillis = cur_millis;
//    Serial.println("HaHaHaHa");
//  }
//  if(cur_millis - premillis2 > 500){
//    premillis2 = cur_millis;
//    Serial.println("Hojin Bob");
//  }
}
