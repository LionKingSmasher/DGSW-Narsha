
// void setup() {
// 	Serial.begin(9600);
// 	pinMode(7, OUTPUT);
// }

// void loop() {
// 	for(int i = 0; i < 50; i++){
// 		digitalWrite(7, HIGH);
// 		delayMicroseconds(700);
// 		digitalWrite(7, LOW);
// 		delay(170);
// 		delayMicroseconds(2300);
// 	}
// 	for(int i = 0; i < 50; i++){
// 		digitalWrite(7, HIGH);
// 		delayMicroseconds(2300);
// 		digitalWrite(7, LOW);
// 		delay(170);
// 		delayMicroseconds(700);
// 	}
// }

unsigned int count = 0;
unsigned int count_1sec = 0;
volatile int servo_duty = 32;

char toggle_1sec = 0;


// void loop() {
// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(7, HIGH);
// 	// 	delayMicroseconds(700);
// 	// 	digitalWrite(7, LOW);
// 	// 	delay(170);
// 	// 	delayMicroseconds(2300);
// 	// }
// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(7, HIGH);
// 	// 	delayMicroseconds(2300);
// 	// 	digitalWrite(7, LOW);
// 	// 	delay(170);
// 	// 	delayMicroseconds(700);
// 	// }

// //----------------------------------
// 	if(count == 625){
// 		count = 0;
// 		digitalWrite(7, HIGH);
// 	}
// 	else if(count == servo_duty){	
// 		digitalWrite(7, LOW);
// 	}
// //----------------------------------
// 	if(count_1sec == 625 * 50){
// 		count_1sec = 0;
// 		if(toggle_1sec == 0){
// 			toggle_1sec = 1;
// 			servo_duty = 22;
// 		} 
// 		else {
// 			toggle_1sec = 0;
// 			servo_duty = 72;
// 		}
// 	}
// //----------------------------------
// 	count_1sec++;
// 	count++;
// 	delayMicroseconds(32);

// }

unsigned long curr_micros = 0;
unsigned long prev_micros = 0;

unsigned long curr_millis = 0;
unsigned long prev_millis = 0;

// void loop(){
// 	curr_micros = micros();
// 	curr_millis = millis();
// 	if(curr_micros - prev_micros > 32){
// 		prev_micros = curr_micros;
// 		if(count == 625){
// 			count = 0;
// 			digitalWrite(7, HIGH);
// 		}
// 		else if(count == servo_duty){
// 			digitalWrite(7, LOW);
// 		}
// 		count++;
// 	}
// 	if(curr_millis - prev_millis > 1000){
// 		prev_millis = curr_millis;
// 		if(toggle_1sec == 0){
// 			toggle_1sec = 1;
// 			servo_duty = 20;
// 		} 
// 		else{
// 			toggle_1sec = 0;
// 			servo_duty = 42;
// 		}
// 	}
// }

#define digital_write_7_HIGH PORTH |= 0x10;
#define digital_write_7_LOW  PORTH &= ~0x10;

// void loop(){
// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(7, HIGH);
// 	// 	delayMicroseconds(647);
// 	// 	digitalWrite(7, LOW);
// 	// 	delay(17);
// 	// 	delayMicroseconds(3000-647);
// 	// }

// 	// for(int i = 0; i < 50; i++){
// 	// 	digitalWrite(7, HIGH);
// 	// 	delayMicroseconds(1473);
// 	// 	digitalWrite(7, LOW);
// 	// 	delay(17);
// 	// 	delayMicroseconds(3000-2473);
// 	// }
// 	// if(count == 625){
// 	// 	count = 0;
// 	// 	digitalWrite(7, HIGH);
// 	// }
// 	// else if(count == servo_duty){
// 	// 	digitalWrite(7, LOW);
// 	// }

// 	// if(count_1sec == 625 * 50){
// 	// 	count_1sec = 0;
// 	// 	if(toggle_1sec == 0){
// 	// 		toggle_1sec = 1;
// 	// 		servo_duty = 20;
// 	// 	}
// 	// 	else{
// 	// 		toggle_1sec = 0;
// 	// 		servo_duty = 46;
// 	// 	}
// 	// }
// 	// count_1sec++;
// 	// count++;
// 	// delayMicroseconds(32);

// 	curr_micros = micros();
// 	curr_millis = millis();
// 	if(curr_micros - prev_micros > 32){
// 		prev_micros = curr_micros;
// 		if(count == 625){
// 			count = 0;
// 			digital_write_7_HIGH;
// 		}
// 		else{
// 			digital_write_7_LOW;
// 		}
// 		count++;
// 	}

// 	if(curr_millis - prev_millis > 1000){
// 		prev_millis = curr_millis;
// 		if(toggle_1sec == 0){
// 			toggle_1sec = 1;
// 			servo_duty = 20;
// 		}
// 		else{
// 			toggle_1sec = 0;
// 			servo_duty = 46;
// 		}
// 	}
// }

// #define m(X) (1 << X)

// void setup() {
// 	DDRB |= m(7);
// }

// void loop(){
// 	PORTB |= m(7);
// 	delay(1000);
// 	PORTB &= ~m(7);
// 	delay(1000);
// }

void setup()
{
	DDRB |= (0x01 << 6); //13번 핀

	TCCR2A = 0x00;
	TCCR2B = 0x02;
	TCNT2  = 256 - 64;
	TIMSK2 = 0x01;
}

void loop() 
{
	curr_millis = millis();
	if(curr_millis - prev_millis > 1000){
		prev_millis = curr_millis;
		if(toggle_1sec == 0){
			toggle_1sec = 1;
			servo_duty = 22;
		}
		else{
			toggle_1sec = 0;
			servo_duty = 72;
		}
	}
}

SIGNAL(TIMER2_OVF_vect){
	TCNT2 = 256 - 64;
	if(count >= 625){
		count = 0;
		PORTB |= (0x01 << 6);
	}
	else if(count >= servo_duty){
		PORTB &= ~(0x01 << 6);
	}
	count++;
}