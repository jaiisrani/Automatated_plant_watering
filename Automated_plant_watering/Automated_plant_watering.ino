// Detect moisture level in soil
int soil_pin = A0; // Some analog pin
int moisture_level = 0; // To store value of moisture level between 0 and 1023

// Setting the pump speed and powering the pump
int pump_pin = 11; // Use one of the PWM pins here. Ai's are for reading analog input, which is not needed here.
int moisture_threshold = 999; // value between 0 and 1023. // Pump is ON while soil moisture content < moisture threshold.
int pump_speed; // Placeholder for int value between 0 and 255.
int max_pump_speed = 199; // Putting an upper bound on pump speed because very fast flowing water causes leaching
float pump_speed_1 = 0.0; // intermediate variable

void setup() {
  pinMode(soil_pin, INPUT);
  pinMode(pump_pin, OUTPUT);
  Serial.begin(9600); // Serial commands to be used only if connected to computer //
}

void loop() {
  // Detect moisture level in soil
  moisture_level = analogRead(soil_pin); // Resolution between 0 and 1023.
  Serial.println(moisture_level); // Serial commands to be used only if connected to computer //

  // Setting the pump_speed
  if (moisture_level <= moisture_threshold) {
    float initial_pump_speed = pump_speed_1;
    pump_speed_1 = (1 - (moisture_level / moisture_threshold)) * 255; // Function for speed of pump.
    pump_speed = int(pump_speed_1);

    // Putting an upper bound on pump speed and powering the pump
    if (pump_speed <= max_pump_speed) {
      analogWrite(pump_pin, pump_speed);
      delay(2000);} // Let pump run for two seconds.
    else {
      analogWrite(pump_pin, max_pump_speed);
      delay(2000);} // Let pump run for two seconds.
    analogWrite(pump_pin, 0);
    delay(6000); // Wait for six seconds to let water get evenly distributed in the soil. This also prevents pump from heating up
  }
}
