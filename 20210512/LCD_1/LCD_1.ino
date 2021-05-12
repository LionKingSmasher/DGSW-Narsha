#include <stdarg.h>

#define RS  		  3
#define RW  		  4
#define E   		  5
#define DB4  		  10
#define DB5    		  11
#define DB6 		  12
#define DB7 		  13

#define LCD_BACKLIGHT 2

void setup()
{
	pinMode(RS, OUTPUT);
	pinMode(RW, OUTPUT);
	pinMode(E,  OUTPUT);

	for(int i = 0; i < 4; i++)
		pinMode(DB4 + i, OUTPUT);

	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);
//-----------initialization------------
	charLCD_inst_8bit(0x30);
	charLCD_inst_8bit(0x30);
	charLCD_inst_8bit(0x30);
	charLCD_inst_8bit(0x28);
	charLCD_inst(0x28);
	charLCD_inst(0x06);
	charLCD_inst(0x0c);
	charLCD_inst(0x80);
	charLCD_inst(0x01);
	delay(2);

}

void loop()
{
	charLCD_data(0x31);
	delay(1000);
}

void charLCD_inst_4bit(unsigned char inst){
	digitalWrite(RS, LOW);

	for(int i = 0; i < 4; i++){
		if(inst & (0x01 << i)){
			digitalWrite(DB4+i, HIGH);
		}
		else{
			digitalWrite(DB4+i, LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);

	delayMicroseconds(50);
}

void charLCD_inst(unsigned char inst){
	char upper_byte = (inst & 0xf0) >> 4;
	char lower_byte = (inst & 0x0f);

	charLCD_inst_4bit(upper_byte);
	charLCD_inst_4bit(lower_byte);
}

void charLCD_data_4bit(unsigned char data){

	for(int i = 0; i < 4; i++){
		if(data & (0x01 << i)){
			digitalWrite(DB4+i, HIGH);
		}
		else{
			digitalWrite(DB4+i, LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	delayMicroseconds(50);
}

void charLCD_inst_8bit(unsigned char inst){
	digitalWrite(RS, LOW);

	for(int i = 0; i < 4; i++){
		if(inst & (0x10 << i)) digitalWrite(DB4+i, HIGH);
		else digitalWrite(DB4+i, LOW);
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);

	delayMicroseconds(50);
}

void charLCD_data(unsigned char data){
	char upper_byte = (data & 0xf0) >> 4;
	char lower_byte = (data & 0x0f);

	charLCD_data_4bit(upper_byte);
	charLCD_data_4bit(lower_byte);
}