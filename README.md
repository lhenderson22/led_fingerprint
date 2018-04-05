# Hardware
The following hardware was used through the development of this project:
Raspberry pi

ZhianTec ZFM-20 Fingerprinting Sensor
  Specs: http://microcontrollershop.com/product_info.php?products_id=7224
  SetUp: https://sicherheitskritisch.de/2015/03/fingerprint-sensor-fuer-den-raspberry-pi-und-debian-linux-en/
  
MAXREFDES117#: HEART-RATE AND PULSE-OXIMETRY MONITOR
  Specs and SetUp: https://www.maximintegrated.com/en/design/reference-design-center/system-board/6300.html

  Function Documentation for algorithm.cpp: https://os.mbed.com/teams/Maxim-Integrated/code/RD117_MBED/docs/5273ab1085ab/algorithm_8cpp.html#a62050365673e666cd0d661a3664c26e6
  
# Software
# LED
Since the documentation for the LED Pulse Oximeter MAXREFDES117# is utilizing an Arduino and this project uses a Raspberry Pi, the LED was connected using Raspberry Pi's I2C interface. Installation instructions for I2C interface is found bleow:

  https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial#i2c-on-pi

The following two resources were used for connecting the LED to the Pi:
  Instructions: https://forum.dexterindustries.com/t/maxrefdes117-heart-rate-and-pulse-oximetry-monitor/3602/2
  Raspberry Pi pinouts: https://www.jameco.com/Jameco/workshop/circuitnotes/raspberry-pi-circuit-note.html

Using this documentation the following connections were made:
MAXREFDES117#     Raspberry Pi (Pin on Pinout Diagram)
VIN           --> POWER 5V (2)
SDA           --> SDA1I2C (3)
SCL           --> SCL1I2C (5)
INT           --> GROUND (39)

After connecting the LED, [i2cdetect -y 1] will detect the address for the LED registers. If multiple devices are connected, run the command before and after connecting the LED.

The following addressing bit number can be found using the following specifications:

  https://www.totalphase.com/support/articles/200349176-7-bit-8-bit-and-10-bit-I2C-Slave-Addressing
  
This will determine the read/write address for the LED. For our example, the LED is located at address 0x57 and is therefore in 7-bit addressing mode.


# Fingerprinter

# Master
The led_fingerprint has all of the python files that are needed to perform any operations using the fingerprinting sensor. These .py files are called in the master_run.cpp file. This cpp file was created using a text editor in development with the python interpretor, therefore enabling the running of both cpp and python files. The set up process for developing this file can be found below:

  Calling Python from a Cpp File: https://www.coveros.com/calling-python-code-from-c/
                                  https://stackoverflow.com/questions/16962430/calling-python-script-from-c-and-using-its-output
  High Level Description of Python Compiler Functions: https://docs.python.org/2/c-api/veryhigh.html#c.PyCompilerFlags

To run the master cpp file, navigate to the correct directory in your file system and execute the following command:

g++ -I/usr/include/python2.7 -lpython2.7 -o exec master_run.cpp
./exec

The following option menu will be seen:

Hello. Welcome to your security test. Please select one of the following options:
E: Enroll a new finger.
D: Delete old finger.
V: Validate an existing finger
I: View the index of currently enrolled fingerprints.
G: Get image of fingerprint at certain index.
C: Check Pulse Ox

E, D, V, I and G will execute their respective .py files. C will open the gtkterm.
