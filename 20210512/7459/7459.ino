#define DATA  7
#define CLK   8
#define LATCH 9

unsigned char value_7459 = 0x03;

void setup()
{
	Serial.begin(9600);
	for(int i = 0; i < 3; i++)
		pinMode(DATA+i, OUTPUT);
	spi_74595(0x03);
}

int count = 0;
int count_variable = 0;
int pwm_duty = 1;

void loop()
{
	count++;
	if(count==100){
		count = 0;

		value_7459 |= 0x01;
		spi_74595(value_7459);
	}
	else if(count == pwm_duty){`

		value_7459 &= ~0x01;
		spi_74595(value_7459);
	}
	//-------------------------

	//-------------------------
	count_variable++;
	if(count_variable == 100){
		count_variable = 0;
		pwm_duty++;
		if(pwm_duty==99) pwm_duty = 1;
	}
	delayMicroseconds(100);
}

void spi_74595(unsigned char data){
	
	for(int i = 0; i < 8; i++){
		if(data & (0x80 >> i)) digitalWrite(DATA, HIGH);
		else digitalWrite(DATA, LOW);

		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}
	digitalWrite(LATCH, HIGH);
	digitalWrite(LATCH, LOW);
}