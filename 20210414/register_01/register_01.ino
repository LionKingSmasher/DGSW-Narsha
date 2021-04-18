void setup() {
	//pinMode(13, OUTPUT);
	//Data Direction Register
	//DDRB |= (0x01 << 7);
	DDRH |= (0x01 << 4);
	TCCR2A = 0x03; //0: OVF, 2: CTC 3: FAST PWM
	TCCR2B = 0x05; //128분주기
	TCNT2 = 6;
	OCR2A = 31;
	TIMSK2 = 0x03; //1: OVF, 2: CTC, FAST PWM
}

unsigned long curr_millis = 0;
unsigned long prev_millis = 0;
int pwm_duty = 1;


void loop() {
	curr_millis = millis();
	if(curr_millis - prev_millis > 5){
		prev_millis = curr_millis;
		pwm_duty++;


		if(pwm_duty == 255) pwm_duty = 6;
	}

	// //digitalWrite(13, HIGH);
	// PORTH |= (0x01 << 4);
	// delay(1000);
	// //digitalWrite(13, LOW);
	
	// PORTH &= ~(0x01 << 4);
	// delay(1000);
}

unsigned int count = 0;
unsigned char toggle_flag = 0;

// SIGNAL(TIMER2_OVF_vect){
// 	TCNT2 = 56;
// 	count++;
// 	if(count > 10000){
// 		count = 0;
// 		if(toggle_flag == 0){
// 			PORTH |= (0x01 << 4); //led on
// 			toggle_flag = 1;
// 		}
// 		else{
// 			PORTH &= ~(0x01 << 4); //led off
// 			toggle_flag = 0;
// 		}
// 	}
// }

// SIGNAL(TIMER2_OVF_vect){
// 	TCNT2 = 56;
// 	count++;
// 	if(count == 100){
// 		count = 0;
// 		PORTH |= (0x01 << 4);
// 	} else if(count == pwm_duty){
// 		PORTH &= ~(0x01 << 4);
// 	}
// }

SIGNAL(TIMER2_COMPA_vect){ // CTC 모드
	PORTH &= ~(0x01 << 4);
}

SIGNAL(TIMER2_OVF_vect){ // 오버플로우 모드
	TCNT2 = 6;
	PORTH |= (0x01 << 4);
	OCR2A = pwm_duty;
}

