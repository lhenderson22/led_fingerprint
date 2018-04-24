# Hardware
The following hardware was used through the development of this project:
Raspberry pi

ZhianTec ZFM-20 Fingerprinting Sensor
  Specs: http://microcontrollershop.com/product_info.php?products_id=7224
  
  
MAXREFDES117#: HEART-RATE AND PULSE-OXIMETRY MONITOR
  Specs and SetUp: https://www.maximintegrated.com/en/design/reference-design-center/system-board/6300.html

  Function Documentation for algorithm.cpp: https://os.mbed.com/teams/Maxim-Integrated/code/RD117_MBED/docs/5273ab1085ab/algorithm_8cpp.html#a62050365673e666cd0d661a3664c26e6
  
# LED
Since the documentation for the LED Pulse Oximeter MAXREFDES117# is utilizing an Arduino, the master_run.py utilizes the Pi's serial ports to call and get data from the Arduino.

Set up can be found in the product documentation

# Fingerprinter
All of the scripts to execute operations on the fingerprinter can be found in the Fingerprinter folder

SetUp: https://sicherheitskritisch.de/2015/03/fingerprint-sensor-fuer-den-raspberry-pi-und-debian-linux-en/

# Master
The Fingerprinter has all of the python files that are needed to perform any operations using the fingerprinting sensor. These .py files are called in the master_run.py file.

The order of execution is important. In order to make sure the master_run file will be able to access the data outputted from the Arduino, the Arduino code must be run first.

1) Make sure that the you have setup you system for the fingerprinter and LED pulse ox. Setp instructions can be found in the links above
2) Connect your Arduino Uno to the Pi using a USB port. Connect the pulse ox using specifications above
3) Open your Arduino IDE. Open the RD117_ARDUINO,ino file from your file directory. If a prompt asks to create another folder, accept. Compile and upload after making sure that your port and board type are correct. 
\t Note:  to see if your Arduino is connected, run >> lsusb
4) To run the master py file, navigate to the correct directory in your file system (it should be 2 folders up from the location of your RD117_ARDUINO.ino file) and execute the following command: >>python2 master_run.py

The following option menu will be seen:

Hello. Welcome to your security test. Please select one of the following options:
E: Enroll a new finger.
D: Delete old finger.
V: Validate an existing finger
I: View the index of currently enrolled fingerprints.
G: Get image of fingerprint at certain index.
C: Check Pulse Ox

E, D, V, I and G will execute their respective .py files. C will open the initialize the LED sensor.

# Errors
from pyfingerprint.pyfingerprint import PyFingerprint
ImportError: No module named 'pyfingerprint'

\t Try >> pip install pyfingerprint
\t If this doesn't work, make sure you are compiling using python2 and not python3

Pulse Ox isn't taking data after I press enter
\t Something may be off with the python-arduino interpretter. Just CNTRL-C and restart the test or wait until data starts taking

Traceback (most recent call last):
  File "master_run.py", line 56, in <module>
    str1,spo,str2, hr1  = output.split(b' ')
ValueError: need more than 2 values to unpack
  
\t CNTRL-C and restart test

