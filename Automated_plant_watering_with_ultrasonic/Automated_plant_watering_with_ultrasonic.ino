// Detect moisture level in soil.
int soil_pin = A0; // Some analog pin.
int moisture_level = 0; // To store value of moisture level between 0 and 1023.

// Setting the pump speed and powering the pump
int pump_pin = 11; // Use one of the PWM pins here. Ai's are for reading analog input, which is not needed here.
int moisture_threshold = 1000; // Value between 0 and 1023, depending on the type of soil. 
// Pump is ON while soil moisture content < moisture threshold.
int pump_speed; // Placeholder for int value between 0 and 255.
int max_pump_speed = 230; // Putting an upper bound on pump speed because very fast flowing water causes leaching.
int min_pump_speed = 210; // So that pump is not deadlocked, i.e. it is able to provide water to the plant.
float pump_speed_i = 0.0; // Intermediate variable.

// Buzzer
int buzzer_pin = 10; // Some pwm pin to power buzzer.
int buzzer_val = 5; // Some integer value from 0 to 255 denoting power given to buzzer.

// Temperature sensing
//// Implementing an LM35 Temperature Sensor.
int temp_pin = A1; // Some analog-input pin for taking temperature input.
bool temp_bool = true;
float temp_threshold = 30.00; // Pump has increased speed for temperature > 25*C.
float cel; // Temperature in celsius.

// Ultrasonic sensor
int trig_pin = 7; // Some non-pwm digital pin for triggering ultrasonic wave.
int echo_pin = 8; // Some non-pwm digital pin for receiving reflected ultrasonic wave.
float duration;
float distance;

void setup() {
  pinMode(soil_pin, INPUT);
  pinMode(pump_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(temp_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  Serial.begin(9600); // Serial commands to be used only if connected to computer. //
}

void loop() {
  // Using ultrasonic sensor to see if tank has enough water.
  digitalWrite(trig_pin, LOW); // Triggers the incident ultrasonic wave.
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH); // Measures the duration for which reflected pulse is high.
  distance = duration * 0.034 / 2; // In cm.
  if (int(distance) > 15) {
    Serial.println("Refill pump!");
    while (true) {
      analogWrite(buzzer_pin, 100);
      delay(500);
      analogWrite(buzzer_pin, 0);
      delay(500);}}
  
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
  if (moisture_level <= moisture_threshold) {
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
    delay(1000); // Wait for six seconds to let water get evenly distributed in the soil. This also prevents pump from heating up.
  }
  else {
    Serial.print("Moisture level is "); // Serial commands to be used only if connected to computer. //
    Serial.println(moisture_level);
    long int waiting_period = 600000; // 10 minutes. In reality it will be 24 hours.
    waiting_period = waiting_period - int(cel - temp_threshold)*60*60*1000/2; // Dividing by 2 just to reduce the magnitude of shift due to temperature.
    delay(waiting_period);
    temp_bool = true;
  }
}
