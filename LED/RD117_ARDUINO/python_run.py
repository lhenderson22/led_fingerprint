import serial

ser = serial.Serial('/dev/ttyACM0',115200)
s = [0]
count = 0
#initialize the sensor and wait for it to get a print(count)
while (count < 60):
	read_serial=ser.readline()
	s[0] = ser.readline()
	print(s[0])
	print(read_serial)
	print(count)
	count = count + 1
