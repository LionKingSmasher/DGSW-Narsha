#define ENCODER_A 10
#define ENCODER_B 11
#define ENCODER_BTN 16

#define LEFT 1
#define RIGHT 2

void setup() {
	Serial.begin(9600);
	pinMode(ENCODER_A, INPUT);
	pinMode(ENCODER_B, INPUT);
	pinMode(ENCODER_BTN, INPUT);

	digitalWrite(ENCODER_A, HIGH);
	digitalWrite(ENCODER_B, HIGH);
	digitalWrite(ENCODER_BTN, HIGH);
}

char flag_up = 0;
char flag_down = 0;
char flag_button = 0;

unsigned long curr_millis = 0;
unsigned long pre_millis = 0;

int state = 0;
int direction = 0;

// void loop() {
	// curr_millis = millis();
	// int in = digitalRead(ENCODER_BTN);
  // put your main code here, to run repeatedly:
	// int in = digitalRead(ENCODER_BTN);
	// if (in == 1){
	// 	if(flag_up == 0){
	// 		Serial.println("up");
	// 		flag_up = 1;
	// 		flag_down = 0;
	// 	}
	// }
	// else if(in == 0){
	// 	if(flag_down == 0){
	// 		Serial.println("down");
	// 		flag_down = 1;
	// 		flag_up = 0;
	// 	}
	// }

	// int in = digitalRead(ENCODER_BTN);
	// if (in == 1){
	// 	if(flag_button == 0) {
	// 		Serial.println("up");
	// 		flag_button = 1;
	// 	}
	// }
	// else if(in == 0){
	// 	if(flag_button == 1) {
	// 		Serial.println("down");
	// 		flag_button = 0;
	// 	}
	// }
	// delay(10);

	// if (in == 1 && flag_button == 0){
	// 	Serial.println("up");
	// 	flag_button = 1;
	// }
	// else if(in == 0 && flag_button == 1){
	// 	Serial.println("down");
	// 	flag_button = 0;
	// }

	// if(curr_millis - pre_millis > 10){
	// 	pre_millis = curr_millis;
	// 	if (in == 1 && flag_button == 0){
	// 		Serial.println("up");
	// 		flag_button = 1;
	// 	}
	// 	else if(in == 0 && flag_button == 1){
	// 		Serial.println("down");
	// 		flag_button = 0;
	// 	}
	// }

// 	int in_a = digitalRead(ENCODER_A);
// 	int in_b = digitalRead(ENCODER_B);
	
// 	if(in_a == 1 && in_b == 1){
// 		if(state == 3 && direction == RIGHT){
// 			state = 0;
// 			Serial.println("right");
// 		}
// 		else if (state == 1 && direction == LEFT){
// 			state = 0;
// 			Serial.println("left");
// 		}
// 		direction = 0;
// 	}
// 	else if(in_a == 0 && in_b == 1){
// 		if(state == 0){
// 			direction = RIGHT;
// 			state = 1;
// 		}
// 	}
// 	else if(in_a == 0 && in_b == 0){
// 		state = 2;
// 	}
// 	else if(in_a == 1 && in_b == 0){
// 		if(state == 0){
// 			direction = LEFT;
// 			state = 3;
// 		}
// 		else if(state == 2){
// 			state = 3;
// 			direction = RIGHT;
// 		}
// 	}
// 	delay(10);
// }

int left_state = 0;
int right_state = 0;

void loop(){
	int in_a = digitalRead(ENCODER_A);
	int in_b = digitalRead(ENCODER_B);
	
	if(in_a == 1 && in_b == 1){
		if(right_state == 3) Serial.println("r");
		else if(left_state == 3) Serial.println("l");
		left_state = 0;
		right_state = 0;
	}
	else if(in_a == 0 && in_b == 1){
		if(right_state == 0) right_state = 1;
		else if(left_state == 2) left_state = 3;
	}
	else if(in_a == 0 && in_b == 0){
		if(right_state == 1) right_state = 2;
		else if(left_state == 1) left_state = 2;
	}
	else if(in_a == 1 && in_b == 0){
		if(right_state == 2) right_state = 3;
		else if(left_state == 0) left_state = 1;
	}
	delay(1);
}