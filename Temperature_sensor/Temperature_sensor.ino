// Implementing an LM35 Temperature Sensor
int temp_pin = A1; // some analog-input pin for taking temperature input

void setup() {
  pinMode(temp_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int temp_val = analogRead(temp_pin);
  float mv = (temp_val/1023.0)*5000;
  float cel = mv/10;
  Serial.print("Temperature is ");
  Serial.print(cel);
  Serial.println(" *C");
  delay(1000);
}
