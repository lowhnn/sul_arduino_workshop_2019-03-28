/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 * Lesson 3 - Calculating a useful temperature reading from the sensor's electrical property (resistance)
 * 
 * We have successfully converted the digital values from the ADC to an actual physical property of the 
 * thermistor (resistance). Now we want to convert the thermistor's resistance to a temperature (which is the 
 * environmental property we actually care about!)
 * 
 * See: https://learn.adafruit.com/thermistor/overview
 * 
 */

int temperaturePin = A0; 

void setup() {
  
  // Initialize serial monitor at 9600 baud
  Serial.begin(9600);  

}

void loop() {
  
  // Read the thermistor input from the ADC
  int temperature_analog = analogRead(temperaturePin);  

  // Convert digital values from the ADC to a resistance value (Lesson 2)
  float temperature_resistance_ohms = calculate_resistance(temperature_analog, 10000.0, 5.0);

  // We know that the thermistor's resistance varies predictably with temperature.
  // This can be approximated using the B-parameter equation: 1/T = 1/To + (1/B)*ln(R/Ro)
  // See: https://en.wikipedia.org/wiki/Thermistor
  // Where T is the temperature, R is the resistance, To = 298.15 K (25 deg C, room temperature)
  // B and Ro are known properties of this specific thermistor (in this case, B = 3950, Ro = 10K)
  // See the specifications for the thermistor on its product page: https://www.adafruit.com/product/372
  
  // Note that there can be small errors/variations in these properties between sensors.
  // For example, thermistor specifications are listed as Ro = 10K +- 1% and B = 3950+- 1%
  // In research use, we often want and need to account for these variations in order to maximize
  // the accuracy of our sensor readings.
  // A way to do this is to calibrate each individual sensor using known temperature values, which
  // will give us the actual Ro and B values of each sensor.
  

   float temperature_C = calculate_temperature_C(temperature_resistance_ohms);

  // Print the temperature to the serial monitor so we can see it
  Serial.print("Temperature = ");
  Serial.print(temperature_C);
  Serial.write(0xC2); //These 2 lines allow us to print the ° symbol. If you are curious,
  Serial.write(0xB0); //they involve calling a special character directly from the ASCII table http://www.asciitable.com/
  Serial.print("C ");
  Serial.println();
  
  delay(500);

}

// These are the functions used in the loop() block
// FUNCTION declaration has form: RETURN TYPE  function_name((TYPE) INPUT ARGUMENTS)


// This is a function to calculate thermistor resistance from the digital value
// produced by the Arduino's 10-bit ADC
float calculate_resistance(int temp_analog, float series_resistance_ohms, float V_in){
  
    //This function takes as INPUT ARGUMENTS the digital value from the ADC 
    // As well as the resistance value of the fixed resistor R1, and the voltage across the entire voltage divider circuit

    // Here we calculate the ADC pin voltage from the digital value we have.
    
    float temp_voltage = 0.004887586*(float)temp_analog;

    // Now we calculate the resistance of the thermistor

    float temp_resistance = (series_resistance_ohms*temp_voltage)/(V_in-temp_voltage);
  
  return temp_resistance; 
  
}


// This is a function to calculate the measured temperature from the resistance of the thermistor
float calculate_temperature_C(float temp_resistance){
  
  // This function takes the thermistor resistance value as its only input argument
  
  // We convert resistance to temperature using the B-parameter equation
  
  float temp_C = (1/(((0.003354016434681)-((0.0002531645569620253)*log(10000)))+((0.0002531645569620253)*log(temp_resistance))))-273.15;

  return temp_C;

}
  
