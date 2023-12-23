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
#include "Appointments.h"
#include "wintoastlib.h"
#include "Toast.h"

//using namespace WinToastLib;
using namespace std;

#ifndef Notifications_H
#define Notifications_H

class Notifications
{
public:
	int 	NotificationID, AppointmentID;
	string Reason, NStatus;

	//Constructor
	Notifications();
	Notifications(sql::ResultSet* data);

	static void updatens(string NStatus, int NotificationID);
	void getndataapp(int AppointmentID);

	//For Patients
	static void conditioncheckerp(chrono::minutes::rep minutesDifference, string AStatus, string NStatus, string Reason, string ADate, string ATime, int DoctorID, string DName, int NotificationID, int AppointmentIDa, int AppointmentIDn);
	/*static void sendtimemsgp(string ADate, string ATime, int DoctorID, string DName);
	static void sendstatusmsgp(int AppointmentID, string ADate, string ATime, string AStatus, int DoctorID, string DName);*/

	//For Doctors
	static void conditioncheckerd(chrono::minutes::rep minutesDifference, string AStatus, string NStatus, string Reason, string ADate, string ATime, int PatientID, string PName, int NotificationID, int AppointmentID);

	static vector<Notifications> getNdataApp(int AppointmentID);
};
#endif