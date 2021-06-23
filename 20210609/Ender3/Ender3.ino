#include "gcode_sample_480.h"
//#include "gcode_sample_200_string.h
#include "gcode_sample_300_string.h"

#define X_DIR  21
#define X_STEP 15
#define X_STOP 18

#define Y_STOP 17
#define Y_DIR  23
#define Y_STEP 22

#define XY_EN  14
#define NUM_GCODE 278

enum {
	STOPPED = 0,
	RUNNING
};

enum {
	DISABLE = 0,
	ENABLE
};

enum dir {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};

char enable_motor_x = 0;
char enable_motor_y = 0;

int count_distance_x = 0;
int count_distance_y = 0;

int half_period_x = 0;
int half_period_y = 0;

int step_count_x = 0;
int step_count_y = 0;

double curr_x = 0;
double curr_y = 0;

char toggle_step_x = 0;
char toggle_step_y = 0;

char interrupt_stopped_x = 0;
char interrupt_stopped_y = 0;

int state = 1;

void motor_direction(char dir){
	if(dir == LEFT){
		digitalWrite(X_DIR, HIGH);
	}
	else if(dir == RIGHT){
		digitalWrite(X_DIR, LOW);
	}
	else if(dir == UP){
		digitalWrite(Y_DIR, LOW);
	}
	else if(dir == DOWN){
		digitalWrite(Y_DIR, HIGH);
	}
}

void timer_x_disable(){
	TIMSK1 = 0x00;
}

void timer_x_enable(){
	TIMSK1 = 0x02;
}

void timer_y_disable(){
	TIMSK3 = 0x00;
}

void timer_y_enable(){
	TIMSK3 = 0x02;
}

// void motor_x_start(){
// 	enable_motor_x = DISABLE;
// 	enable_motor_y = ENABLE;

// 	interrupt_stopped_x = STOPPED;
// 	interrupt_stopped_y = RUNNING;

// 	count_distance_x = 0;
// 	count_distance_y = (int)(25 * 80);

// 	half_period_x = 0;
// 	half_period_y = (int)(200 * 1.732);

// 	OCR1A = half_period_x * 2;
// 	OCR3A = half_period_y * 2;

// 	motor_direction(DOWN);
// }

void motor_x_start(float distance, int step_period, char left_right){
	interrupt_stopped_x = RUNNING;
	count_distance_x = (int)(distance * 80);
	half_period_x = step_period;
	OCR1A = half_period_x * 2;
	TCNT1 = 0;
	motor_direction(left_right);
}

void motor_y_start(float distance, int step_period, char up_down){
	interrupt_stopped_y = RUNNING;
	count_distance_y = (int)(distance * 80);
	half_period_y = step_period;
	OCR3A = half_period_y * 2;
	TCNT3 = 0;
	motor_direction(up_down);
}

void motor_x_y_enable(char x_enable, char y_enable){
	if(x_enable == ENABLE) 
		timer_x_enable();
	if(y_enable == ENABLE)
		timer_y_enable();

	enable_motor_x = x_enable;
	enable_motor_y = y_enable;
}

void motor_init(){
	motor_x_start(43.3, 200, RIGHT);
	motor_y_start(25, 200 * 1.732, UP);
	motor_x_y_enable(ENABLE, ENABLE);
	state = 1;
}

double prev_x = 0;
double prev_y = 0;
int prev_speed = 0;

void motor_move_1(double x, double y, int speed){
	char dir_x;
	char dir_y;

	char move_enable_x;
	char move_enable_y;

	double x_dist = x - prev_x;
	double y_dist = y - prev_y;

	// Serial.println("start_x: " + String(start_x));
	// Serial.println("end_x: " + String(end_x));

	//------------------------------------
	if(x_dist > 0) dir_x = RIGHT;
	else if(x_dist < 0) dir_x = LEFT;

	if(y_dist > 0) dir_y = UP;
	else if(y_dist < 0) dir_y = DOWN;
	//------------------------------------
	if(x_dist == 0) move_enable_x = DISABLE;
	else move_enable_x = ENABLE;

	if(y_dist == 0) move_enable_y = DISABLE;
	else move_enable_y = ENABLE;
	//------------------------------------
	
	int period_x = 200;
	int period_y = 200;

	if(speed == 0) speed = prev_speed;
	double half_period = (1000000 / (((double)speed / 60.0)*80.0))/2;
	// Serial.println(half_period);

	double theta = atan2(y_dist, x_dist);
	// Serial.println(theta);

	if(y_dist != 0 && x_dist != 0)
		period_y = (int)(200.0 * (x_dist/y_dist));

	period_x = abs(period_x);
	period_y = abs(period_y);

	x_dist = abs(x_dist);
	y_dist = abs(y_dist);

	if(x_dist != 0)
	{
		period_x = half_period * (1/cos(theta));
		motor_x_start((float)x_dist, abs(period_x), dir_x);
	}
	if(y_dist != 0)
	{
		period_y = half_period * (1/sin(theta));
		motor_y_start((float)y_dist, abs(period_y), dir_y);
	}

	motor_x_y_enable(move_enable_x, move_enable_y);
	prev_x = x;
	prev_y = y;
	prev_speed = speed;
	// Serial.println("x_dist: " + String(x_dist));
	// Serial.println("y_dist: " + String(y_dist));
	// while(enable_motor_x==ENABLE || enable_motor_y==ENABLE);
}

void motor_move(double start_x, double start_y, double end_x, double end_y, int speed){
	char dir_x;
	char dir_y;

	char move_enable_x;
	char move_enable_y;

	double x_dist = end_x - start_x;
	double y_dist = end_y - start_y;

	// Serial.println("start_x: " + String(start_x));
	// Serial.println("end_x: " + String(end_x));

	//------------------------------------
	if(x_dist > 0) dir_x = RIGHT;
	else if(x_dist < 0) dir_x = LEFT;

	if(y_dist > 0) dir_y = UP;
	else if(y_dist < 0) dir_y = DOWN;
	//------------------------------------
	if(x_dist == 0) move_enable_x = DISABLE;
	else move_enable_x = ENABLE;

	if(y_dist == 0) move_enable_y = DISABLE;
	else move_enable_y = ENABLE;
	//------------------------------------
	
	int period_x = 200;
	int period_y = 200;

	double half_period = (1000000 / (((double)speed / 60.0)*80.0))/2;
	// Serial.println(half_period);

	double theta = atan2(y_dist, x_dist);
	// Serial.println(theta);

	if(y_dist != 0 && x_dist != 0)
		period_y = (int)(200.0 * (x_dist/y_dist));

	period_x = abs(period_x);
	period_y = abs(period_y);

	x_dist = abs(x_dist);
	y_dist = abs(y_dist);

	if(x_dist != 0)
	{
		period_x = half_period * (1/cos(theta));
		motor_x_start((float)x_dist, abs(period_x), dir_x);
	}
	if(y_dist != 0)
	{
		period_y = half_period * (1/sin(theta));
		motor_y_start((float)y_dist, abs(period_y), dir_y);
	}

	motor_x_y_enable(move_enable_x, move_enable_y);
	// Serial.println("x_dist: " + String(x_dist));
	// Serial.println("y_dist: " + String(y_dist));
	// while(enable_motor_x==ENABLE || enable_motor_y==ENABLE);
}

char x_value_string[10];
char y_value_string[10];
char e_value_string[10];
char f_value_string[10];
int x_value_index = 0;
int y_value_index = 0;
int e_value_index = 0;
int f_value_index = 0;

double xy_value[NUM_GCODE][2];
double f_value[NUM_GCODE];
int e_value[NUM_GCODE];

enum {
	X_BUFFFER = 0,
	Y_BUFFER,
	E_BUFFER,
	F_BUFFER
};

char xyef_string[4][10];
int buffer_index = 0;

char which_buffer = 0;
double xyef_value[NUM_GCODE][4];

char initial_value[4] = {'X', 'Y', 'E', 'F'};

void gcode_parsing()
{
	for(int j = 0; j < NUM_GCODE; j++)
	{
		int one_code_length = strlen(xy_pos_string[j]);
		buffer_index = 0;

		for(int k = 0; k < 4; k++){
			for(int i = 0; i < 10; i++){
				xyef_string[k][i] = 0;
			}
		}
		for(int i = 0; i < 10; i++)
		{
			x_value_string[i] = 0;
			y_value_string[i] = 0;
			e_value_string[i] = 0;
			f_value_string[i] = 0;
		}
		if(*(xy_pos_string[j]+0)=='G' && *(xy_pos_string[j]+1)=='1')
		{
			for(int i = 2; i < one_code_length; i++)
			{
				for(int m = 0; m < 4; m++){
					if(*(xy_pos_string[j] + i) == initial_value[m]){
						which_buffer = m;
						buffer_index = 0;
						break;
					}
				}
				if(*(xy_pos_string[j]+i) != ' ')
					xyef_string[which_buffer][buffer_index++] = *(xy_pos_string[j]+i);
			}
			for(int i = 0; i < 4; i++){
				xyef_value[j][i] = atof(&xyef_string[i][1]);
			}
			// xy_value[j][0] = atof(&x_value_string[1]);
			// xy_value[j][1] = atof(&y_value_string[1]);
			// f_value[j] = atoi(&f_value_string[1]);
			// if(f_value[j] == 0){
		 //      f_value[j] = f_value[j-1];
		 //    }
		}
	    // Serial.println(xy_value[j][0]);
	    // Serial.println(xy_value[j][1]);
	    // Serial.println(f_value[j]);
	}
}

void setup()
{
	Serial.begin(9600);
	pinMode(X_DIR, OUTPUT);
	pinMode(X_STEP, OUTPUT);

	pinMode(Y_DIR, OUTPUT);
	pinMode(Y_STEP, OUTPUT);

	pinMode(XY_EN, OUTPUT);

	//Timer 1 : X Motor
	TCCR1A = 0x00; //CTC 모드
	TCCR1B = 0x0a; //발동 하고, 8분주기
	TCCR1C = 0x00;
	TCNT1 = 0x00;
	OCR1A = 400;
	TIMSK1 = 0x02;

	//Timer 3 : Y Motor
	TCCR3A = 0x00; //CTC 모드
	TCCR3B = 0x0a; //발동 하고, 8분주기
	TCCR3C = 0x00;
	TCNT3 = 0x00;
	OCR3A = 400;
	TIMSK3 = 0x02;

	digitalWrite(XY_EN, LOW);
	// motor_direction(RIGHT);
	// motor_direction(UP);

	// interrupt_stopped_x = RUNNING;
	// interrupt_stopped_y = RUNNING;

	// enable_motor_x = ENABLE;
	// enable_motor_y = ENABLE;

	// count_distance_x = (int)(43.3 * 80);
	// count_distance_y = (int)(25 * 80);

	// half_period_x = 200;
	// half_period_y = (int)(200 * 1.732);

	// OCR1A = half_period_x * 2;
	// OCR3A = half_period_y * 2;

	// timer_x_enable();
	// timer_y_enable();

	// motor_init();
	// Serial.println("first done");
	// motor_move(0, 0, 50, 30);
	// Serial.println("second done");
	// motor_x_y_enable(ENABLE, ENABLE);
	gcode_parsing();
	state = 0;
}

//ISR Routine
SIGNAL(TIMER1_COMPA_vect){
	if(enable_motor_x == ENABLE){
		if(toggle_step_x == 0){
			toggle_step_x = 1;
			digitalWrite(X_STEP, HIGH);
		}
		else{
			toggle_step_x = 0;
			step_count_x++;
			if(step_count_x > count_distance_x) {
				step_count_x = 0;
				digitalWrite(X_STEP, LOW);
				enable_motor_x = DISABLE;
				interrupt_stopped_x = STOPPED;
			}
			else 
				digitalWrite(X_STEP, LOW);
		}
	}
}

SIGNAL(TIMER3_COMPA_vect){
	if(enable_motor_y == ENABLE){
		if(toggle_step_y == 0){
			toggle_step_y = 1;
			digitalWrite(Y_STEP, HIGH);
		}
		else{
			toggle_step_y = 0;
			step_count_y++;
			if(step_count_y > count_distance_y) {
				step_count_y = 0;
				digitalWrite(Y_STEP, LOW);
				enable_motor_y = DISABLE;
				interrupt_stopped_y = STOPPED;
			}
			else 
				digitalWrite(Y_STEP, LOW);
		}
	}
}

// double gcode_array[4][4] = {
// 	{0, 0, 50, 0},
// 	{50, 0, 50, 50},
// 	{50, 50, 0, 50},
// 	{0, 50, 0, 0},
// };

double gcode_array[4][2] = {
	{50, 0},
	{50, 50},
	{0, 50},
	{0, 0},
};

int pre_state = 0;
int gcode_index = 0;

void loop()
{
	// Serial.println(half_period_y);
	// delay(1000);

	while(1)
	{
		if(interrupt_stopped_x == STOPPED){
		  	timer_x_disable();
		}
		if(interrupt_stopped_y == STOPPED){
		  	timer_y_disable();
		}
		if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED)
		{
			motor_move_1(xyef_value[gcode_index][0], xyef_value[gcode_index][1], xyef_value[gcode_index][3]);
			if(gcode_index++ == NUM_GCODE) while(1);
			// Serial.println(String(gcode_index));
		}
	}

	
	// if(state == 1){
	// 		if(interrupt_stopped_x == STOPPED){
	// 		  	timer_x_disable();
	// 		}
	// 		if(interrupt_stopped_y == STOPPED){
	// 		  	timer_y_disable();
	// 		}
	// 		if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED){
	// 			motor_move(0, 0, 50, 0);
	// 			state = 2;
	// 		}

	// 	}
	// 	else if(state == 2){
	// 		if(interrupt_stopped_x == STOPPED){
	// 			timer_x_disable();
	// 		}
	// 		if(interrupt_stopped_y == STOPPED){
	// 			timer_y_disable();
	// 		}
	// 		if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED){
	// 			motor_move(50, 0, 50, 50);
	// 			state = 3;

	// 			// enable_motor_x = ENABLE;
	// 			// enable_motor_y = DISABLE;

	// 			// interrupt_stopped_x = RUNNING;
	// 			// interrupt_stopped_y = STOPPED;

	// 			// count_distance_x = (int)(43.3 * 80);
	// 			// count_distance_y = 0;

	// 			// half_period_x = 200;
	// 			// half_period_y = 0;

	// 			// OCR1A = half_period_x * 2;
	// 			// OCR3A = half_period_y * 2;

	// 			// motor_direction(LEFT);

	// 			// timer_x_enable();

	// 			// enable_motor_x = ENABLE;
	// 			// enable_motor_y = DISABLE;
	// 		}
	// 	}
	// 	else if(state == 3){
	// 		if(interrupt_stopped_x == STOPPED){
	// 			timer_x_disable();
	// 		}
	// 		if(interrupt_stopped_y == STOPPED){
	// 			timer_y_disable();
	// 		}
	// 		if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED){
	// 			motor_move(50, 50, 0, 50);
	// 			state = 4;
	// 		}
	// 	}
	// 	else if(state == 4){
	// 		if(interrupt_stopped_x == STOPPED){
	// 			timer_x_disable();
	// 		}
	// 		if(interrupt_stopped_y == STOPPED){
	// 			timer_y_disable();
	// 		}
	// 		if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED){
	// 			motor_move(0, 50, 0, 0);
	// 			state = 1;
	// 		}
	// 	}
}