#Some notes and useful resources for your Arduino projects 

Natalie Low

Updated: 2019-03-27

----

### Where to get stuff:

[Sparkfun Electronics] (www.sparkfun.com) and [Adafruit Industries] (www.adafruit.com) supply electronics components and breakout boards to the hobbyist market. They are a great source of user-friendly parts that often come with good online tutorials.

[Digi-Key Electronics] (www.digikey.com), [Newark] (www.newark.com), and [Mouser Electronics] (www.mouser.com) are good sources of electronics components.
Stanford has accounts with these suppliers. From my experience, Newark is the fastest shipper if you need something in a hurry.


---

### Sensors:

There are a ton of different sensors out there - check out the selection at [Sparkfun] (https://www.sparkfun.com/categories/23) and [Adafruit] (https://www.adafruit.com/category/35), many of which come as breakout boards and have tutorials/libraries that make them easy to work with.

[Vernier Software and Technology] (https://www.vernier.com/) makes a variety of pricier but good sensors that use a 0-5 volt output and are are Arduino-compatible. I've made extensive use of their optical dissolved oxygen sensors in my work. Sparkfun carries an [Arduino shield] (https://www.sparkfun.com/products/12858), as well as [individual female connectors] (https://www.sparkfun.com/products/12754), that are compatible with the Vernier sensors.

For higher-precision applications, you may find that the built-in 10-bit ADC on the Arduino does not give you enough resolution for your sensors. You can find ADCs with [12-bit] (https://www.adafruit.com/product/1083) or [16-bit] (https://www.adafruit.com/product/1085) resolutions to use in your projects.


---

### Actuators: 

Many actuators (e.g., heaters, chillers, pumps) run on 110-volt AC power. The easiest way to control these without doing a bunch of DIY high-voltage wiring is with a ready-made [AC power relay] (https://www.adafruit.com/product/2935). You can also use solid-state relays [like this one](https://www.newark.com/opto-22/z240d10/ssr-panel-mount-280vac-32vdc-10a/dp/18M9766) to wire up a more compact system - if you are interested in working with AC wiring in this way, I highly recommend checking out the electrical safety training module offered by Stanford EH&S.






---

### Logging, visualizing, and streaming data:

Adafruit makes a [basic datalogging shield] (https://www.adafruit.com/product/1141) that contains an SD card reader and a DS1307 Real Time Clock, which is great for logging timestamped data. They also have a detailed [tutorial] (https://learn.adafruit.com/adafruit-data-logger-shield) on how to use the shield.

LCD screens (e.g., [1](https://www.sparkfun.com/products/255), [2](https://www.adafruit.com/product/198)) are a good way to get visual feedback on sensor readings or on the current status of your system. A standard LCD screen uses a lot of Arduino output pins, but you can use something like [this breakout board](https://www.adafruit.com/product/292) to control an LCD using the I2C or SPI protocols, which greatly reduces pin usage.


[ThingSpeak] (www.thingspeak.com) is a cool platform that allows data to be sent to an online datastream via HTTP request, so you can check on things from your phone or laptop without having to come into the lab. They also have an app called ThingTweet, which allows you to tweet using an Arduino. Since you can follow a Twitter account using a mobile phone, it essentially allows you to set up text alerts. You'll need to connect your Arduino to the internet using something like an [Ethernet] or [WiFi] shield.

---



### Beyond the Arduino Uno:
The Uno is the most common Arduino board out there, but there are also [a range of other Arduino boards] (https://www.arduino.cc/en/Main/Products) with different specifications.

There are also plenty of other microcontroller boards that are compatible with the Arduino IDE. For example, Adafruit has a range of small, stackable boards called the [Adafruit Feather](https://www.adafruit.com/category/943), and the [Teensy] (https://www.pjrc.com/teensy) packs a lot of processing power into a small package.

---

### Making custom shields and boards

If size/space is at a premium (e.g., small data loggers that need to fit in a housing or on a bird), or if you need 20 of a circuit, designing your own printed circuit board might be the way to go.

My preferred software for PCB design is [EAGLE] (https://www.autodesk.com/products/eagle/free-download) - their free version is generally sufficient for most purposes. Sparkfun has a [nice set of tutorials] (https://learn.sparkfun.com/tutorials/tags/eagle) on how to design boards in EAGLE. 

[Fritzing] (http://fritzing.org/home/) is another option that gives you the option of  switching between a breadboarded circuit, circuit diagram, and PCB. The breadboard diagrams in this workshop were made in Fritzing.

I have had good success getting boards printed using [OSHpark] (oshpark.com) - they do small-scale print runs (minimum of 3 copies) and the turnaround is generally <2 weeks. 







