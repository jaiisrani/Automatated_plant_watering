// Detect moisture level in soil
int soil_pin = A0; // Some analog pin
int moisture_level = 0; // To store value of moisture level between 0 and 1023

// Setting the pump speed and powering the pump
int pump_pin = 11; // Use one of the PWM pins here. Ai's are for reading analog input, which is not needed here.
int moisture_threshold = 300; // value between 0 and 1023, depending on the type of soil // Pump is ON while soil moisture content < moisture threshold.
int pump_speed; // Placeholder for int value between 0 and 255.
int max_pump_speed = 230; // Putting an upper bound on pump speed because very fast flowing water causes leaching
int min_pump_speed = 210; // so that pump is not deadlocked, i.e. it is able to provide water to the plant
float pump_speed_i = 0.0; // Intermediate variable.

// Buzzer
int buzzer_pin = 10; // some pwm pin to power buzzer
int buzzer_val = 5; // some integer value from 0 to 255 denoting power given to buzzer

// Temperature sensing
// Implementing an LM35 Temperature Sensor
int temp_pin = A1; // some analog-input pin for taking temperature input
bool temp_bool = true;
float temp_threshold = 30.00; // pump has increased speed for temperature > 25*C
float cel; // temperature in celsius

void setup() {
  pinMode(soil_pin, INPUT);
  pinMode(pump_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(temp_pin, INPUT);
  Serial.begin(9600); // Serial commands to be used only if connected to computer. //
}

void loop() {
  // Detect moisture level in soil
  moisture_level = analogRead(soil_pin); // Resolution between 0 and 1023.
  // measure atmospheric temperature
  if (temp_bool == true) {
    int temp_val = analogRead(temp_pin);
    float mv = (temp_val/1023.0)*5000;
    cel = mv/10;
    temp_bool = false;
  }

  // Setting the pump_speed
  if (moisture_level <= moisture_threshold) { // Have to extend further //
    Serial.print("Moisture level is "); // Serial commands to be used only if connected to computer. //
    Serial.println(moisture_level); 
    float initial_pump_speed = pump_speed_i;
    pump_speed_i = (1 - pow((moisture_level / (1.0*moisture_threshold)),5)) * 255; // Function for speed of pump.
    pump_speed = int(pump_speed_i);

    // Putting an upper bound on pump speed and powering the pump, buzzer
    if (pump_speed >= 0) {
      int final_pump_speed = max(min_pump_speed, min(pump_speed, max_pump_speed)); // Since pump fails to send water at speed < min_pump_speed.
      Serial.print("Pump speed is "); // Serial commands to be used only if connected to computer. //
      Serial.println(final_pump_speed);
      analogWrite(pump_pin, final_pump_speed);
      analogWrite(buzzer_pin, buzzer_val);
      delay(2000);}
    analogWrite(pump_pin, 0);
    analogWrite(buzzer_pin, 0);
    delay(15000); // Wait for six seconds to let water get evenly distributed in the soil. This also prevents pump from heating up.
  }
  else {
    Serial.print("Moisture level is "); // Serial commands to be used only if connected to computer. //
    Serial.println(moisture_level);
    long int waiting_period = 600000; // 10 minutes // in reality one day
    waiting_period = waiting_period - int(cel - temp_threshold)*60*60*1000/2; // dividing by 2 jus to reduce the magnitude of shift due to temperature
    delay(waiting_period);
    temp_bool = true;
  }
}
