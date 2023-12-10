#include <string>
#include <ctime>
#include <vector> 
#include <chrono>
#include <conio.h>
#include <dos.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <thread>
#include <windows.h>
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>


using namespace std;

#ifndef Doctors_H
#define Doctors_H

class Doctors
{
public:
	int DoctorID, DepartmentID, AccountID;
	string DName, DTelephoneNo, Specialization;

	//Constructor
	Doctors();
	Doctors(sql::ResultSet* data);

	//Insert for account patients
	void insertd();

	//Update account (Password)
	void updated();

	void getddataa(int AccountID);
	void getddatad(int DoctorID);

	static vector<Doctors> getdlist(string keyword, string sortColumn, bool ascending);

	static void removeDoctor(int DoctorID);

	double getbestd();
	double getworstd();
	static int gettotal();
};
#endif
//Return doctor list
//void getdlist();