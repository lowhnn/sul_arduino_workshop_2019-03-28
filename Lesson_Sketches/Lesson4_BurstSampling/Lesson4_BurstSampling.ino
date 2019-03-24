/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 * 
 * Lesson 4 -  Burst sampling a sensor 
 * 
 * You may notice that the temperature readings can be 'noisy', i.e., the value fluctuates.
 * This can be an issue especially with more sensitive sensors.
 * 
 * Here, we will define a function that uses a for loop to 'burst sample' temperature
 * i.e., we will take multiple readings over a short time and average them. 
 * This should give us a more stable temperature reading.
 *  
 */

int temperaturePin = A0; 

void setup() {
 
  Serial.begin(9600);  
  
}

void loop() {

//  int temperature_analog = analogRead(temperaturePin);  

   // Suppose our temperature was "noisy", (https://en.wikipedia.org/wiki/Noise_(signal_processing))
   // We may decide to cope with this by BURST SAMPLING, and taking the average of an ENSEMBLE of samples
   // So, we create the FUNCTION read_temp_avg(ARGUMENTS), to do precisely this.
   int temperature_analog_avg = read_temp_avg(temperaturePin, 20);


   // Convert the averaged digital values from the ADC to a resistance value (Lesson 2)
   float temperature_resistance_ohms = calculate_resistance(temperature_analog_avg, 10000.0, 5.0);
   
   // Convert the resistance value to a useable temperature value (Lesson 3)
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


// This is a function to burst sample the thermistor by taking multiple ADC readings and averaging them
int read_temp_avg(int pin, int samples){

    //This function takes as INPUT ARGUMENTS the pin which we want to burst sample, and how many samples we want to average over. 
    
    int T_avg ;

    //See: https://www.arduino.cc/reference/en/language/structure/control-structure/for/
    //For explanation of the "for()" control stucture
    for (int i = 1; i<=samples; i++){
      T_avg += analogRead(pin);
    }

    T_avg /= samples;

    //Here, we specify the variable (if any) we want to RETURN ("spit out" of our function)
    return T_avg;


    //Check your understanding: what is the SCOPE of variables: pin, samples? 
    //What about T_avg?
    
}


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


