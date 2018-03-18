//#include "stdafx.h"
//#include "pyembed.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <Python.h>
using namespace std;

char ask_input() {
	std::cout << "Hello. Welcome to your security test. Please select one of the following options:" << std::endl;
	std::cout << "E: Enroll a new finger." << std::endl;
	std::cout << "D: Delete old finger." << std::endl;
	std::cout << "V: Validate an existing finger\n";
	std::cout << "I: View the index of currently enrolled fingerprints." << std::endl;
	std::cout << "G: Get image of fingerprint at certain index." << std::endl;
	std::cout << "C: Check Pulse Ox\n";
	char option;
	std::cin >> option;
	return option;
}
main(int argc, char** argv)
{
	//Prompt user response
	char choice = ask_input();

	std::cout << choice << endl;
	switch(tolower(choice)) {
		case 'e':
			Py_Initialize();
			PyRun_SimpleString("print 'Running Python Enroll'\n");
			PyRun_SimpleString( "import sys; sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')\n");
			PyRun_SimpleString( "import example_enroll\n");
			Py_Finalize();
			break;
		case 'd':
			Py_Initialize();
			PyRun_SimpleString("print 'Running Python Delete'\n");
			PyRun_SimpleString( "import sys; sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')\n");
			PyRun_SimpleString( "import example_delete\n");
			Py_Finalize();
			break;
		case 'v':
			Py_Initialize();
			PyRun_SimpleString("print 'Running Python Search'\n");
			PyRun_SimpleString( "import sys; sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')\n");
			PyRun_SimpleString( "import example_search\n");
			Py_Finalize();
			break;
		case 'i':
			Py_Initialize();
			PyRun_SimpleString("print 'Running Python Index'\n");
			PyRun_SimpleString( "import sys; sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')\n");
			PyRun_SimpleString( "import example_index\n");
			Py_Finalize();
			break;
		case 'g':
			Py_Initialize();
			PyRun_SimpleString("print 'Running Python Get Image'\n");
			PyRun_SimpleString( "import sys; sys.path.insert(0,'/home/pi/LED_Print/python-fingerprint/examples')\n");
			PyRun_SimpleString( "import example_downloadimage\n");
			Py_Finalize();
			break;
		case 'c':
			printf ("Checking if processor is available...");
  			if (system(NULL)) puts ("Ok");
    			else exit (EXIT_FAILURE);
  			printf ("Opening GTKTERM...\n");
  			system ("gtkterm");
			break;
		default:
			std::cout << "You have chosen something incorrect" << std::endl;
			break;
	}
	return 0;
 }
