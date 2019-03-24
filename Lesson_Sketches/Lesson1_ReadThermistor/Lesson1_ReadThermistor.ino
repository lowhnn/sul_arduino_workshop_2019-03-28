/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Natalie HN Low, PhD (nlow@stanford.edu) and Paul R Leary, PhD (pleary@nps.edu)
 * 
 * 
 *
 * Lesson 1 -  Reading a thermistor 
 * 
 * Many uses of Arduinos in science involve reading one or more sensors and then doing something with the data.
 * 
 * A sensor is a device that measures something in the environment (e.g., temperature, humidity, sound) and 
 * converts that information into a signal based on a physical electrical property (e.g., current, voltage, resistance).
 * 
 * Here, we will read a simple sensor - a thermistor - with the Arduino using the analogRead() function.
 * 
 */


// This section (outside the setup() and loop() blocks) stores the GLOBAL VARIABLES
// These are variables that are GLOBAL in SCOPE
// Global variables are accessible to any section of the program

// DECLARE a global variable to store the pin number of the pin we will use to read temperature 
// We use an analog pin to read the thermistor
int temperaturePin = A0; 



void setup() {
  // The setup() block is a requirement of ANY Arduino program (even if it is left empty).
  // The setup() block is the first set of commands to run, and the block runs only once, before entering loop()
  

  // Open a line to the serial monitor at 9600 baud rate (bits/second).
  // The serial monitor is a useful way to get feedback on what the Arduino is doing.
  // It can be especially useful for troubleshooting
  // Note that serial communication uses digital pins 0 and 1, so we can't use them for other things
  // while serial communication is happening.
  Serial.begin(9600);  
}




void loop() {
  
  // The loop() block is a requirement of ANY Arduino program (even if it is left empty).
  // After the setup() block is run, loop() runs indefinitely.
  
  // DECLARE a LOCAL variable (LOCAL in SCOPE), a variable accessible only to the current function, here loop(). 
  // This variable, of type INT (integer), stores the raw ANALOG value that we read from the thermistor
  // using the function analogRead()
  int temperature_analog = analogRead(temperaturePin);  

  // Here we simply print our data to the Serial Monitor. 
  // Within the Arduino IDE, we can access the Serial Monitor using the square icon on the top right of the window.
  // Note that serial communication uses digital pins 0 and 1, so we can't use them for other things
  // while serial communication is happening between the Arduino and the serial monitor.
  // For command reference, see: https://www.arduino.cc/reference/en/language/functions/communication/serial/
  Serial.print("Temperature = ");
  Serial.print(temperature_analog);
  Serial.println();

  // And last, we delay for 500 ms.  Otherwise these commands would execute as fast as the Arduino could possibly run.
  delay(500);

}


