#include <stdarg.h>

#define CS      28
#define SCK     30
#define MOSI    17

#define ENC_A   10
#define ENC_B   11

#define ENC_BTN 16

#define HOTBED  12

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

enum curr_menu {
	MAIN_MENU = 0,
	SUB_MENU_1,
	SUB_MENU_2,
	SUB_MENU_3,
	SUB_MENU_4,
	SUB_MENU_5,
	SUB_MENU_6
};


enum{
	HOME = 4,
	BACK
};


char current_menu = MAIN_MENU;

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


char cursor_pos = 0;
int prev_cursor_pos = 0;
char buf[50] = {0,};

char *main_menu[] = {
	" 1.Step Motor1   ",
	" 2.Step Motor2   ",
	" 3.Step Motor3   ",
	" 4.Step Motor4   ",
	" 5.Step Motor5   ",
	" 6.Step Motor6   ", 
};

char *sub_menu1[2][6] = {
	{
		" 1.sub_menu 1 1  ",
		" 2.sub_menu 2 1  ",
		" 3.sub_menu 3 1  ",
		" 4.sub_menu 4 1  ",
		" 5.HOME          ",
		" 6.BACK          ",
	},
	{
		" 1.Bed Temp      ",
		" 2.sub_menu 2 2  ",
		" 3.sub_menu 3 2  ",
		" 4.sub_menu 4 2  ",
		" 5.HOME          ",
		" 6.BACK          ",
	}
};

char *sub_menu2[] = {
	">1.sub_menu 1 2  ",
	" 2.sub_menu 2 2  ",
	" 3.sub_menu 3 2  ",
	" 4.sub_menu 4 2  ",
};

void print_all_menu(char window_pos, char cursor_pos){
	for(int i = 0; i < 4; i++){
		cursor_set(LINE_1 + i, 0);
		printf_("%s", main_menu[i + window_pos]);
	}

	cursor_set(LINE_1 + cursor_pos, 0);
	gLCD_data('>');
}

void print_sub_menu(char current_menu, char window_pos, char cursor_pos){
	for(int i = 0; i < 4; i++){
		cursor_set(LINE_1 + i, 0);
		printf_("%s", sub_menu1[current_menu - 1][i + window_pos]);
	}

	cursor_set(LINE_1 + cursor_pos, 0);
	gLCD_data('>');
}


void setup()
{
	Serial.begin(9600);
	pinMode(CS, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(ENC_BTN, INPUT);
//----------------------------------------------
	pinMode(ENC_A, INPUT);
	pinMode(ENC_B, INPUT);

	digitalWrite(ENC_A, HIGH);
	digitalWrite(ENC_B, HIGH);

	digitalWrite(ENC_BTN, HIGH); // Pull up
//----------------------------------------------
	pinMode(HOTBED, OUTPUT);
	digitalWrite(HOTBED, HIGH);
	// while(1){
	// 	int hot_bad_value = analogRead(A6);
	// 	Serial.println(hot_bad_value);
	// 	delay(100);
	// }
//----------------------------------------------
	gLCD_init();
//----------------------------------------------
	print_all_menu(0, 0);
}

char enc_state = 0;
char enc_state_prev = 0;
char left_flag = 0;
char right_flag = 0;
char enter_flag = 0;
char btn_flag = 0;
char bed_temp_enable = 0;
char temp_count = 0;

void loop()
{
	encoder_func();
	if(bed_temp_enable == 1){
		temp_count++;
		if(temp_count > 1000){
			temp_count = 0;
			int hot_bad_value =  analogRead(A6);
			if(hot_bad_value > 910){
				digitalWrite(HOTBED, LOW);
			}
			else {
				digitalWrite(HOTBED, HIGH);
				cursor_set(LINE_1, 0);
				printf_("Bed Temp = %d", hot_bad_value);
			}
		}
	}
	Serial.println((int)bed_temp_enable);
	delay(1);
}

// void loop()
// {
// 	cursor_set(LINE_1, 0);
// 	sprintf(buf, "cursor_pos = %d", cursor_pos++);
// 	gLCD_string(buf);
// 	printf_("cursor_pos = %d", cursor_pos++);
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
	else if(line == LINE_3) gLCD_inst(0x80 + 8 + column);
	else if(line == LINE_4) gLCD_inst(0x80 + 24 + column);
}

char window_pos = 0;
char enter_pos = 0;

void encoder_func(){

	int enc_a = digitalRead(ENC_A);
	int enc_b = digitalRead(ENC_B);
	int enc_btn = digitalRead(ENC_BTN);

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
			cursor_set(LINE_1+cursor_pos, 0);
			printf_(" ");
			cursor_pos++;
			if(cursor_pos < 4){
				cursor_set(LINE_1 + cursor_pos, 0);
				gLCD_data('>');
			}
			else if(cursor_pos >= 4){
				cursor_pos = 3;
				window_pos++;
				if(window_pos > 2) {
					window_pos = 2;
					cursor_set(LINE_1 + cursor_pos, 0);
					gLCD_data('>');
				}
				else {
					if(current_menu == MAIN_MENU){
						print_all_menu(window_pos, cursor_pos);
					}
					else{
						print_sub_menu(current_menu, window_pos, cursor_pos);
					}
				}
			}
		}
	}
	else if(enc_state == ENC_STATE_1 && enc_state_prev == ENC_STATE_2){
		if(left_flag == 0){
			left_flag = 1;
			// Serial.println("L");
			cursor_set(LINE_1+cursor_pos, 0);
			printf_(" ");
			cursor_pos--;
			if(cursor_pos >= 0){
				cursor_set(LINE_1 + cursor_pos, 0);
				gLCD_data('>');
			}
			else if(cursor_pos < 0){
				cursor_pos = 0;
				window_pos--;
				if(window_pos < 0) {
					window_pos = 0;
					cursor_set(LINE_1 + cursor_pos, 0);
					gLCD_data('>');
				}
				else{
					if(current_menu == MAIN_MENU) 
						print_all_menu(window_pos, cursor_pos);
					else{
						print_sub_menu(current_menu, window_pos, cursor_pos);
					}
				} 
			}
		}
	}

	enter_pos = window_pos + cursor_pos;

	if(enc_btn == 0){
		// sub_menu_function();
		//Serial.println((int)enter_pos);
		if(btn_flag == 0) {
			btn_flag = 1;
			enter_func();
		}
	}
	else{
		btn_flag = 0;
	}

	// cursor_pos = (cursor_pos < 0) ? 0 :
	// 			 (cursor_pos > 3) ? 3 :
	// 			 cursor_pos; 

	// cursor_set(LINE_1+cursor_pos, 0);
	// printf_(">");

	enc_state_prev = enc_state;
}

void clear_lcd(){
	for(int i = 0; i < 4; i++){
		cursor_set(LINE_1 + i, 0);
		printf_("               ");
	}
}

char sub_menu1_flag = 0;

void enter_func(){
	if(current_menu != MAIN_MENU && enter_pos == HOME){
		current_menu = MAIN_MENU;
		enter_pos = 0;
		cursor_pos = 0;
		window_pos = 0;
		print_all_menu(window_pos, cursor_pos);
	}
	else if(current_menu != MAIN_MENU && enter_pos == BACK){

	}
	else if(current_menu == SUB_MENU_1){
		if(enter_pos == 0){
			if(bed_temp_enable == 0){
				clear_lcd();
				bed_temp_enable = 1;
			}
		}
	}
	else{
		current_menu = enter_pos + 1;
		enter_pos = 0;
		cursor_pos = 0;
		window_pos = 0;
		print_sub_menu(current_menu, window_pos, cursor_pos);
		if(current_menu == SUB_MENU_1) sub_menu1_flag = 1; 
	}
}