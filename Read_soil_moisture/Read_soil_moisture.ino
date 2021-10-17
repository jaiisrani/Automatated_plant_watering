int soil_pin = ; // some analog pin
int moisture_level = 0; // to store value of moisture level between 0 and 1023

void setup() {
  pinMode(soil_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // to confirm whether we will use digital or analog input 
  moisture_level = analogRead(soil_pin); // resolution between 0 and 1023
}
