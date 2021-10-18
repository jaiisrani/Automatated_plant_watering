int pump_pin = ; // some analog pin
int moisture_threshold = ; // value between 0 and 1023.
//// Pump is ON while soil moisture content < moisture threshold

float pump_speed; // placeholder for value between 0 and 255
// Soil moisture variables:
//// int moisture_level

void setup() {
  Serial.begin(9600);
  pinMode(pump_pin, OUTPUT);
}

void loop() {
  if (moisture_level < moisture_threshold) {
    float initial_pump_speed = pump_speed;
    // Have todesign proper function for pump_speed
    pump_speed = (1 - (moisture_level / moisture_threshold)) * 255 - (initial_pump_speed / 10);
    analogWrite(pump_pin, int(pump_speed)); 
  }
}
