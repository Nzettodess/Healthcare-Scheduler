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

#ifndef Appointments_H
#define Appointments_H

class Appointments
{
public:
	int AppointmentID, PatientID, DoctorID;
	string ADate, ATime,  AStatus;

	//Constructor
	Appointments();
	Appointments(sql::ResultSet* data);

	//Insert for account patients
	void insertapp(string AStatus);

	//Genaral update
	void updateapp(string AStatus, int AppointmentID);

	//Update appointment (change status from pending to succeed)
	void updateapps(string AStatus, int AppointmentID);

	//Return appointment data(AppointmentID) //For machine read data
	void getappdataa(int AppointmentID);
	void getappdatap(int PatientID);

	//Return appointment data //For user dynamic query
	static vector<Appointments> getapplistp(int PatientID, string sortColumn, bool ascending, bool isAll);

	static vector<Appointments> getapplistd(int PatientID, string sortColumn, bool ascending, bool isAll);

	static vector<Appointments> getappdatapd(int PatientID, int DoctorID);

	static vector<Appointments> getapplistall(string sortColumn, bool ascending);

	static void removeApps(int AppointmentID);

	static int gettotal();

	static int getTodayTotal();
	static int getTmrTotal();
	static int getLast2wkTotal();
	static int getNext2wkTotal();
};
#endif


//Update appointment (change status from pending to failed)
//void updateappsf();

//Update appointment (change status to pending )
//void updateappsp();

//Return appointment data(PatientID) //For machine read data
//void getappdatap(int PatientID);

