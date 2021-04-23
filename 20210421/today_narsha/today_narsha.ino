void setup()
{
	Serial.begin(9600);
	DDRB |= (0x01 << 7);
	
	TCCR2A = 0x00; //normal 모드로 설정
	TCCR2B = 0x02; //8분주기 설정
	TCNT2 = 56;
	TIMSK2 = 0x01;
}

unsigned long curr_millis;
unsigned long prev_millis;
char pwm_duty = 1;

void loop()
{
	// PORTB |= (0x01 << 7);
	// delay(1000);
	// PORTB &= ~(0x01 << 7);
	// delay(1000);

	curr_millis = millis();
	if(curr_millis - prev_millis > 10){
		prev_millis = curr_millis;

		pwm_duty++;
		if(pwm_duty == 99) pwm_duty = 1;
	}
}

int count = 0;
SIGNAL(TIMER2_OVF_vect)
{
	TCNT2 = 56;
	count++;
	if(count == 100){
		count = 0;
		PORTB |= (0x01 << 7);
	}
	else if(count == pwm_duty){
		PORTB &= ~(0x01 << 7);
	}
}