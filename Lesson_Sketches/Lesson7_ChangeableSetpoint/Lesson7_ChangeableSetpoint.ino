/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 * Lesson 7 - Feedback loop with changeable setpoint
 * 
 * We can also define the temperature setpoint more interactively. Here we will use a POTENTIOMETER 
 * (in the form of a knob) to adjust the temperature setpoint without having to edit code or SD card
 * files. A potentiometer is a adjustable voltage divider (see: https://en.wikipedia.org/wiki/Potentiometer). 
 * Recall from Lesson 2 that our thermistor is also set up in a voltage divider circuit. Therefore, 
 * we will handle the output from the potentiometer in a similar way to how we handled the signal 
 * from the thermistor. 
 * 
 * The key difference is that the potentiometer output does not 'naturally' correspond to any 
 * temperature. Instead, we will define the range of potential temperature setpoints that we want 
 * to be able to set using the knob, and 'map' this temperature range onto the range of potentiometer 
 * outputs.
 */


#include <SdFat.h>

SdFat sd;
SdFile myFile;


// Here we define our pin numbers as global variables
int temperaturePin = A0; 
int sdPin = 10;
int fanPin = 4;
int knobPin = A2; // like the thermistor pin, we use an analog pin to read the potentiometer



void setup() { 

  // Initialize serial monitor at 9600 baud
  Serial.begin(9600);  

  // Initialize the SD card  
  if (!sd.begin(sdPin)) sd.initErrorHalt();

  // Define the fanPin as an digital output pin
  pinMode(fanPin, OUTPUT);

}



void loop() {

  // Burst sample and average the sample values (Lesson 4)
   int temperature_analog_avg = read_temp_avg(temperaturePin, 20);

   // Convert averaged digital values from the ADC to a resistance value (Lesson 2)
   float temperature_resistance_ohms = calculate_resistance(temperature_analog_avg, 10000.0, 5.0);

   // Convert resistance to temperature (Lesson 3)
   float temperature_C = calculate_temperature_C(temperature_resistance_ohms);


   // Here we burst sample and average the readings from the potentiometer
   // This is exactly the same as what we did for the themistor in Lesson 4
   int knob_analog_avg = read_temp_avg(knobPin, 20);
   
   // Here we convert the averaged reading from the potentiometer to a temperature setpoint
   float setpoint_C = knob2temp_C(knob_analog_avg);

  
   // We define a BOOLEAN (true/false) variable to compare the current temperature to our setpoint
   // fanState is TRUE if current temperature is higher than the setpoint (i.e., we want to turn on the fan and cool down!)  
   bool fanState = (temperature_C > setpoint_C);


   // We define a STRING (an array of characters) to use in the serial output
   // It will report to us whether the fan is on or off
   
   String fanString = "Off";

   if(fanState){
    fanString = "On";
    }

  Serial.print("Temperature = ");
  Serial.print(temperature_C);
  Serial.write(0xC2);
  Serial.write(0xB0);
  Serial.print("C ");

  Serial.print("Setpoint = ");
  Serial.print(setpoint_C);
  Serial.write(0xC2);
  Serial.write(0xB0);
  Serial.print("C");

  Serial.print("  Fan = ");
  Serial.print(fanString);
  Serial.println();


  // Write the temperature, setpoint, and fan state to the SD card

  if (!myFile.open("TEMPLOG.TXT", O_RDWR | O_CREAT | O_AT_END)) { // open or create the file 'TEMPLOG.TXT' on the SD card
    sd.errorHalt("failed to open TEMPLOG.TXT"); // return error message if we fail to open the file
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to file...");
  myFile.print("Temperature = ");
  myFile.print(temperature_C);
  myFile.write(0xC2); //These 2 lines allow us to print the ° symbol. If you are curious,
  myFile.write(0xB0); //they involve calling a special character directly from the ASCII table http://www.asciitable.com/
  myFile.print("C, Setpoint = ");
  myFile.print(setpoint_C);
  myFile.write(0xC2);
  myFile.write(0xB0);
  myFile.print("C, Fan = ");
  myFile.println(fanString);

  // close the file:
  myFile.close();
  Serial.println("done.");



  // Turn the fan on or off, based on the fanState variable that we defined earlier
  digitalWrite(fanPin, fanState);

  delay(1000);

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

    return T_avg;
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


// This is a function to map potentiometer (knob) input to a range of temperature setpoints
float knob2temp_C(int temp_analog){

  // Define our desired range (minimum and maximum values) of setpoints
  float temp_min = 20;
  float temp_max = 36;

  // Map the range of temperature setpoints onto the range of digital values from the ADC
  // For the Arduino's built-in 10-bit ADC, this is a range of 1023 
  float set_temp_C = temp_min + ((temp_max - temp_min)/1023)*temp_analog;
  return set_temp_C;
  
}
