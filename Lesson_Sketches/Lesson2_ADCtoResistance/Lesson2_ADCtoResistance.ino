/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 * 
 * Lesson 2 - Converting a digital reading from a thermistor to a physical sensor property (resistance)
 * 
 * We read sensor inputs Analog to Digital Converter
 * 
 * 
 * Here, we will define a simple function, calculate_resistance(), to convert the digital input from the 
 * ADC into the physical property (resistance) from the thermistor.
 * 
 */

int temperaturePin = A0; 


void setup() {
 
  // Initialize serial monitor at 9600 baud
  Serial.begin(9600);  

}

void loop() {
  
  int temperature_analog = analogRead(temperaturePin);  

   // We have seen that the above command does produce a number which responds to temperature.
   // However, its units are meaningless.  
   // This number is simply a DIGITAL number which corresponds to the ANALOG voltage on our pin. (https://en.wikipedia.org/wiki/Analog-to-digital_converter)
   // The possible values of this number are dictated by the RESOLUTION (here, 10 bit) of our ADC
   // (The built-in 10-bit on the Arduino board gives a range of 2^10 = 1024 possible values)
   // And also by the VOLTAGE RANGE this resolution corresponds to (here, 0-5 V)

   // To convert this number to something physically useful, we do two calculations:
   // Firstly, we convert the digital number to the analog voltage on the pin, using the known ADC resolution and voltage range
   // See: https://learn.sparkfun.com/tutorials/analog-to-digital-conversion/relating-adc-value-to-voltage
   
   // Secondly, we use this voltage to calculate the resistance of the thermistor by applying Ohm's law to the voltage divider circuit 
   // See: https://en.wikipedia.org/wiki/Voltage_divider
   // For our circuit, we have a fixed resistor (10K) as our R1 and the thermistor (a variable resistor) as R2
   

   float temperature_resistance_ohms = calculate_resistance(temperature_analog, 10000.0, 5.0);

  
  Serial.print("Thermistor = ");
  Serial.print(temperature_resistance_ohms);
  Serial.print(" ohms");
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
