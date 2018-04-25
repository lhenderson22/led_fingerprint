import serial
import struct
import sys

##Note: this script is meant to be run on a python2 compiler
ser = serial.Serial('/dev/ttyACM0',115200)
options = ["e", "v", "i", "g", "c"]

print("Hello. Welcome to your security test.")
print("Please select one of the following options:")
print("E: Enroll a new finger.\nD: Delete old finger.")
print("V: Validate an existing finger.")
print("I: View the index of currently enrolled fingerprints.")
print("G: Get image of fingerprint at certain index.")
print("C: Check Pulse Ox (SPO) and Heart Rate.")




while True:
        selection = raw_input('\nPlease enter an option: ')
        if (selection.lower == options[0].lower):
                ## Enroll finger python script
                print("\nSelection: Enroll finger\n\n")
                sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')
                import example_enroll
                break
        elif (selection.lower == options[1].lower):
                ## Validate finger python script
                print("\nSelection: Validate finger\n\n")
                sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')
                import example_search
                break
        elif (selection.lower == options[2].lower):
                ## View index python script
                print("\nSelection: View index\n\n")
                sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')
                import example_index
                break
        elif (selection.lower == options[3].lower):
                ## Get image python script
                print("\nSelection: Get image\n\n")
                sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')
                import example_downloadimage
                break
        elif (selection.lower == options[4].lower):
                ## Get pulse ox by interfacing with Arduino
                print("\nSelection: SPO and HR\n\n")
                while True:
                        ## Wait for finger to be placed on sensor
                        enter = raw_input("Press Enter when finger is on sensor\n\n")
                        
                        #initialize the sensor and wait for it to get a SPO/HR value
                        while True:
                                ## Get output from Arduino
                                output=ser.readline()
                                print("Establishing buffer\n")
                                print("\t This could take up to 10 seconds\n")
                                ## Wait for a new test
                                while b'New Test' not in output:
                                        output=ser.readline()
                                
                                count = 8
                                totalSamples = count
                                SPOs = [0, 0, 0, 0, 0, 0, 0, 0]
                                HRs = [0, 0, 0, 0, 0, 0, 0, 0]
                                ## Take 8 samples of SPO and HR
                                print("Getting SPO and Heart Rate\n")
                                print("\t This could take up to 10 seconds\n")
                                while(count > 0):
                                        output=ser.readline()
                                        if(b'New Test' not in output):
                                                str1,spo,str2, hr1  = output.split(b' ')
                                                ## Get the HR depending on whether it is below or above 100
                                                if len(hr1) == 4:
                                                        hr = hr1[0:3]
                                                else:
                                                        hr = hr1[0:2]
                                                #print("SPO is " , spo.decode('UTF-8'))
                                                #print("HR is ", hr.decode('UTF-8'))

                                                ##fill arrays with the values
                                                SPOs[count-1] = spo.decode('UTF-8')
                                                HRs[count-1] = hr.decode('UTF-8')
                                        print(count)
                                        count = count - 1
                                #print(SPOs)
                                #print(HRs)
                                i = 0
                                SPOsum = 0 #summer for SPOs
                                SPOcount = 0;
                                HRsum = 0; #summer for HRs
                                HRcount = 0 
                                while i < len(SPOs):
                                        ## if the pulse ox is in the range 95-100
                                        if int(SPOs[i]) > 94 and int(SPOs[i]) < 101:
                                                SPOsum = SPOsum + int(SPOs[i])
                                                SPOcount = SPOcount + 1
                                        ## if the heart rate is within the rant 55-140
                                        if int(HRs[i]) > 54 and int(HRs[i]) < 141:
                                                HRsum = HRsum + int(HRs[i])
                                                HRcount = HRcount + 1
                                        i = i + 1;
                                #print(HRsum, SPOsum)
                                #print(HRcount, SPOcount)

                                ## the amount of valid reading must be over half total samples
                                if SPOcount > (totalSamples/2) and HRcount > (totalSamples/2):
                                        aveSPO = SPOsum/SPOcount
                                        aveHR = HRsum/ HRcount
                                        print("Accepted")
                                        print "Average SPO is %u and average HR is %u" %(aveSPO, aveHR)
                                else:
                                        print("Not enough valid pulse ox samples to be considered valid")

                                break
                        break
                break
        else:
                print("Invalid entry. Please try again.")
