void setup() {
	Serial.begin(9600);
	pinMode(13, OUTPUT);
}

int servo_duty = 700;
int duty_count = 22;
int count = 0;
int count_1 = 0;

// void loop() {
// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(13, HIGH);
// 	// 	delayMicroseconds(700);
// 	// 	digitalWrite(13, LOW);
// 	// 	delay(17);
// 	// 	delayMicroseconds(2300);
// 	// }
// 	//for(int j = 0; j < 50; j++){
// 	// 	for(int i = 0; i < 2; i++){
// 	// 		digitalWrite(7, HIGH);
// 	// 		delayMicroseconds(servo_duty);
// 	// 		digitalWrite(7, LOW);
// 	// 		delay(17);
// 	// 		delayMicroseconds(3000-servo_duty);
// 	// 	}
// 	// 	servo_duty += 32;
// 	// 	if(servo_duty > 2300) servo_duty = 700;
// 	// }
// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(13, HIGH);
// 	// 	delayMicroseconds();
// 	// 	digitalWrite(13, LOW);
// 	// 	delay(17);
// 	// 	delayMicroseconds(700);
// 	// }

// 	//--------------------------
// 	// count++;
// 	// if(count == 625){
// 	// 	count = 0;
// 	// 	digitalWrite(7, HIGH);
// 	// }
// 	// else if(count == duty_count){
// 	// 	digitalWrite(7, LOW);
// 	// }
// 	// delayMicroseconds(32);

// 	// //--------------------------

// 	// if(count_1 == 625 * 50){
// 	// 	count_1 = 0;
// 	// 	duty_count++;
// 	// 	if(duty_count > 72) duty_count = 22;
// 	// }

//  //  count_1++;

// 	//--------------------------

// }


unsigned long curr_micros = 0;
unsigned long pre_micros = 0;
unsigned long curr_millis = 0;
unsigned long pre_millis = 0;

void loop(){
	curr_micros = micros();
	curr_millis = millis();

	if(curr_micros - pre_micros > 32){
		pre_micros = curr_micros;
		if(count == 625){
			count = 0;
			digitalWrite(13, HIGH);
		}
		else if(count == 22){
			digitalWrite(13, LOW);
		}
		count++;
	}

	if(curr_millis - pre_millis > 100){
		pre_millis = curr_millis;

		duty_count++;
		if(duty_count > 72) duty_count = 22;
	}
}