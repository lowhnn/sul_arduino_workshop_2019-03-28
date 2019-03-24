/*
 * Workshop: Automate and Enhance Your Research with Practical Electronics Using Arduino
 * 
 * March 28, 2019 at Stanford University Libraries
 * 
 * Developed by Dr. Paul Leary, and Dr. Natalie Low
 * 
 * 
 * 
 * Lesson 5 -  Logging sensor data to an SD card 
 * 
 * Now that we have successfully obtained temperature values from our thermistor, we can start to
 * do something with those values. One of the most basic things to do with sensor data is simply 
 * to log it to a permanent storage device, like an SD card.
 * 
 * Here, we will record our temperature data to an microSD card. We are using an microSD card 
 * reader breakout board from Adafruit (https://www.adafruit.com/product/254). Breakout boards 
 * are user-friendly 'units' that usually take an electronic component and bundle it with all 
 * the necessary associated components so it is ready to use by just connecting the pins to 
 * the Arduino.
 * 
 * In this case, the breakout board contains the microSD card reader, but also a 3.3V-5V logic
 * shifter, a resistor, two capacitors, and an indicator LED - which are all the things you need
 * to make the card reader work.
 * 
 * The Arduino communicates with the SD card via the SPI (Serial Peripheral Interface) protocol.
 * This communication protocol uses 4 wires between the Arduino and the card reader:
 * 
 * 1. SCLK (Arduino pin 13): the 'serial clock' line ensures that both devices are communicating
 * at the same frequency.
 * 2. MISO or DO (Arduino pin 12): this line transmits information from Arduino to SD card.
 * 3. MOSI or DI (Arduino pin 11): this line transmits information from SD card to Arduino.
 * 4. CS or SS (user defined, we will use pin 10): the 'chip select' line - since the Arduino 
 * can communicate with more than one SPI device, this is how it selects which device it is
 * communicating with at a given time. If we were using additional SPI devices, like a real-time 
 * clock (RTC), or an ethernet chip, each device would be connected to its own CS pin on the 
 * Arduino.
 * 
 * 
 * 
 * See: https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial
 * 
 * 
 */


// Here we are using a software library for reading and writing to SD cards
#include <SdFat.h>

SdFat sd;
SdFile myFile;


// Here we define our pin numbers as global variables
int temperaturePin = A0; 
int sdPin = 10; // define the chip select pin for the SD card reader


void setup() {
  
  // Initialize serial monitor at 9600 baud
  Serial.begin(9600);  
  
  // Initialize the SD card using Arduino pin 10 as the chip select (CS)
  if (!sd.begin(sdPin)) sd.initErrorHalt();
    
}



void loop() {

  // Burst sample and average the sample values (Lesson 4)
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


  // Write the temperature to the SD card

  if (!myFile.open("TEMPLOG.TXT", O_RDWR | O_CREAT | O_AT_END)) { // open or create the file 'TEMPLOG.TXT' on the SD card
    sd.errorHalt("failed to open TEMPLOG.TXT"); // return error message if we fail to open the file
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to file...");
  myFile.print("Temperature = ");
  myFile.print(temperature_C);
  myFile.write(0xC2); //These 2 lines allow us to print the ° symbol. If you are curious,
  myFile.write(0xB0); //they involve calling a special character directly from the ASCII table http://www.asciitable.com/
  myFile.println("C ");

  // close the file:
  myFile.close();
  Serial.println("done.");

  
   
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


