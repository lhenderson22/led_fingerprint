#include<stdio.h>
#include </home/pi/LED_Print/master_file/Python.h>
/* demo.c:  My first C program on a Linux */
int main(void)
{
 printf("Hello my friend! This is a test program.\n");
 // Initialize the Python interpreter.
 Py_Initialize();
 printf("Python has been initialized\n");
PyRun_SimpleString("import sys");
printf("Python has been imported\n");
PyRun_SimpleString('sys.path.append("/home/pi/LED_Print/master_file")')
printf("Plugin has been initialized\n");
 return 0;
}