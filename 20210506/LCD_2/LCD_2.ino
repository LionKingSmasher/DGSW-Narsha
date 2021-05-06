#include <stdarg.h>

#define RS  		  3
#define RW  		  4
#define E   		  5
#define DB0 		  6
#define DB1 		  7
#define DB2 		  8
#define DB3 		  9
#define DB4  		  10
#define DB5    		  11
#define DB6 		  12
#define DB7 		  13

#define LCD_BACKLIGHT 2


enum charLCD_LINE{
	LINE_1 = 1,
	LINE_2,
	LINE_3,
	LINE_4
};

enum{
	DISABLE = 0,
	ENABLE
};

enum {
	MAIN_MENU = 0,
	SUBMENU_1,
	SUBMENU_2,
	SUBMENU_3,
	SUBMENU_4,
};

int count = 0;
char buf[50];

char menu = MAIN_MENU;

char *lcd_menu[] = {
	">1.PWM Backlight",
	" 2.PWM LED      ",
	" 3.Servo Motor  ",
	" 4.ADC          "
};

char * lcd_submenu_backlight[] = { ">1.PWM BL On    ",
								   " 2.PWM BL Off   ",
								   " 3.BL On        ",
								   " 4.BL Off       ",
								   " 5.Back         ",
								   " 6.Hoem         "
								};

// #define NUM_SUB_MENU_BACKLIGHT 6

char num_submenum[] = {6, 5, 4, 3};

void setup()
{
	Serial.begin(9600);

	pinMode(LCD_BACKLIGHT, OUTPUT);
	digitalWrite(LCD_BACKLIGHT, HIGH);

	for(int i = 0; i < 11; i++){
		pinMode(RS + i, OUTPUT);
	}

	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);

	//-------------------------
	// Inst: 30, 30, 30, 38, 06, 0c, 80, 01
	charLCD_inst_8(0x30);
	charLCD_inst_8(0x30);
	charLCD_inst_8(0x30);
	charLCD_inst_8(0x38);
	charLCD_inst_8(0x06);
	charLCD_inst_8(0x0c);
	charLCD_inst_8(0x80);
	charLCD_inst_8(0x01);
	delay(2);
	//--------------------------

	cursor_set(LINE_1, 0);
	printf_lcd("%s", lcd_menu[0]);
	cursor_set(LINE_2, 0);
	printf_lcd("%s", lcd_menu[1]);
}

char line_pos = LINE_1;
char menu_pos = LINE_1;
char enter_pos = LINE_1;

unsigned long prev_micros = 0;
unsigned long curr_micros = 0;
unsigned long prev_millis = 0;
unsigned long curr_millis = 0;

int pwm_count = 0;
int pwm_duty = 1;

char toggle_flag_line1 = 0;
char toggle_flag_line2 = 0;

char backlight_pwm_enable = 0;

void loop()
{
	curr_micros = micros();
	curr_millis = millis();

	if(backlight_pwm_enable == 1){
		if(curr_micros - prev_micros > 100){
			prev_micros = curr_micros;

			pwm_count++;
			if(pwm_count == 100){
				pwm_count = 0;
				digitalWrite(LCD_BACKLIGHT, HIGH);
			}
			else if(pwm_count == pwm_duty){
				digitalWrite(LCD_BACKLIGHT, LOW);
			}
		}

		if(curr_millis - prev_millis > 50){
			prev_millis = curr_millis;
			pwm_duty++;
			if(pwm_duty == 99) pwm_duty = 1;
		}
	}
	else{
		digitalWrite(LCD_BACKLIGHT, HIGH);
	}

	if(Serial.available()){
		char in_key = Serial.read();
		if(in_key == 'w'){
			if(line_pos == LINE_2){
				line_pos = LINE_1;
				cursor_set(LINE_2, 0);
				printf_lcd(" ");
				cursor_set(LINE_1, 0);
				printf_lcd(">");
			}
			else{
				if(menu == MAIN_MENU){
					menu_pos--;
					if(menu_pos > 0){
						for(int i = 0; i < 2; i++){
							cursor_set(LINE_1 + i, 0);
							printf_lcd("%s", lcd_menu[(menu_pos - 1) + i]);	
						}
						cursor_set(LINE_1, 0);
						printf_lcd(">");
					}
					else{
						menu_pos = 1;
					}
				}
				else if(menu == SUBMENU_1){
					menu_pos--;
					if(menu_pos > 0){
						for(int i = 0; i < 2; i++){
							cursor_set(LINE_1 + i, 0);
							printf_lcd("%s", lcd_submenu_backlight[(menu_pos - 1) + i]);	
						}
						cursor_set(LINE_1, 0);
						printf_lcd(">");
					}
					else{
						menu_pos = 1;
					}
				}
			}
		}
		else if(in_key == 's'){
			// Serial.println("Down");
			if(line_pos == LINE_1){
				line_pos = LINE_2;
				cursor_set(LINE_1, 0);
				printf_lcd(" ");
				cursor_set(LINE_2, 0);
				printf_lcd(">");
			}
			else{
				if(menu == MAIN_MENU){
					menu_pos++;
					if(menu_pos < 4){
						for(int i = 0; i < 2; i++){
							cursor_set(LINE_1 + i, 0);
							printf_lcd("%s", lcd_menu[(menu_pos - 1) + i]);	
						}
						cursor_set(LINE_2, 0);
						printf_lcd(">");
					}
					else{
						menu_pos = 3;
					}
				}
				else if(menu == SUBMENU_1){
					menu_pos++;
					if(menu_pos < 6){
						for(int i = 0; i < 2; i++){
							cursor_set(LINE_1 + i, 0);
							printf_lcd("%s", lcd_submenu_backlight[(menu_pos - 1) + i]);	
						}
						cursor_set(LINE_2, 0);
						printf_lcd(">");
					}
					else{
						menu_pos = 5;
					}
				}
			}
		}
		else if(in_key == ' '){
			enter_pos = line_pos + menu_pos - 1;
			if(enter_pos == LINE_1){
				menu = SUBMENU_1;
				menu_pos = LINE_1;
				for(int i = 0; i < 2; i++){
					cursor_set(LINE_1+i, 0);
					printf_lcd("%s", lcd_submenu_backlight[(menu_pos - 1) + i]);
				}

				// if(toggle_flag_line1 == 0){
				// 	toggle_flag_line1 = 1;
				// 	backlight_pwm_enable = 1;
				// }
				// else {
				// 	toggle_flag_line1 = 0;
				// 	backlight_pwm_enable = 0;
				// }
			}
			// else if(enter_pos == LINE_2){
			// 	if(toggle_flag_line1 == 0){
			// 		toggle_flag_line1 = 1;
			// 		backlight_pwm_enable = DISABLE;
			// 	}
			// 	else {
			// 		toggle_flag_line1 = 0;
			// 		backlight_pwm_enable = DISABLE;
			// 	}			}
			// else if(enter_pos == LINE_3){
			// 	Serial.println("3");
			// }
			// else if(enter_pos == LINE_4){
			// 	Serial.println("4");
			// }
		}
	}
	// delay(2);
	// charLCD_inst_8(0xc0);
	// // charLCD_data_8(0x31);
	// printf_lcd("count: %d", count++);
	// delay(1000);
}

void charLCD_inst_8(unsigned char inst){
	digitalWrite(RS, LOW);

	for(int i = 0; i < 8; i++){
		if(inst & (0x01 << i)){
			digitalWrite(DB0 + i, HIGH);
		}
		else{
			digitalWrite(DB0 + i, LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);

	delayMicroseconds(50);
}

void charLCD_data_8(unsigned char data){
	for(int i = 0; i < 8; i++){
		if(data & (0x01 << i)){
			digitalWrite(DB0 + i, HIGH);
		}
		else{
			digitalWrite(DB0 + i, LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	delayMicroseconds(50);
}

void charLCD_string(char* str){
	for(int i = 0; i < strlen(str); i++){
		// charLCD_data_8(*(str+i));
		charLCD_data_8(str[i]);
	}
}

void printf_lcd(char *format, ...)
{
	char buf[512];
	va_list arglist;

	va_start(arglist, format);
	vsprintf(buf, format, arglist);
	va_end(arglist);
	charLCD_string(buf);
}

void cursor_set(char line, char column){
	if(line == LINE_1){
		charLCD_inst_8(0x80 | column);
	}
	else if(line == LINE_2){
		charLCD_inst_8(0xc0 | column);
	}
}
