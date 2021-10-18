int moisture_level_1; // resolution between 0 and 1023
int moisture_level_2; // resolution between 0 and 1023
int moisture_rate; // store it as int amd not float otherwise arduino will approximate it to 2 decimals

// Soil moisture variables:
//// int soil_pin
void setup() {
  pinMode(soil_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
 // we will evaluate the average rate of water supply over 1 second // have to test what time to take
  moisture_level_1 = analogRead(soil_pin);
  delay(1000);
  moisture_level_2 = analogRead(soil_pin);
  moisture_rate = moisture_level_2 - moisture_level_1;
  delay(1000)
}
