# Hardware
The following hardware was used through the development of this project:
Raspberry pi
Arduino Uno connected through USB
ZhianTec ZFM-20 Fingerprinting Sensor
  Specs: http://microcontrollershop.com/product_info.php?products_id=7224
  SetUp: https://sicherheitskritisch.de/2015/03/fingerprint-sensor-fuer-den-raspberry-pi-und-debian-linux-en/
MAXREFDES117#: HEART-RATE AND PULSE-OXIMETRY MONITOR
  Specs and SetUp: https://www.maximintegrated.com/en/design/reference-design-center/system-board/6300.html


# led_fingerprint
The led_fingerprint has all of the python files that are needed to perform any operations using the fingerprinting sensor. These .py files are called in the master_run.cpp file. This cpp file was created using a text editor in development with the python interpretor, therefore enabling the running of both cpp and python files. The set up process for developing this file can be found below:

https://www.coveros.com/calling-python-code-from-c/
https://stackoverflow.com/questions/16962430/calling-python-script-from-c-and-using-its-output
https://www.cyberciti.biz/faq/howto-compile-and-run-c-cplusplus-code-in-linux/

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
