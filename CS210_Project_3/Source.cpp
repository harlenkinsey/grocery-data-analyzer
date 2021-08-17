#include <Python.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
				 
#undef max

using namespace std;


void DisplayMenu() // might display the menu
{
	cout << "Menu:" << endl;
	cout << "1. Display a List of All Items Purchased in a Day" << endl;
	cout << "2. Display the Number of Times an Item was Purchased" << endl;
	cout << "3. Display a Histogram of All Items Purchased" << endl;
	cout << "4. Exit";
	cout << endl;
}

int ValidateMenuInput() // might validate menu input
{
	int input;
	bool inputValidated = false;

	while (!inputValidated)
	{
		cout << "Enter your selection as an integer." << endl;
		cin >> input;

		if (cin.fail() || (input != 1 && input != 2 && input != 3 && input != 4)) // If input fails or is invalid
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clears the entire buffer not just one character
			cout << endl;
			cout << "Invalid input. Please try again." << endl;
			cout << endl;
			return input;
		}
		else // if input succeeds
		{
			inputValidated = true; // breaks input loop
			return input;
		}
	}
}

void FillVectors(vector<string>& itemNames, vector<int>& itemCounts)  // fills vectors using data from frequency.dat
{
	ifstream frequencyFS;
	frequencyFS.open("frequency.dat");
	string itemName;
	int itemCount;

	if (!frequencyFS.is_open())  // if file fails to open
	{
		cout << "Failed to open file: \"frequency.dat\"" << endl;
		return;
	}

	while (!frequencyFS.eof()) // while the end of file isnt reached appends name and count from frequency.dat to vectors
	{
		frequencyFS >> itemName;
		frequencyFS >> itemCount;

		itemNames.push_back(itemName);
		itemCounts.push_back(itemCount);
	}

	frequencyFS.close();

}

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python function you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


void main()
{
	bool contInput = true;
	int menuInput;
	string op2Input;

	vector<string> itemNames = vector<string>();
	vector<int> itemCounts = vector<int>();

	while (contInput) // main input loop
	{
		DisplayMenu();
		menuInput = ValidateMenuInput();

		switch (menuInput)
		{
			case 1: // outputs the counts of all items purchased

				CallProcedure("PrintItemsFrequency");

				break;

			case 2: // outputs the count of a specific item, no extra input validation is needed

				cout << "Enter item's name:" << endl;
				cin >> op2Input;
				cout << endl;
				cout << op2Input << " = " << callIntFunc("GetItemFrequency", op2Input) << endl; // outputs frequency of specific item
				cout << endl;

				break;

			case 3: // outputs histogram

				CallProcedure("CreateDataFile");
				FillVectors(itemNames, itemCounts);
				int i;
				int j;

				cout << left;
				cout << endl;

				for (i = 0; i < itemNames.size() - 1; i++) // ouputs each item name, -1 stops from outputting a duplicate value
				{
					cout << setw(15) << itemNames.at(i); // setw used for formatting 'bins'
					
					for (j = 0; j < itemCounts.at(i); j++) // outputs each item 'bin'
					{
						cout << "*";
					}

					cout << endl;
				}

				cout << endl;

				break;

			case 4:

				contInput = false; // Breaks loop and exits program

				break;

		}
	}

}