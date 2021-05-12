#include <stdarg.h>

#define CS    28
#define SCK   30
#define MOSI  17

#define ENC_A 10
#define ENC_B 11

enum line_number{
	LINE_1 = 1,
	LINE_2,
	LINE_3,
	LINE_4
};

enum {
	ENC_STATE_1 = 1,
	ENC_STATE_2,
	ENC_STATE_3,
	ENC_STATE_4
};

void printf_(char *format, ...)
{
	char buf[512];
	va_list arglist;

	va_start(arglist, format);
	vsprintf(buf, format, arglist);
	va_end(arglist);
	gLCD_string(buf);
}

void shift_8bit(unsigned char data){
	for(int i = 0; i < 8; i++){
		if(data & (0x80 >> i)){
			digitalWrite(MOSI, HIGH);
		}
		else{
			digitalWrite(MOSI, LOW);
		}
		digitalWrite(SCK, HIGH);
		digitalWrite(SCK, LOW);
	}
}

void gLCD_inst(unsigned char inst){
	
	unsigned char upper_byte = inst & 0xf0;
	unsigned char lower_byte = (inst & 0x0f) << 4;

	digitalWrite(CS, HIGH);
	
	shift_8bit(0xf8);
	shift_8bit(upper_byte);
	shift_8bit(lower_byte);

	digitalWrite(CS, LOW);

	delayMicroseconds(100);
}

void gLCD_init(){
	gLCD_inst(0x30);
	gLCD_inst(0x30);
	gLCD_inst(0x30);
	gLCD_inst(0x38);
	gLCD_inst(0x06);
	gLCD_inst(0x0c);
	gLCD_inst(0x80);
	gLCD_inst(0x01);
	delay(2);

}

void gLCD_data(unsigned char data){
	
	unsigned char upper_byte = data & 0xf0;
	unsigned char lower_byte = (data & 0x0f) << 4;

	digitalWrite(CS, HIGH);
	
	shift_8bit(0xfa);
	shift_8bit(upper_byte);
	shift_8bit(lower_byte);

	digitalWrite(CS, LOW);

	delayMicroseconds(100);
}


int count = 0;
char buf[50] = {0,};

char *main_menu[] = {
	" 1.Step Motor1      ",
	" 2.Step Motor2      ",
	" 3.Step Motor3      ",
	" 4.Step Motor4      ",
	" 5.Step Motor5      ",
	" 6.Step Motor6      ", 
};

void setup()
{
	Serial.begin(9600);
	pinMode(CS, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
//----------------------------------------------
	pinMode(ENC_A, INPUT);
	pinMode(ENC_B, INPUT);

	digitalWrite(ENC_A, HIGH);
	digitalWrite(ENC_B, HIGH);
//----------------------------------------------
	gLCD_init();
//----------------------------------------------
	for(int i = 0; i < 4; i++){
		cursor_set(LINE_1 + i, 0);
		printf_("%s", main_menu[i]);
	}
}

char enc_state = 0;
char enc_state_prev = 0;
char left_flag = 0;
char right_flag = 0;

void loop()
{
	encoder_func();
	delay(1);
}

// void loop()
// {
// 	cursor_set(LINE_1, 0);
// 	sprintf(buf, "count = %d", count++);
// 	gLCD_string(buf);
// 	printf_("count = %d", count++);
// 	delay(1000);
// }

void gLCD_string(char* str){
	for(int i = 0; i < strlen(str); i++){
		gLCD_data(*(str+i));
	}
}

void cursor_set(char line, char column){
	if(line == LINE_1) gLCD_inst(0x80 + column);
	else if(line == LINE_2) gLCD_inst(0x80 + 16 + column);
	else if(line == LINE_3) gLCD_inst(0xd0+ + column);
	else if(line == LINE_4) gLCD_inst(0xd0 + 16 + column);
}

void encoder_func(){
	int enc_a = digitalRead(ENC_A);
	int enc_b = digitalRead(ENC_B);

	if(enc_a==0 && enc_b == 1) enc_state = ENC_STATE_1;
	else if(enc_a == 0 && enc_b == 0) enc_state = ENC_STATE_2;
	else if(enc_a == 1 && enc_b == 0) enc_state = ENC_STATE_3;
	else if(enc_a == 1 && enc_b == 1){
		enc_state = ENC_STATE_4;
		left_flag = 0;
		right_flag = 0;
	}

	if(enc_state == ENC_STATE_3 && enc_state_prev == ENC_STATE_2){
		if(right_flag == 0){
			right_flag = 1;
			// Serial.println("R");
			cursor_set(LINE_1, 0);
			printf_("count = %3d", ++count);
		}
	}
	else if(enc_state == ENC_STATE_1 && enc_state_prev == ENC_STATE_2){
		if(left_flag == 0){
			left_flag = 1;
			// Serial.println("L");
			cursor_set(LINE_2, 0);
			printf_("count = %3d", --count);
		}
	}

	enc_state_prev = enc_state;
}