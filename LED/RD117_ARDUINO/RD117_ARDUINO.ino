/** \file RD117_LILYPAD.ino ******************************************************

  Project: MAXREFDES117#
  Filename: RD117_LILYPAD.ino
  Description: This module contains the Main application for the MAXREFDES117 example program.

  Revision History:
  \n 1-18-2016 Rev 01.00 GL Initial release.
  \n

  --------------------------------------------------------------------

  This code follows the following naming conventions:

  char              ch_pmod_value
  char (array)      s_pmod_s_string[16]
  float             f_pmod_value
  int32_t           n_pmod_value
  int32_t (array)   an_pmod_value[16]
  int16_t           w_pmod_value
  int16_t (array)   aw_pmod_value[16]
  uint16_t          uw_pmod_value
  uint16_t (array)  auw_pmod_value[16]
  uint8_t           uch_pmod_value
  uint8_t (array)   auch_pmod_buffer[16]
  uint32_t          un_pmod_value
  int32_t *         pn_pmod_value

  ------------------------------------------------------------------------- */
/*******************************************************************************
  Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  Except as contained in this notice, the name of Maxim Integrated
  Products, Inc. shall not be used except as stated in the Maxim Integrated
  Products, Inc. Branding Policy.

  The mere transfer of this software does not imply any licenses
  of trade secrets, proprietary technology, copyrights, patents,
  trademarks, maskwork rights, or any other form of intellectual
  property whatsoever. Maxim Integrated Products, Inc. retains all
  ownership rights.
*******************************************************************************
*/
/*!\mainpage Main Page

  \section intro_sec Introduction

  This is the code documentation for the MAXREFDES117# subsystem reference design.

   The Files page contains the File List page and the Globals page.

   The Globals page contains the Functions, Variables, and Macros sub-pages.

  \image html MAXREFDES117_Block_Diagram.png "MAXREFDES117# System Block Diagram"

  \image html MAXREFDES117_firmware_Flowchart.png "MAXREFDES117# Firmware Flowchart"

*/
#include <Arduino.h>
#include "algorithm.h"
#include "max30102.h"

//if Adafruit Flora development board is chosen, include NeoPixel library and define an NeoPixel object
//#if defined(ARDUINO_AVR_FLORA8)
//#include "adafruit_neopixel.h"
//#define BRIGHTNESS_DIVISOR 8  //to lower the max brightness of the neopixel LED
//Adafruit_NeoPixel LED = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);
//#endif

#define MAX_BRIGHTNESS 255

//#if defined(ARDUINO_AVR_UNO)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated.  Samples become 16-bit data.
uint16_t aun_ir_buffer[100]; //infrared LED sensor data
uint16_t aun_red_buffer[100];  //red LED sensor data
uint32_t n_spo2_values[20]; //array for storing spo values
uint32_t n_heart_rate_values[10]; //array for storing heart rate values
int32_t counting_spo2 = 0; //counter for fake v.s. real spo values
int32_t counting_hr = 0; //counter for fake v.s real HR values

//#else
//uint32_t aun_ir_buffer[100]; //infrared LED sensor data
//uint32_t aun_red_buffer[100];  //red LED sensor data
//#endif

int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;


// the setup routine runs once when you press reset:
void setup() {

  maxim_max30102_reset(); //resets the MAX30102
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pinMode(10, INPUT);  //pin D10 connects to the interrupt output pin of the MAX30102
  delay(1000);
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_dummy); //Reads/clears the interrupt status register
  maxim_max30102_init();  //initialize the MAX30102
}

// the loop routine runs over and over again forever:
void loop() {
  
  uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
  int32_t i;
  float f_temp;

  un_brightness = 0;
  un_min = 0x3FFFF;
  un_max = 0;

  n_ir_buffer_length = 80; //buffer length of 100 stores 4 seconds of samples running at 25sps
  Serial.print(F("New Test"));
  //read the first 100 samples, and determine the signal range
  for (i = 0; i < n_ir_buffer_length; i++)
  {
    while (digitalRead(10) == 1); //wait until the interrupt pin asserts
    maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i)); //read from MAX30102 FIFO

    if (un_min > aun_red_buffer[i])
      un_min = aun_red_buffer[i]; //update signal min
    if (un_max < aun_red_buffer[i])
      un_max = aun_red_buffer[i]; //update signal max
    //Serial.print(F("red="));
    //Serial.print(aun_red_buffer[i], DEC);
    //Serial.print(F(", ir="));
    //Serial.println(aun_ir_buffer[i], DEC);
    //if(i%10 == 0)
      //Serial.print(F("."));
  }
  
  un_prev_data = aun_red_buffer[i];
  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
  
  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  int32_t k = 0;
  //Serial.print(F("Getting SPO and Heart Rate"));
  for (k = 0 ; k < 20 ; k++)
  {
    
    i = 0;
    un_min = 0x3FFFF;
    un_max = 0;
    //if(k < 5) {
      //Serial.print(F("."));
    //}
    //adding hr and spo values to respective arrays
    if(k > 4) {
      //Serial.println(F("counting numbers are"));
      //Serial.println(counting_spo2);
      //Serial.println(counting_hr);
      n_spo2_values[k - 5] = n_spo2;
      if(n_spo2 > 110 || n_spo2 < 85)
        --counting_spo2;
      else
        ++counting_spo2;
      Serial.print(F("SPO: "));
      Serial.print(n_spo2);
      //Serial.print(F(" to index "));
      //Serial.print(k-5);
      //Serial.print(F(" and counting_spo is "));
      //Serial.println(counting_spo2);
      n_heart_rate_values[k - 5] = n_heart_rate;
      if(n_heart_rate > 146 || n_heart_rate < 40)
        --counting_hr;
      else
        ++counting_hr;
      Serial.print(F(" HR: "));
      Serial.print(n_heart_rate);
      Serial.print(F("\n"));
      //Serial.print(F(" to index "));
      //Serial.print(k-5);
      //Serial.print(F(" and counting_hr is "));
      //Serial.println(counting_hr);
    }
    
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (i = 25; i < 100; i++)
    {
      aun_red_buffer[i - 25] = aun_red_buffer[i];
      aun_ir_buffer[i - 25] = aun_ir_buffer[i];

      //update the signal min and max
      if (un_min > aun_red_buffer[i])
        un_min = aun_red_buffer[i];
      if (un_max < aun_red_buffer[i])
        un_max = aun_red_buffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (i = 75; i < 100; i++)
    {
      un_prev_data = aun_red_buffer[i - 1];
      while (digitalRead(10) == 1);
      digitalWrite(9, !digitalRead(9));
      maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

      //calculate the brightness of the LED
      if (aun_red_buffer[i] > un_prev_data)
      {
        f_temp = aun_red_buffer[i] - un_prev_data;
        f_temp /= (un_max - un_min);
        f_temp *= MAX_BRIGHTNESS;
        f_temp = un_brightness - f_temp;
        if (f_temp < 0)
          un_brightness = 0;
        else
          un_brightness = (int)f_temp;
      }
      else
      {
        f_temp = un_prev_data - aun_red_buffer[i];
        f_temp /= (un_max - un_min);
        f_temp *= MAX_BRIGHTNESS;
        un_brightness += (int)f_temp;
        if (un_brightness > MAX_BRIGHTNESS)
          un_brightness = MAX_BRIGHTNESS;
      }
    }
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
    
  }
}

