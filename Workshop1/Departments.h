#include "DatabaseConnector.h"
#include "DatabaseConnector.h"
#include <chrono>
#include <conio.h>
#include <conio.h>
#include <ctime>
#include <ctime>
#include <dos.h>
#include <iomanip>
#include <iostream>
#include <mysql/jdbc.h>
#include <mysql/jdbc.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <string>
#include <thread>
#include <time.h>
#include <vector> 
#include <vector> 
#include <windows.h>
using namespace std;

#ifndef Departments_H
#define Departments_H

class Departments
{
public:
	int DepartmentID;
	string DptName, Description;

	//Constructor
	Departments();
	Departments(sql::ResultSet* data);

	//Insert for departments detail 
	void insertdpt();

	//Update for departments detail 
	void updatedpt();

	//Remove departments
	void removeDpt();

	//Query department
	int dptidquery(string DptName);

	static vector<Departments> getdptlist(string keyword, string sortColumn, bool ascending);

	static void removeDpt(int DepartmentID);

	void getdptdatadpt(int DepartmentID);

	static int* getDptDcount();

	static int gettotal();
};
#endif
