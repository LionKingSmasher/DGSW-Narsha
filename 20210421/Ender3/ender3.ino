#define X_DIR     21
#define X_STEP    15
#define XY_ENABLE 14
#define X_STOP	  18

#define Y_DIR     23
#define Y_STEP    22
#define Y_STOP    19

enum {
	STOP = 0,
	RUN,
};

enum{
	DIR_LEFT = 0,
	DIR_RIGHT,
};

enum{
	DIR_UP = 0,
	DIR_DOWN
};

enum{
	RUNNING = 0,
	STOPPED
};

enum{
	STATE_RIGHT = 0,
	STATE_LEFT,
	STATE_UP,
	STATE_DOWN,
	STATE_CROSS,
	STATE_NONE
};

int step_count_x = 0;
int stop_run_flag_x = RUN;
int howManyCounts_x = 0;
int dist_x = 0;
char x_stopped = STOPPED;

int step_count_y = 0;
int stop_run_flag_y = RUN;
int howManyCounts_y = 0;
int dist_y = 0;
char y_stopped = STOPPED;

int motor_state = STATE_NONE;

void x_direction(char dir){
	if(dir == DIR_LEFT) digitalWrite(X_DIR, HIGH);
	else if(dir == DIR_RIGHT) digitalWrite(X_DIR, LOW);
}

void y_direction(char dir){
	if(dir == DIR_UP) digitalWrite(Y_DIR, LOW);
	else if(dir == DIR_DOWN) digitalWrite(Y_DIR, HIGH);
}

void init_func(){
	dist_x = 50;
	howManyCounts_x = dist_x * 80;
	
	dist_y = 50;
	howManyCounts_y = dist_y * 80;

	stop_run_flag_x = RUN;
	stop_run_flag_y = STOP;

	x_stopped = RUNNING;
	y_stopped = RUNNING;

	motor_state = STATE_RIGHT;

	x_direction(DIR_RIGHT);
	y_direction(DIR_DOWN);

	stop_run_flag_x = RUN;
	stop_run_flag_y = STOP;
}

void setup()
{
	Serial.begin(9600);

	pinMode(X_DIR, OUTPUT);
	pinMode(X_STEP, OUTPUT);
	pinMode(X_STOP, INPUT);

	pinMode(Y_DIR, OUTPUT);
	pinMode(Y_STEP, OUTPUT);
	pinMode(Y_STOP, INPUT);

	pinMode(XY_ENABLE, OUTPUT);

	digitalWrite(X_DIR, LOW);
	digitalWrite(XY_ENABLE, LOW);

	TCCR2A = 0x02;
	TCCR2B = 0x04; //CTC Mode
	TCNT2 = 0;
	OCR2A = 50;
	TIMSK2 = 0x02; //CTC Mode

	init_func();
}

void loop()
{

	if (motor_state == STATE_RIGHT){
		if(x_stopped == STOPPED){
			Serial.println("X Motor Stopped!!");
			
			motor_state = STATE_DOWN;

			y_direction(DIR_DOWN);
			y_stopped = RUNNING;
			stop_run_flag_y = RUN;
		}
	}
	else if(motor_state == STATE_DOWN){
		if(y_stopped == STOPPED) {
			Serial.println("Y Motor Stopped!!");

			motor_state = STATE_LEFT;

			x_direction(DIR_LEFT);
			x_stopped = RUNNING;
			stop_run_flag_x = RUN;
		}
	}
	else if(motor_state == STATE_LEFT){
		if(x_stopped == STOPPED){
			motor_state = STATE_UP;

			y_direction(DIR_UP);
			y_stopped = RUNNING;
			stop_run_flag_y = RUN;
		}
	}
	else if(motor_state == STATE_UP){
		if(y_stopped == STOPPED){
			motor_state = STATE_CROSS;

			x_direction(DIR_RIGHT);
			y_direction(DIR_DOWN);
			x_stopped = RUNNING;
			y_stopped = RUNNING;
			stop_run_flag_x = RUN;
			stop_run_flag_y = RUN;
		}
	}
	else if(motor_state == STATE_CROSS){
		if(y_stopped == STOPPED && x_stopped == STOPPED){
			motor_state = STATE_NONE;
			stop_run_flag_y = STOP;
			stop_run_flag_x = STOP;
		}
	}

	// digitalWrite(X_DIR, LOW);
	// for(int i = 0; i < 3200; i++)
	// {
	// 	digitalWrite(X_STEP, HIGH);
	// 	delayMicroseconds(300);
	// 	digitalWrite(X_STEP, LOW);
	// 	delayMicroseconds(300);
	// }
	// delay(2000);

	// digitalWrite(X_DIR, HIGH);
	// for(int i = 0; i < 3200; i++)
	// {
	// 	digitalWrite(X_STEP, HIGH);
	// 	delayMicroseconds(300);
	// 	digitalWrite(X_STEP, LOW);
	// 	delayMicroseconds(300);
	// }
	// delay(2000);

}

char toggle_flag_x = 0;
char toggle_flag_y = 0;

SIGNAL(TIMER2_COMPA_vect)
{
	if(stop_run_flag_x == RUN) {
		if(toggle_flag_x == 0){
			toggle_flag_x = 1;
			digitalWrite(X_STEP, HIGH);
		}
		else{
			toggle_flag_x = 0;
			digitalWrite(X_STEP, LOW);
			step_count_x++;
			if(step_count_x > howManyCounts_x)
			{
				stop_run_flag_x = STOP;
				step_count_x = 0;
				x_stopped = STOPPED;
			}
		}
	}

	if(stop_run_flag_y == RUN) {
		if(toggle_flag_y == 0){
			toggle_flag_y = 1;
			digitalWrite(Y_STEP, HIGH);
		}
		else{
			toggle_flag_y = 0;
			digitalWrite(Y_STEP, LOW);
			step_count_y++;
			if(step_count_y > howManyCounts_y)
			{
				stop_run_flag_y = STOP;
				step_count_y = 0;
				y_stopped = STOPPED;
			}
		}
	}
}