// #include "gcode_sample_480.h"
//#include "gcode_sample_200_string.h
// #include "gcode_sample_300_string.h"

#define X_DIR  21
#define X_STEP 15
#define X_STOP 18

#define Y_STOP 17
#define Y_DIR  23
#define Y_STEP 22

#define XY_EN  14
#define NUM_GCODE 600

#define BUFFER_LEN  1024
#define GCODE_BUF_LEN  100

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

//================================================================

#include <SPI.h>
#include <SD.h>

enum{
  NOT_FULL = 0,
  FULL
};

enum{
  NOT_EMPTY = 0,
  EMPTY
};

File myFile;

char buffer[BUFFER_LEN];
char gcode_buf[GCODE_BUF_LEN][50];
int buf_index = 0;
int gcode_index = 0;
int gcode_buf_head = 0;
int gcode_buf_tail = 0;
int free_buf_num = GCODE_BUF_LEN;
int can_use_buff = 0;
unsigned int buf_total_len = 0;
unsigned int buf_len = 0;
int num_gcode = 0;
char gcode_buf_full = 0;

int tmp_toggle = 0;

char buf_full_or_not()
{
  if(gcode_buf_head == gcode_buf_tail)
  {

  }
  else if(gcode_buf_head > gcode_buf_tail)
  {
    int tmp_buf_len = GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
    if(tmp_buf_len == 1) return FULL;
    else if(tmp_buf_len > 1) return NOT_FULL;
  }
  else if(gcode_buf_head < gcode_buf_tail)
  {
    int tmp_head = gcode_buf_head + GCODE_BUF_LEN;
    int tmp_buf_len = GCODE_BUF_LEN - (tmp_head - gcode_buf_tail);
    if(tmp_buf_len > 0) return NOT_FULL;
    else return FULL;
  }

}

char buf_is_empty()
{
  if(gcode_buf_head == gcode_buf_tail)
    return EMPTY;
  else
    return NOT_EMPTY;
}

char buf_is_full()
{
  if(gcode_buf_head == gcode_buf_tail)
    return EMPTY;
  else
    return NOT_EMPTY;
}


int buff_free_num()
{
    int temp_buf_len;
    if(gcode_buf_head >= gcode_buf_tail) 
      temp_buf_len= GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
    else if(gcode_buf_tail > gcode_buf_head)
      temp_buf_len= (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;

    return temp_buf_len;
}

void free_buf_num_func(){
  if(gcode_buf_head >= gcode_buf_tail){
    free_buf_num = GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
  }
  else if(gcode_buf_head < gcode_buf_tail){
    //free_buf_num = GCODE_BUF_LEN - (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;
    free_buf_num = gcode_buf_tail - gcode_buf_head;
  }
}

void buffer_clear(){
  for(int i = 0; i < 50; i++)
    gcode_buf[gcode_buf_tail][i] = 0;
}
//===========================================================================

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

// double xy_value[NUM_GCODE][2];
// double f_value[NUM_GCODE];
// int e_value[NUM_GCODE];

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

int gcode_parsing_index = 0;

char parsing_done = 0;

void gcode_parsing_one_string(char* gcode_string)
{
	int one_code_length = strlen(gcode_string);
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
	if(*(gcode_string+0)=='G' && *(gcode_string+1)=='1')
	{
		for(int i = 2; i < one_code_length; i++)
		{
			for(int m = 0; m < 4; m++){
				if(*(gcode_string + i) == initial_value[m]){
					which_buffer = m;
					buffer_index = 0;
					break;
				}
			}
			if(*(gcode_string+i) != ' ')
				xyef_string[which_buffer][buffer_index++] = *(gcode_string+i);
		}
		for(int i = 0; i < 4; i++){
			xyef_value[gcode_parsing_index][i] = atof(&xyef_string[i][1]);
		}
		//=========================================
		// int tmp_index = gcode_parsing_index - 1;
		// if(gcode_parsing_index < 0)
		// 	gcode_parsing_index = 0;
		// Serial.print("[" + String(gcode_parsing_index) + "]: ");
		// for(int i = 0; i < 4; i++){
		// 	Serial.print(xyef_value[gcode_parsing_index][i]);
  //   		Serial.print(" ");
		// }
		// Serial.println("");
		//=========================================
		gcode_parsing_index++;
		if(gcode_parsing_index == NUM_GCODE) parsing_done = 1;
	}

	// if(interrupt_stopped_x == STOPPED){
	//   	timer_x_disable();
	// }
	// if(interrupt_stopped_y == STOPPED){
	//   	timer_y_disable();
	// }
	// if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED)
	// {
	// 	if(gcode_index < gcode_parsing_index && gcode_parsing_index > 0){
	// 		Serial.println("gcode_index = " + String(gcode_index));
	// 		motor_move_1(xyef_value[gcode_index][0], xyef_value[gcode_index][1], xyef_value[gcode_index][3]);
	// 		if(gcode_index++ == NUM_GCODE) while(1);
	// 	}
	// 	// Serial.println(String(gcode_index));
	// }

    // Serial.println(xy_value[j][0]);
    // Serial.println(xy_value[j][1]);
    // Serial.println(f_value[j]);
}

// void gcode_parsing()
// {
// 	for(int j = 0; j < NUM_GCODE; j++)
// 	{
// 		int one_code_length = strlen(xy_pos_string[j]);
// 		buffer_index = 0;

// 		for(int k = 0; k < 4; k++){
// 			for(int i = 0; i < 10; i++){
// 				xyef_string[k][i] = 0;
// 			}
// 		}
// 		for(int i = 0; i < 10; i++)
// 		{
// 			x_value_string[i] = 0;
// 			y_value_string[i] = 0;
// 			e_value_string[i] = 0;
// 			f_value_string[i] = 0;
// 		}
// 		if(*(xy_pos_string[j]+0)=='G' && *(xy_pos_string[j]+1)=='1')
// 		{
// 			for(int i = 2; i < one_code_length; i++)
// 			{
// 				for(int m = 0; m < 4; m++){
// 					if(*(xy_pos_string[j] + i) == initial_value[m]){
// 						which_buffer = m;
// 						buffer_index = 0;
// 						break;
// 					}
// 				}
// 				if(*(xy_pos_string[j]+i) != ' ')
// 					xyef_string[which_buffer][buffer_index++] = *(xy_pos_string[j]+i);
// 			}
// 			for(int i = 0; i < 4; i++){
// 				xyef_value[j][i] = atof(&xyef_string[i][1]);
// 			}
// 			// xy_value[j][0] = atof(&x_value_string[1]);
// 			// xy_value[j][1] = atof(&y_value_string[1]);
// 			// f_value[j] = atoi(&f_value_string[1]);
// 			// if(f_value[j] == 0){
// 		 //      f_value[j] = f_value[j-1];
// 		 //    }
// 		}
// 	    // Serial.println(xy_value[j][0]);
// 	    // Serial.println(xy_value[j][1]);
// 	    // Serial.println(f_value[j]);
// 	}
// }

void can_use_buff_func(){
	if(gcode_buf_head >= gcode_buf_tail){
        can_use_buff = gcode_buf_head - gcode_buf_tail;
    }
    else if(gcode_buf_head < gcode_buf_tail){
        can_use_buff = (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;
    }
}

void setup()
{
	Serial.begin(115200);
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

	// gcode_parsing();
	Serial.print("Initializing SD card...");
	if (!SD.begin(31)) {
		Serial.println("initialization failed!");
	 	while(1);
	}
	Serial.println("initialization done.");
	  
	myFile = SD.open("gcode.gco");
	if (myFile) {


	} else {
	 	Serial.println("error opening test.txt");
	}
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

int pre_state = 0;

void loop()
{
	if (myFile)
    {
      if(myFile.available())
      {
        // num_of_free_buff = buff_free_num();
        // Serial.println(num_of_free_buff);

        if(free_buf_num > 0)
        {
          char tmp_read = myFile.read();
          gcode_buf[gcode_buf_head][buf_index++] = tmp_read;

          if(tmp_read == '\n')
          {
            // Serial.print("Head[" + String(gcode_buf_head) + "]: ");
            // Serial.println(gcode_buf[gcode_buf_head]);
            buf_index = 0;

          //  if(num_of_free_buff > 1)
            gcode_buf_head++;
            free_buf_num_func();
            if(gcode_buf_head == GCODE_BUF_LEN) {
              gcode_buf_head = 0;
            }




            // if(gcode_buf_head == gcode_buf_tail)
            // {
            //   gcode_buf_full = 1;
            //   gcode_buf_head--;
            //   if(gcode_buf_head < 0) gcode_buf_head = GCODE_BUF_LEN - 1;
            // }
            

            // delay(1000);
          }
        }
      }
      else
      {
        myFile.close();
      }
    }

    if(tmp_toggle == 1){
      tmp_toggle = 0;

      can_use_buff_func();
      if(can_use_buff > 0){
      	// Serial.println("can_use_buff = " + String(can_use_buff));
      	if(parsing_done == 0)
      		gcode_parsing_one_string(gcode_buf[gcode_buf_tail]);
        // Serial.print("Tail[" + String(gcode_buf_tail) + "]: ");
        // Serial.println(gcode_buf[gcode_buf_tail]);

        buffer_clear();

        gcode_buf_tail++;

        free_buf_num_func();

        if(gcode_buf_tail == 100) gcode_buf_tail = 0;
      }
    }
    else if(tmp_toggle == 0){
      tmp_toggle=1;
    }
    
    // if(buf_is_empty() == NOT_EMPTY)
    // {
    //   String print_buf = "[" + String(gcode_buf_head) + "]" + gcode_buf[gcode_buf_head];
    //   //Serial.print(gcode_buf[gcode_index]);
    //   Serial.print(print_buf);
    // }

    if(interrupt_stopped_x == STOPPED){
	  	timer_x_disable();
	}
	if(interrupt_stopped_y == STOPPED){
	  	timer_y_disable();
	}
	if(interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED)
	{
		if(gcode_index < gcode_parsing_index && gcode_parsing_index > 0){
			// Serial.println("gcode_index = " + String(gcode_index));
			if(xyef_value[gcode_index][0] && xyef_value[gcode_index][1])
				motor_move_1(xyef_value[gcode_index][0], xyef_value[gcode_index][1], xyef_value[gcode_index][3]);
			if(gcode_index++ == NUM_GCODE) while(1);
		}
		// Serial.println(String(gcode_index));
	}
}