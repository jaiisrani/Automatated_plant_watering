// I will test the motor circuit
int power_pin = 3; // Use one of the PWM pins here. Ai's are are for reading analog input
int power_val = 170; // Any integer from 0 to 255. This is the analog voltage to be providedto motor

void setup() {
  pinMode(power_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  analogWrite(power_pin, power_val);
}
