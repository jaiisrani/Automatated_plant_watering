int buzzer_pin = 10; // some pwm pin to power buzzer
int buzzer_val = 3; // some integer value from 0 to 255

void setup() {
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  analogWrite(buzzer_pin, buzzer_val);
}
