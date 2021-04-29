#define ENC_A      10 // Right
#define ENC_B      11 // Left
#define ENC_BUTTON 16

#define X_DIR      21
#define X_STEP     15
#define X_STOP     18

#define Y_DIR      23
#define Y_STEP     22
#define Y_STOP	   18

#define XY_EN      14

int value_enc_a = 0;
int value_enc_b = 0;
int curr_enc_state = 0;
int prev_enc_state = 0;

int before_status = 4;

int right_flag = 0;
int left_flag = 0;

int count = 0;

enum enc_state {
	ENC_STATE_1 = 1,
	ENC_STATE_2,
	ENC_STATE_3,
	ENC_STATE_4
};

enum enc_state state_status;

enum MoveStatus{
	NONE = 0,
	LEFT,
	RIGHT
};

enum RunStop {
	STOP = 0,
	RUN
};

enum direction{
	DIR_NONE = 0,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN
};

enum menu_pos {
	MAIN_LEFT_STEP = 0,
	MAIN_RIGHT_STEP,
	MAIN_UP_STEP,
	MAIN_DOWN_STEP,
	MAIN_SPEED_0_1,
	MAIN_SPEED_1,
	MAIN_SPEED_10,
	MAIN_SPEED_SET
};


int x_stop_value = 0;
int y_stop_value = 0;
int run_stop_x = STOP;
int run_stop_y = STOP;
int num_of_step_count_x = 0;
int num_of_step_count_y = 0;
double distance = 0;
char menu_position = 0;

void main_menu(){
	Serial.println("1. left step");
	Serial.println("2. right step");
	Serial.println("3. speed 0.1");
	Serial.println("4. speed 1");
	Serial.println("5. speed 10");
	Serial.println("6. speed set");
	Serial.println(">");
}

void direction_x(char dir){
	if(dir == DIR_LEFT){
		digitalWrite(X_DIR, HIGH);
	}
	else if (dir == DIR_RIGHT){
		digitalWrite(X_DIR, LOW);
	}
}

void direction_y(char dir){
	if(dir == DIR_UP){
		digitalWrite(Y_DIR, HIGH);
	}
	else if (dir == DIR_DOWN){
		digitalWrite(Y_DIR, LOW);
	}
}

void setup()
{
	Serial.begin(9600);
	
	pinMode(10, INPUT);
	pinMode(11, INPUT);

	// Hadware(Ender 3) part doesn't have pull up resistor :(
	// So we make virtual pull up resistence on software 
	digitalWrite(ENC_A, HIGH);
	digitalWrite(ENC_B, HIGH);

	pinMode(X_DIR, OUTPUT);
	pinMode(Y_DIR, OUTPUT);

	pinMode(X_STEP, OUTPUT);
	pinMode(Y_STEP, OUTPUT);

	pinMode(X_STOP, INPUT);
	pinMode(Y_STOP, INPUT);

	pinMode(XY_EN, OUTPUT);

	// Timer 1 Setting Complete!
	// Timer 1 -> X
	TCCR1A = 0x00;
	TCCR1B = 0x0A;
	TCCR1C = 0x00;
	TCNT1 = 0;
	OCR1A = 400;
	TIMSK1 = 0x02;

	// Timer 2
	TCCR3A = 0x00;
	TCCR3B = 0x0A;
	TCCR3C = 0x00;
	TCNT3 = 0;
	OCR3A = 400;
	TIMSK3 = 0x02;


	digitalWrite(XY_EN, LOW);
	digitalWrite(X_DIR, HIGH);

	// init value
	distance = 1;
	num_of_step_count_x = distance * 80;
	run_stop_x = RUN;

	main_menu();
}

// 1. left step
// 2. right step
// 3. speed 0.1
// 4. speed 1
// 5. speed 10
void loop()
{
	if(Serial.available()){
		char key_value = Serial.read();
		if(key_value == '1'){
			menu_position = MAIN_LEFT_STEP;

			// direction_x(DIR_LEFT);
			// num_of_step_count_x = distance * 80;
			// run_stop_x = RUN;
		}
		else if (key_value == '2'){
			menu_position = MAIN_RIGHT_STEP;
			// // Serial.println("2");
			// direction_x(DIR_RIGHT);
			// num_of_step_count_x = distance * 80;
			// run_stop_x = RUN;
		}
		else if (key_value == '3'){
			menu_position = MAIN_SPEED_0_1;
			distance = 0.1;
		}
		else if (key_value == '4'){
			menu_position = MAIN_SPEED_1;
			distance = 1;
		}
		else if (key_value == '5'){
			menu_position = MAIN_SPEED_10;
			distance = 10;
		}
		else if (key_value == '6'){
			menu_position = MAIN_SPEED_SET;
		}
	}
	// while(1){
	// 	x_stop_value = digitalRead(X_STOP);
	// 	Serial.println(x_stop_value);
	// }
	x_stop_value = digitalRead(X_STOP);
	value_enc_a = digitalRead(ENC_A);
	value_enc_b = digitalRead(ENC_B);
	if(value_enc_a == 0 && value_enc_b == 1){
		state_status = ENC_STATE_1;
	}
	else if(value_enc_a == 0 && value_enc_b == 0){
		state_status = ENC_STATE_2;
	}
	else if(value_enc_a == 1 && value_enc_b == 0){
		state_status = ENC_STATE_3;
	}
	else if(value_enc_a == 1 && value_enc_b == 1){
		state_status = ENC_STATE_4;
		right_flag = 0;
		left_flag = 0;
	}
	if(state_status == ENC_STATE_3 && prev_enc_state == ENC_STATE_2)
	{
		if(right_flag == 0){
			right_flag = 1;
			if(menu_position == MAIN_RIGHT_STEP){
				// distance = 7;
				num_of_step_count_x = (int)(distance * 80);
				run_stop_x = RUN;
			}
			else if(menu_position == MAIN_UP_STEP){
				num_of_step_count_y = (int)(distance * 80);
				run_stop_y = RUN;
			}
			else if(menu_position == MAIN_SPEED_SET){
				distance += 0.2;
				Serial.println("distance = " + String(distance));
			}
			// digitalWrite(X_DIR, LOW);
			// Serial.println(++count);
		}
	}
	else if (state_status == ENC_STATE_1 && prev_enc_state == ENC_STATE_2){
		if(left_flag == 0){
			left_flag = 1;
			if(menu_position == MAIN_LEFT_STEP){
				// distance = 7;
				num_of_step_count_x = (int)(distance * 80);
				run_stop_x = RUN;
			}
			else if(menu_position == MAIN_DOWN_STEP){
				num_of_step_count_y = (int)(distance * 80);
				run_stop_y = RUN;
			}
			// digitalWrite(X_DIR, HIGH);
			// Serial.println(--count);
			else if(menu_position == MAIN_SPEED_SET){
				distance -= 0.2;
				Serial.println("distance = " + String(distance));
			}
		}
	}

	prev_enc_state = state_status;
	delay(1);
}

char toggle_flag_x = 0;
char toggle_flag_y = 0;

int step_count_x = 0;
int step_count_y = 0;

SIGNAL(TIMER1_COMPA_vect){ // x interrupt
	if(x_stop_value == 0 && run_stop_x == RUN){
		if(toggle_flag_x == 0){
			toggle_flag_x = 1;
			digitalWrite(X_STEP, HIGH);
		}
		else{
			toggle_flag_x = 0;
			digitalWrite(X_STEP, LOW);
			step_count_x++;
			if(step_count_x >= num_of_step_count_x){
				step_count_x = 0;
				run_stop_x = STOP;
			}
			// run_stop = STOP;
		}
	}
}

SIGNAL(TIMER3_COMPA_vect){ //y interrupt
	if(y_stop_value == 0 && run_stop_y == RUN){
		if(toggle_flag_y == 0){
			toggle_flag_y = 1;
			digitalWrite(Y_STEP, HIGH);
		}
		else{
			toggle_flag_y = 0;
			digitalWrite(Y_STEP, LOW);
			step_count_y++;
			if(step_count_y >= num_of_step_count_y){
				step_count_y = 0;
				run_stop_y = STOP;
			}
			// run_stop = STOP;
		}
	}
}