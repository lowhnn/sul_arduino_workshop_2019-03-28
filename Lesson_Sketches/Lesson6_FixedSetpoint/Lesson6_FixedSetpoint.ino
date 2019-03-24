/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 * 
 * Lesson 6 - Combining a sensor with an actuator to create a feedback loop
 * 
 * Another useful thing we can do with our temperature values is to compare them with a 
 * specified temperature setpoint, and if the measured temperature rises above this setpoint, 
 * we can get the Arduino to turn on an actuator.
 * 
 * For our purposes, an ACTUATOR is something that acts on the physical environment, in response 
 * to a digital signal from the Arduino, which we will produce using the function digitalWrite(). 
 * Examples of actuators can include lights, pumps, solenoid valves, motors, heaters, chillers, 
 * or the fans that we are using in this example. An actuator that acts to bring the sensor value 
 * back toward the setpoint (e.g., a fan to lower the temperature) forms part of a feedback or 
 * regulatory loop. 
 * 
 * Here, we will define a fixed temperature setpoint in our setup loop. We can do this either by
 * writing the setpoint value into our code (hard-coding), or by creating a function to read a 
 * number off a file on our SD card. By setting up our program to read the setpoint from the SD 
 * card, we don't have to edit and re-upload the code each time we want to change the setpoint.
 * 
 */


#include <SdFat.h>

SdFat sd;
SdFile myFile;

// Here we define our pin numbers as global variables
int temperaturePin = A0; 
int sdPin = 10;
int fanPin = 4; // we use a digital pin to turn on/off the fan


// Define a global variable for our temperature setpoint
float setpoint_C;



void setup() { 

  // Initialize serial monitor at 9600 baud
  Serial.begin(9600);  

  // Initialize the SD card 
  if (!sd.begin(sdPin)) sd.initErrorHalt();

  // Define the fanPin as an digital output pin
  // This will allow us to use the digitalWrite() function to control the fan from this pin. 
  pinMode(fanPin, OUTPUT);
  
  
  // We define a temperature setpoint, above which we want our fan to turn on
  setpoint_C = 29;
  
  // Alternatively, we can also read our desired temperature setpoint off a file on our SD card
  //setpoint_C = read_temperature_setpoint();

}



void loop() {

   // Burst sample and average the sample values (Lesson 4)
   int temperature_analog_avg = read_temp_avg(temperaturePin, 20);

   // Convert averaged digital values from the ADC to a resistance value (Lesson 2)
   float temperature_resistance_ohms = calculate_resistance(temperature_analog_avg, 10000.0, 5.0);

   // Convert resistance to temperature (Lesson 3)
   float temperature_C = calculate_temperature_C(temperature_resistance_ohms);
   
     
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
    sd.errorHalt("failed to open TempLog.txt"); // return error message if we fail to open the file
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


// This is a function to read a fixed temperature setpoint
float read_temperature_setpoint() {
  char readChar;
  char tempVal [5];
  float setPoint;

  Serial.println("Retrieving setpoint...");
  
  if (!myFile.open("SetPoint.txt", O_READ)) sd.errorHalt("Failed to load SetPoint.txt");
  while (myFile.available()) { 
    readChar = myFile.read();
    for (int i = 0; i < 5; i++) {
      if (readChar != '\0') {
        tempVal[i] = readChar;
        readChar = myFile.read();
      } else {
      tempVal[i] = '\0';
      break;
      }
    }
    setPoint = atof(tempVal);
  }
  myFile.close(); 

  Serial.print("Temperature setpoint = ");
  Serial.print(setPoint);
  Serial.write(0xC2);
  Serial.write(0xB0);
  Serial.println("C");
  
  return(setPoint);
}
  

