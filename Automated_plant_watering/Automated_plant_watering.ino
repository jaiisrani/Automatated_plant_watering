// Detect moisture level in soil
int soil_pin = A0; // Some analog pin
int moisture_level = 0; // To store value of moisture level between 0 and 1023

// Setting the pump speed and powering the pump
int pump_pin = 11; // Use one of the PWM pins here. Ai's are for reading analog input, which is not needed here.
int moisture_threshold = 400; // value between 0 and 1023, depending on the type of soil // Pump is ON while soil moisture content < moisture threshold.
int pump_speed; // Placeholder for int value between 0 and 255.
int max_pump_speed = 230; // Putting an upper bound on pump speed because very fast flowing water causes leaching
int min_pump_speed = 220; // so that pump is not deadlocked, i.e. it is able to provide water to the plant
float pump_speed_i = 0.0; // Intermediate variable.

void setup() {
  pinMode(soil_pin, INPUT);
  pinMode(pump_pin, OUTPUT);
  Serial.begin(9600); // Serial commands to be used only if connected to computer. //
}

void loop() {
  // Detect moisture level in soil
  moisture_level = analogRead(soil_pin); // Resolution between 0 and 1023.

  // Setting the pump_speed
  if (moisture_level <= moisture_threshold) {
    Serial.print("Moisture level is ");
    Serial.println(moisture_level); // Serial commands to be used only if connected to computer. //
    float initial_pump_speed = pump_speed_i;
    pump_speed_i = (1 - (moisture_level / (1.0*moisture_threshold))) * 255; // Function for speed of pump.
    pump_speed = int(pump_speed_i);

    // Putting an upper bound on pump speed and powering the pump
    if (pump_speed >= 0) {
      int final_pump_speed = max(min_pump_speed, min(pump_speed, max_pump_speed));
      Serial.print("Pump speed is ");
      Serial.println(final_pump_speed);
      analogWrite(pump_pin, final_pump_speed); // Since pump fails to send water at speed < 195.
      delay(2000);}
    analogWrite(pump_pin, 0);
    delay(6000); // Wait for six seconds to let water get evenly distributed in the soil. This also prevents pump from heating up.
  }
  else {
    Serial.print("Moisture level is ");
    Serial.println(moisture_level); // Serial commands to be used only if connected to computer. //
    float waiting_period = 60000; // 60 seconds
    delay(waiting_period);
  }
}
