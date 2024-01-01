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
	//Data
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

	//Remove appointment
	static void removeApps(int AppointmentID);

	//Return total number of appointments
	static int gettotal();

	//Return total number of appointments today
	static int getTodayTotal();
	//Return total number of appointments tomorrow
	static int getTmrTotal();
	//Return total number of appointments last 2 week
	static int getLast2wkTotal();
	//Return total number of appointments next 2 week
	static int getNext2wkTotal();
	static int getPastTotal();
	static int getFutureTotal();

};
#endif


//Update appointment (change status from pending to failed)
//void updateappsf();

//Update appointment (change status to pending )
//void updateappsp();

//Return appointment data(PatientID) //For machine read data
//void getappdatap(int PatientID);
//Update appointment (change status to pending )
//void updateappsp();

//Return appointment data(PatientID) //For machine read data
//void getappdatap(int PatientID);