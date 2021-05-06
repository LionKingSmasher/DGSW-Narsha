void setup()
{
	Serial.begin(9600);
}

void loop(){
	int a0_value = analogRead(A0);

	float voltage = ((float)a0_value/1024.0) * 5.0;

	Serial.println(voltage);
	delay(10);
}