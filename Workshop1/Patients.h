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
#include <array>
using namespace std;

#ifndef Patients_H
#define Patients_H

class Patients
{
public:
	int PatientID, AccountID;
	string PName, PTelephoneNo, Gender, DateOfBirth;
	double Height, Weight;

	//Constructor
	Patients();
	Patients(sql::ResultSet* data);

	//Insert for profile patient
	void insertp();

	//Update profile patient
	void updatep();

	//Return patient profile data (using accountID)
	void getpdataa(int AccountID);

	//Return doctor profile data (using patientID)
	void getpdatap(int PatientID);

	static vector<Patients> getplist(string keyword, string sortColumn, bool ascending);

	static void removePatient(int PatientID);

	static int* getAgeGroup();
	static int* getBMIGroup();

	static int gettotal();
};
#endif
