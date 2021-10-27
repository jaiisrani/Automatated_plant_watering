//reading averaging
int num_samples=10; //take these many reading samples and average them to reduce noise
int sample_wait=100; //wait this long to take another sample
int moisture_reading_func();
int temp_reading_func();

// Detect moisture level in soil.
int soil_power_pin=4;
int soil_pin = A0; // Some analog pin.
int moisture_level = 1023; // To store value of moisture level between 0 and 1023.

// Setting the pump speed and powering the pump
int pump_pin = 11; // Use one of the PWM pins here. Ai's are for reading analog input, which is not needed here.
int moisture_threshold = 318; // Value between 0 and 1023, depending on the type of soil. 
// Pump is ON while soil moisture content < moisture threshold.
int pump_speed; // Placeholder for int value between 0 and 255.
int max_pump_speed = 245; // Putting an upper bound on pump speed because very fast flowing water causes leaching.
int min_pump_speed = 190; // So that pump is not deadlocked, i.e. it is able to provide water to the plant.
float pump_speed_i = 0.0; // Intermediate variable.

// Buzzer
int buzzer_pin = 10; // Some pwm pin to power buzzer.
int buzzer_val = 5; // Some integer value from 0 to 255 denoting power given to buzzer.

// Temperature sensing
//// Implementing an LM35 Temperature Sensor.
int temp_pin = A1; // Some analog-input pin for taking temperature input.
bool temp_bool = true;
float temp_threshold = 25.00; // Pump has increased speed for temperature > 25*C.
float cel; // Temperature in celsius.
long int max_wait=600000; // maximum waiting period between consecutive watering periods. 10 minutes here. In reality 24 hours.

// Ultrasonic sensor
int trig_pin = 7; // Some non-pwm digital pin for triggering ultrasonic wave.
int echo_pin = 8; // Some non-pwm digital pin for receiving reflected ultrasonic wave.
float duration;
float distance;
float distance_threshold=15.0;

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
  ///*
  // Using ultrasonic sensor to see if tank has enough water.
  digitalWrite(trig_pin, LOW); // Triggers the incident ultrasonic wave.
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH); // Measures the duration for which reflected pulse is high.
  distance = duration * 0.034 / 2; // In cm.
  if (int(distance) > distance_threshold) {
    Serial.println("Refill tank!");
    while (true) {
      analogWrite(buzzer_pin, 100);
      delay(500);
      analogWrite(buzzer_pin, 0);
      delay(500);}}//*/
  
  // Detect moisture level in soil
  moisture_level = moisture_reading_func(); // Calling the measuring functions that returns an averaged value
  // measure atmospheric temperature
  if (temp_bool == true) {
    int temp_val = temp_reading_func();
    float mv = (temp_val/1023.0)*5000;
    cel = mv/10;
    temp_bool = false;
  }

  // Setting the pump_speed
  if (moisture_level >= moisture_threshold) {
    Serial.print("Moisture Sensor reading is "); // Serial commands to be used only if connected to computer. //
    Serial.println(moisture_level); 
    Serial.print("Temperature is ");
    Serial.println(cel);
    float initial_pump_speed = pump_speed_i;
    pump_speed_i = (1 - pow(((1.0*moisture_threshold)/moisture_level ),5)) * 255; // Function for speed of pump.
    pump_speed = int(pump_speed_i);

    // Putting an upper bound on pump speed and powering the pump, buzzer
    if (pump_speed >= 0) {
      int final_pump_speed = max(min_pump_speed, min(pump_speed, max_pump_speed)); // Since pump fails to send water at speed < min_pump_speed.
      Serial.print("Pump speed is "); // Serial commands to be used only if connected to computer. //
      Serial.println(final_pump_speed);
      analogWrite(pump_pin, final_pump_speed);
      analogWrite(buzzer_pin, buzzer_val);
      delay(3000);}
    analogWrite(pump_pin, 0);
    analogWrite(buzzer_pin, 0);
    delay(8000); // This prevents pump from heating up. Also allows water to get uniformly distributed.
  }
  else {
    Serial.print("There is sufficient moisture. Moisture Sensor reading is "); // Serial commands to be used only if connected to computer. //
    Serial.println(moisture_level);
    Serial.print("Temperature is ");
    Serial.println(cel);
    long int waiting_period = max_wait;
      analogWrite(buzzer_pin, 255);
      delay(200);
      analogWrite(buzzer_pin, 0);
      delay(200);
      analogWrite(buzzer_pin, 255);
      delay(200);
      analogWrite(buzzer_pin, 0);
      delay(200);
      analogWrite(buzzer_pin, 255);
      delay(200);
      analogWrite(buzzer_pin, 0);
      delay(200);
    //waiting_period= (1-(1/(cel-temp_threshold)))*waiting_period;
    delay(min(waiting_period,max_wait));
    temp_bool = true;
  }
}

int moisture_reading_func() // to take average readings of soil moisture before watering
{
   int moist[num_samples];
   int moisture_avg,sum=0;
   for(int i=0;i<num_samples;i++)
   {
      moist[i]=analogRead(soil_pin); // Resolution between 0 and 1023.
      delay(sample_wait);
   }
   for(int j=0;j<num_samples;j++)
   {
      sum=sum+moist[j];
   }
   moisture_avg=sum/num_samples;
   return moisture_avg;
}

int temp_reading_func() // to take average readings of soil moisture before watering
{
   int temp[num_samples];
   int temp_avg,sum=0;
   for(int i=0;i<num_samples;i++)
   {
      temp[i]=analogRead(temp_pin);
      delay(sample_wait);
   }
   for(int j=0;j<num_samples;j++)
   {
      sum=sum+temp[j];
   }
   temp_avg=sum/num_samples;
   return temp_avg;
}
