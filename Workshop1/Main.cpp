//system header
#include <cctype>  
#include <chrono>
#include <conio.h>
#include <cstdlib> 
#include <ctime>
#include <dos.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mysql/jdbc.h>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <thread>
#include <time.h>
#include <windows.h>

//project header
#include "Accounts.h"
#include "Appointments.h"
#include "DatabaseConnector.h"
#include "Departments.h"
#include "Doctors.h"
#include "Feedback.h"
#include "Menu.h"
#include "Notifications.h"
#include "Patients.h"
#include "Timer.h"
#include "Toast.h"
#include "wintoastlib.h"

using namespace WinToastLib;
using namespace std;
using namespace sql;


//Function Declaration

//Main Menu
void mainMenu();

//Register
void registerAccount();
void registerPatientProfile(int AccountID);
void registerDoctorProfile(int AccountID);
int tempDepartmentID = -1;
void listDepartments();

//Login
void loginAccount(int Role);

//Home
void homePatient(Accounts user, Patients p);
void homeDoctor(Accounts user, Doctors d);
void homeAdmin(Accounts user);

//Patients utility menu
Patients patientProfileMenu(Accounts user, Patients p);
Accounts accountEditMenu(Accounts user, Patients p);
void patientAppointmentMenu(Accounts a, Patients p);
void addAppointmentMenu(Accounts a, Patients p);
void updateAppointmentMenu(int AppointmentID, Accounts a, Patients p);
void feedbackMenu(Accounts a, Patients p);
void addFeedbackMenu(Accounts a, Patients p);
void updateFeedbackMenu(int FeedbackID, Accounts a, Patients p);
void viewDoctorList(Accounts a, Patients p);
void viewDepartmentList(Accounts a, Patients p);
void patientNotificationChecker(Accounts a, Patients p);

//Doctor utility menu
void doctorNotificationChecker(Accounts adnc, Doctors ddnc);
Doctors doctorProfileMenu(Accounts user, Doctors d);
Accounts accountEditMenu(Accounts user, Doctors d);
void doctorAppointmentMenu(Accounts a, Doctors d);
void addAppointmentMenu(Accounts a, Doctors d, int PatientID);
Patients updatePatientProfileMenu(Accounts a, Doctors d, int PatientID);
void feedbackMenu(Accounts a, Doctors d);

//Admin utility menu 
void accountsManager();
void addAccounts();
void accountEditMenu(int AccountID);
void doctorsManager();
void addDoctors();
void doctorProfileMenu(int DoctorID);
void patientsManager();
void addPatients();
void patientProfileMenu(int PatientID);
void departmentsManager();
void addDepartments();
void updateDepartmentMenu(int DepartmentID);
void appointmentsManager();
void addAppointments();
void updateAppointments(int AppointmentID);
void feedbackManager();
void addFeedback();
void updateFeedback(int FeedbackID);
void statistics();


void doctorList();
int doctorSelected = -1;
void patientList();
int patientSelected = -1;
void accountList();
int accountSelected = -1;

//Miscellaneous Functions (Input validation)
bool hasAlphabet(const string& s);
bool hasDigit(const string& s);
bool validateFormat(const string& input, const string& formatRegex);
bool isValidDate(const string& year, const string& month, const string& day);
bool isValidDOB(const std::string& year, const std::string& month, const std::string& day);
bool isValidTime(const string& hours, const string& minutes, const string& seconds);
bool isValidRatings(int Ratings);
bool isNumeric(const std::string& str);
bool isLeapYear(int year);

//Function Defininition

//Register and Login
void registerAccount() {
	Accounts accreg;
	bool isPatient = true;

	//Register Menu 1 (Into Accounts)
	Menu RM1;
	RM1.header = "Account Registration\n\n";
	RM1.addOption("->> Username (for Login)");
	RM1.addOption("->> Password (for Login)");
	RM1.addOption("->> Toggle to Change Your Role");
	RM1.setValue(2, "Patient");
	RM1.addOption("->> Register Account");
	RM1.addOption("->> Back");
	RM1.footer = "\nEnter the data required accordingly.";

	while (1) {

		//Toogle function
		if (isPatient) {
			RM1.setValue(2, "Patient");
			accreg.ARole = 2;
		}
		else {
			RM1.setValue(2, "Doctor");
			accreg.ARole = 1;
		}

		switch (RM1.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> accreg.Username;

			if (accreg.Username.length() < 5) {
				cout << "Username must be at least 5 characters.";
				_getch();
				break;
			}

			if (accreg.usernameExisted(accreg.Username) == true) {
				cout << "This username is already in use. Please try a different one.";
				_getch();
				break;
			}
			RM1.setValue(0, accreg.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> accreg.Password;
			if (accreg.Password.length() < 8) {
				cout << "Password must be at least 8 characters.";
				_getch();
				break;
			}

			if (hasAlphabet(accreg.Password) == true && hasDigit(accreg.Password) == true) {
				RM1.setValue(1, accreg.Password);
				break;
			}
			else {
				cout << "The password needs to be made up of both numbers and letters.";
				_getch();
				break;
			}
			
			break;

		case 3:
			isPatient = !isPatient;
			break;

		case 4:
			
			if (accreg.Username.empty() || accreg.Password.empty()) {
				cout << "Username and Password cannot be NULl or Space!!";
				_getch();
				break;
			}
			else {
				accreg.AccountID = accreg.insertacc();
			
			}
							
			if (isPatient) {
				registerPatientProfile(accreg.AccountID);
			}
			else {
				registerDoctorProfile(accreg.AccountID);
			}
			break;

		case 5:
			return;

		default:
			break;
		}
	}
}
void registerPatientProfile(int AccountID) {

	Patients profilep;
	
	ostringstream weightStream, heightStream;
	bool isMale = true;
	profilep.AccountID = AccountID;
	//Register Menu 2 (Into Patient)
	Menu RM2;
	RM2.header = "Patient Profile Registration\n\n";
	RM2.addOption("->> Name [Required]");
	RM2.addOption("->> Mobile Number (010-0000000) [Required]");
	RM2.addOption("->> Toggle to Change Your Gender");
	RM2.setValue(2, "Male");
	RM2.addOption("->> Date of Birth (YYYY-MM-DD) [Required]");
	RM2.addOption("->> Height (cm) [Optional]");
	RM2.addOption("->> Weight (kg) [Optional]");
	RM2.addOption("->> Confirm");
	RM2.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		//Toogle function
		if (isMale) {
			RM2.setValue(2, "Male");
			profilep.Gender = "Male";
		}
		else {
			RM2.setValue(2, "Female");
			profilep.Gender = "Female";
		}

		switch (RM2.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> profilep.PName;
			RM2.setValue(0, profilep.PName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> profilep.PTelephoneNo;
			if (validateFormat(profilep.PTelephoneNo, R"(\d{3}-\d{7})") && profilep.PTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				RM2.setValue(1, profilep.PTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;
			}
			break;

		case 3:
			isMale = !isMale;
			break;

		case 4:
			cout << "Please enter your date of birth (YYYY-MM-DD): ";
			cin >> profilep.DateOfBirth;

			if (validateFormat(profilep.DateOfBirth, R"(\d{4}-\d{2}-\d{2})")) {
				cout << "Valid format." << endl;
				RM2.setValue(3, profilep.DateOfBirth);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;


		case 5:
			cout << "Please enter your height (cm): ";
			heightStream.str("");
			cin >> profilep.Height;
			heightStream << fixed << setprecision(2) << profilep.Height;
			RM2.setValue(4, heightStream.str());		
			break;

		case 6:
			cout << "Please enter your weight (kg): ";
			weightStream.str("");
			cin >> profilep.Weight;
			weightStream << fixed << setprecision(2) << profilep.Weight;
			RM2.setValue(5, weightStream.str());
			break;

		case 7:
			if (profilep.PName.empty() || profilep.PTelephoneNo.empty() || profilep.DateOfBirth.empty()) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				cout << "\nNow inserting and press any key to redirecte you to login...";
				_getch();
				profilep.insertp();
				loginAccount(2);
				break;

			}
			

		default:
			break;

		}
	}
}
void registerDoctorProfile(int AccountID) {

	Doctors profiled;
	
	Departments dpt;
	profiled.AccountID = AccountID;
	//Register Menu 3 (Into Doctor)
	Menu RM3;
	RM3.header = "Doctor Profile Registration\n\n";
	RM3.addOption("->> Name [Required]");
	RM3.addOption("->> Mobile Number (010-0000000) [Required]");
	RM3.addOption("->> Specialization [Required]");
	RM3.addOption("->> DepartmentID [Required]");
	RM3.addOption("->> Confirm");
	RM3.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		switch (RM3.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> profiled.DName;
			RM3.setValue(0, profiled.DName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> profiled.DTelephoneNo;

			if (validateFormat(profiled.DTelephoneNo, R"(\d{3}-\d{7})") && profiled.DTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				RM3.setValue(1, profiled.DTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;
			}
			break;

		case 3:
			cout << "Please enter your specialization: ";
			cin >> profiled.Specialization;
			RM3.setValue(2, profiled.Specialization);
			break;

		case 4:
			listDepartments();
			profiled.DepartmentID = tempDepartmentID;
			RM3.setValue(3, to_string(profiled.DepartmentID));
			break;

		case 5:
			if (profiled.DName.empty() || profiled.DTelephoneNo.empty() || profiled.Specialization.empty()) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				cout << "\nNow inserting and press any key to redirecte you to login...";
				_getch();
				profiled.insertd();
				loginAccount(1);
				break;

			}
			

		default:
			break;
		}
	}
}
void listDepartments() {

	Departments dpt;
	vector<Departments> Departments;
	string displayString = "", keyWord = "", sortColumn = "DepartmentID";
	int DepartmentID = -1;
	
	bool ascending = true;

	//Show the list at first encounter
	Departments = Departments::getdptlist(keyWord, sortColumn, ascending);

	//DepartmentList
	Menu DPTL;
	DPTL.header = "Search Option";
	DPTL.addOption("->> Keyword");
	DPTL.addOption("->> Sort By");
	DPTL.addOption("->> Ordering");
	DPTL.addOption("->> Search/Refresh");
	DPTL.addOption("->> Select");
	DPTL.addOption("->> Back");

	//ViewDoctorSortingSubMenu
	Menu VDSSM;
	VDSSM.header = "Choose the column that will be used to sort the list: \n";
	VDSSM.addOption("->> DepartmentID");
	VDSSM.addOption("->> DepartmentName");

	while (1)
	{
		DPTL.setValue(1, sortColumn);
		if (ascending) {
			DPTL.setValue(2, "Ascending");
		}
		else {
			DPTL.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DepartmentID" << "|" << setw(34) << "Name"
				<< "|" << setw(75) << "Description" << "|" << endl;

			for (int i = 0; i < Departments.size(); i++) {
				tmpString << setw(12) << Departments[i].DepartmentID << "|" << setw(34) << Departments[i].DptName
					<< "|" << setw(75) << Departments[i].Description << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DPTL.footer = displayString;

		switch (DPTL.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DPTL.setValue(0, keyWord);
			break;

		case 2:
			switch (VDSSM.prompt())
			{
			case 1:
				sortColumn = "DepartmentID";
				break;
			case 2:
				sortColumn = "DptName";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Departments = Departments::getdptlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			cout << "Please enter your DepartmentID: ";
			cin >> tempDepartmentID;
			return;
			break;
		case 6:
			return;
			break;
		default:
			break;
		}
	};
}
void loginAccount(int Role) {
	string Roles = "";
	if (Role == 0) {
		 Roles = " For Admin";
	}
	else if (Role == 1) {
		Roles = " For Doctor";
	}
	else {
		Roles = " For Patient";
	}
	Appointments appdata;
	Accounts acclogin;
	Patients  pdata;
	Doctors ddata;
	Menu LM;
	LM.header = "Login" + Roles + "\n";
	LM.addOption("->> Username");
	LM.addOption("->> Password");
	LM.addOption("->> Login");
	LM.addOption("->> Back");
	LM.footer = "\nEnter the data accordingly and login once done.";

	while (1) {
		switch (LM.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> acclogin.Username;
			LM.setValue(0, acclogin.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> acclogin.Password;
			LM.setValue(1, acclogin.Password);
			break;

		case 3: 
			if (acclogin.Username.empty() || acclogin.Password.empty()) {
				cout << "Invalid Account!";
				_getch();
				break;
			}

			if (acclogin.loginacc()) {

				if (acclogin.ARole == 0) {
					homeAdmin(acclogin); 
				}
				else if (acclogin.ARole == 1) { 
					ddata.getddataa(acclogin.AccountID);
					homeDoctor(acclogin, ddata);
				}
				else if (acclogin.ARole == 2) {
					pdata.getpdataa(acclogin.AccountID); 
					homePatient(acclogin, pdata);
				}
				else {
					cout << "Invalid Account!" << endl;
					_getch();
					break;
				}
			}
			else {
				cout << "Invalid Login!" << endl;
				_getch();
			}
			break;

		case 4:
			mainMenu();
			break;
		default:
			break;
		}
	}
}


// HomeMenuPatients 
void homePatient(Accounts a, Patients p) {
	auto notificationCallback = [a, p]() { patientNotificationChecker(a, p); };
	Timer::getInstance().startTimer(60, notificationCallback);

	// HomeMenuPatients
	Menu HMP;
	HMP.addOption("->> Profile Details");
	HMP.addOption("->> Account Details");
	HMP.addOption("->> Appointment");
	HMP.addOption("->> Feedback");
	HMP.addOption("->> Doctor List");
	HMP.addOption("->> Department List");
	HMP.addOption("->> Logout");
	HMP.addOption("->> Exit");
	HMP.footer = "\nEnter the number that represents your selection.";
	while (1) {

		HMP.header = "Welcome " + p.PName + "\n\n";


		switch (HMP.prompt())
		{
		case 1:
			p = patientProfileMenu(a, p);
			break;

		case 2:
			a = accountEditMenu(a, p);
			break;

		case 3:
			patientAppointmentMenu(a, p);
			break;

		case 4:
			feedbackMenu(a, p);
			break;

		case 5:
			viewDoctorList(a, p);
			break;

		case 6:
			viewDepartmentList(a, p);
			break;

		case 7:
			return;
			break;
		case 8:
			exit(0);
			break;
		default:
			break;
		}
	}
}
void patientNotificationChecker(Accounts a, Patients p) {

	//Current Time
	string currentDate;
	string currentTime;
	auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
	struct tm parts;
	localtime_s(&parts, &now);
	char dateBuffer[11];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &parts);
	char timeBuffer[9];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &parts);
	currentDate = dateBuffer;
	currentTime = timeBuffer;

	//Class getter
	Appointments apppnc;
	Doctors dpnc;
	Notifications npnc;
	vector<Appointments> Appointments;

	p.getpdatap(a.AccountID);
	//apppnc.getappdatap(ppnc.PatientID);

	//Get data for appointment using patientID, we dont have doctorID in this case
	Appointments = Appointments::getappdatapd(p.PatientID, -1);

	for (int i = 0; i < Appointments.size(); i++) {
		//To get minutesDifference
		string scheduledDateTimeStr = Appointments[i].ADate + " " + Appointments[i].ATime;
		tm scheduledDateTimeStruct = {};
		istringstream scheduledSS(scheduledDateTimeStr);
		scheduledSS >> get_time(&scheduledDateTimeStruct, "%Y-%m-%d %H:%M:%S");
		auto scheduledTime = chrono::system_clock::from_time_t(mktime(&scheduledDateTimeStruct));

		auto timeDifference = scheduledTime - chrono::system_clock::from_time_t(now);
		auto minutesDifference = chrono::duration_cast<chrono::minutes>(timeDifference).count();

		dpnc.getddatad(Appointments[i].DoctorID);
		npnc.getndataapp(Appointments[i].AppointmentID);

		//After having every parameter call for condition checker, if fulfill send notification
		Notifications::conditioncheckerp(minutesDifference, Appointments[i].AStatus, npnc.NStatus, npnc.Reason, Appointments[i].ADate, Appointments[i].ATime, Appointments[i].DoctorID, dpnc.DName, npnc.NotificationID, Appointments[i].AppointmentID, npnc.AppointmentID);
		
	}
}
Patients patientProfileMenu(Accounts a, Patients p) {

	Patients temp = p;
	
	ostringstream weightStream, heightStream;
	bool isMale;

	if (p.Gender == "Male") {
		temp.Gender = "Male";
		isMale = true;
	}
	else {
		temp.Gender = "Female";
		isMale = false;
	}
	
	heightStream << fixed << setprecision(2) << temp.Height;
	weightStream << fixed << setprecision(2) << temp.Weight;

	//PatientProfileMenu
	Menu PPM;
	PPM.header = "Edit Your Profile Details\n\n";
	PPM.addOption("->> Name");
	PPM.addOption("->> Mobile Number (010-0000000)");
	PPM.addOption("->> Toogle To Change Your Gender");
	PPM.addOption("->> Date of Birth (YYYY-MM-DD)");
	PPM.addOption("->> Height (cm)");
	PPM.addOption("->> Weight (kg)");
	PPM.addOption("->> Confirm");
	PPM.addOption("->> Back");
	PPM.addOption("->> Reset");
	PPM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		PPM.setValue(0, temp.PName);
		PPM.setValue(1, temp.PTelephoneNo);
		PPM.setValue(2, temp.Gender);
		PPM.setValue(3, temp.DateOfBirth);
		PPM.setValue(4, heightStream.str());
		PPM.setValue(5, weightStream.str());
		
		//Toogle function
		if (isMale) {
			PPM.setValue(2, "Male");
			temp.Gender = "Male";
		}
		else {
			PPM.setValue(2, "Female");
			temp.Gender = "Female";
		}

		switch (PPM.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> temp.PName;
			PPM.setValue(0, temp.PName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> temp.PTelephoneNo;
			if (validateFormat(temp.PTelephoneNo, R"(\d{3}-\d{7})") && temp.PTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				PPM.setValue(1, temp.PTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.PTelephoneNo = p.PTelephoneNo;
				_getch();
				break;
			}
			break;

		case 3:
			isMale = !isMale;
			break;

		case 4:
			cout << "Please enter your date of birth (YYYY-MM-DD): ";
			cin >> temp.DateOfBirth;
			if (validateFormat(temp.DateOfBirth, R"(\d{4}-\d{2}-\d{2})")) {
				cout << "Valid format." << endl;
				PPM.setValue(3, temp.DateOfBirth);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.DateOfBirth = p.DateOfBirth;
				_getch();
				break;

			}
			break;

		case 5:
			cout << "Please enter your height (cm): ";
			heightStream.str("");
			cin >> temp.Height;
			heightStream << fixed << setprecision(2) << temp.Height;
			PPM.setValue(4, heightStream.str());

			break;

		case 6:
			cout << "Please enter your weight (kg): ";
			weightStream.str("");
			cin >> temp.Weight;
			weightStream << fixed << setprecision(2) << temp.Weight;
			PPM.setValue(5, weightStream.str());

			break;

		case 7: //Confirm
			p = temp;
			p.updatep();
			cout << "Updated!";
			_getch();
			break;

		case 8: //Back
			return p;
			break;

		case 9: //Reset
			temp = p;
			heightStream.str(""); // Clear the contents of the height stream
			weightStream.str(""); // Clear the contents of the weight stream
			heightStream << fixed << setprecision(2) << temp.Height;
			weightStream << fixed << setprecision(2) << temp.Weight;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
Accounts accountEditMenu(Accounts a, Patients p) {

	Accounts temp = a;
	//AccountEditMenu
	Menu AEM;
	AEM.header = "Edit Your Account Details\n\n";
	AEM.addOption("->> Username");
	AEM.addOption("->> Password");
	AEM.addOption("->> Confirm");
	AEM.addOption("->> Back");
	AEM.addOption("->> Reset");
	AEM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		AEM.setValue(0, (temp.Username));
		AEM.setValue(1, temp.Password);
		switch (AEM.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> temp.Username;
			if (temp.Username.length() < 5) {
				cout << "Username must be at least 5 characters.";
				temp.Username = a.Username;
				_getch();
				break;
			}

			if (temp.usernameExisted(temp.Username) == true) {
				cout << "This username is already in use. Please try a different one.";
				temp.Username = a.Username;
				_getch();
				break;
			}
			AEM.setValue(0, temp.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> temp.Password;

			if (temp.Password.length() < 8) {
				cout << "Password must be at least 8 characters.";
				temp.Password = a.Password;
				_getch();
				break;
			}

			if (hasAlphabet(temp.Password) == true && hasDigit(temp.Password) == true) {
				AEM.setValue(1, temp.Password);
				break;
			}
			else {
				cout << "The password needs to be made up of both numbers and letters.";
				temp.Password = a.Password;
				_getch();
				break;
			}
			
			break;

		case 3:
			a = temp;
			a.updateacc();
			cout << "Updated!";
			_getch();

		case 4:
			return a;
			break;
		case 5:
			temp = a;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
void patientAppointmentMenu(Accounts a, Patients p) {

	Appointments appam;
	vector<Appointments> Appointments;
	string displayString = "", sortColumn = "AppointmentID";
	bool ascending = true;
		bool isAll = true;

	//Show the list at first encounter
	Appointments = Appointments::getapplistp(p.PatientID, sortColumn, ascending, isAll);
	bool foundAppointment;
	//AppointmentMenu
	Menu AM;
	AM.header = "Appointment\n\n";
	AM.addOption("->> Sort by");
	AM.addOption("->> Order by");
	AM.addOption("->> Toogle");
	AM.addOption("->> Search/Refresh");
	AM.addOption("->> Make Appointment");
	AM.addOption("->> Update Appointment");
	AM.addOption("->> Cancel Appointment");
	AM.addOption("->> Back");

	//AppointmentSortingSubMenu
	Menu ASSM;
	ASSM.header = "Choose the column that will be used to sort the list: \n";
	ASSM.addOption("->> AppointmentID");
	ASSM.addOption("->> Date");
	ASSM.addOption("->> Time");
	ASSM.addOption("->> Status");
	ASSM.addOption("->> DoctorID");

	while (1) {

		AM.setValue(0, sortColumn);
		if (ascending) {
			AM.setValue(1, "Ascending");
		}
		else {
			AM.setValue(1, "Descending");
		}
		if (isAll) {
			AM.setValue(2, "Show All Appointments");
		}
		else {
			AM.setValue(2, "Show Future Appointments");
		}
		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "AppointmentID" << "|" << setw(12) << "Date"
				<< "|" << setw(15) << "Time" << "|" << setw(20) << "Status" << "|" << setw(20) << "DoctorID" << "|" << endl;

			for (int i = 0; i < Appointments.size(); i++) {
				tmpString << setw(13) << Appointments[i].AppointmentID << "|" << setw(12) << Appointments[i].ADate
					<< "|" << setw(15) << Appointments[i].ATime << "|" << setw(20) << Appointments[i].AStatus << "|" << setw(20)
					<< Appointments[i].DoctorID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		AM.footer = displayString;

		switch (AM.prompt())
		{
		case 1:
			switch (ASSM.prompt())
			{
			case 1:
				sortColumn = "AppointmentID";
				break;
			case 2:
				sortColumn = "ADate";
				break;
			case 3:
				sortColumn = "ATime";
				break;
			case 4:
				sortColumn = "AStatus";
				break;
			case 5:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			isAll = !isAll;
			break;

		case 4:
			Appointments = Appointments::getapplistp(p.PatientID, sortColumn, ascending, isAll);
			displayString = "";
			break;

		case 5:
			addAppointmentMenu(a, p);
			break;

		case 6:
			cout << "Enter the AppointmentID to choose the appointment that needs to be updated: ";
			cin >> appam.AppointmentID;
			
			foundAppointment = false;

			for (int i = 0; i < Appointments.size(); i++) {
				if (appam.AppointmentID == Appointments[i].AppointmentID) {
					foundAppointment = true;
					appam.getappdataa(appam.AppointmentID);
					if (appam.AStatus == "Succeeded") {
						cout << "This appointment has been confirmed, thus you are no longer able to update.\n";
						cout << "You may only cancel this appointment or make a new appointment. ";
						_getch();
						break;

					}
					else {
						updateAppointmentMenu(appam.AppointmentID, a, p);
					}
				}
				
			}

			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;

		case 7:
			cout << "Enter the AppointmentID to choose the appointment that needs to be cancel: ";
			cin >> appam.AppointmentID;
			foundAppointment = false;
			for (int i = 0; i < Appointments.size(); i++) {
				if (appam.AppointmentID == Appointments[i].AppointmentID) {
					foundAppointment = true;
					appam.getappdataa(appam.AppointmentID);
					if (appam.AStatus == "Canceled") {
						cout << "This appointment has been canceled, thus you are no longer able to cancel.\n";
						_getch();
						break;

					}
					else if (appam.AStatus == "Failed") {
						cout << "This appointment has been canceled, thus you are no longer able to cancel.\n";
						_getch();
						break;

					}
					else {
						appam.updateapps("Canceled", appam.AppointmentID);
						cout << "Canceled!";
						_getch();
						break;
					}
				}
			
			}
			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;
			
			

		case 8:
			return;
			break;
		default:
			break;
		}
	}
}
void addAppointmentMenu(Accounts a, Patients p) {

	Appointments appaam;
	appaam.DoctorID = -1;
	appaam.PatientID = p.PatientID;
	//Add Appointment Menu
	Menu AAM;
	AAM.header = "Make Appointment \n\n";
	AAM.addOption("->> Date (YYYY-MM-DD)");
	AAM.addOption("->> Time (08:00 - 18:00)");
	AAM.addOption("->> DoctorID");
	AAM.addOption("->> Confirm");
	AAM.addOption("->> Back");
	AAM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		switch (AAM.prompt())
		{
		case 1:
			cout << "Please enter the date you would like to schedule an appointment (YYYY-MM-DD): ";
			cin >> appaam.ADate;
			if (validateFormat(appaam.ADate, R"(\d{4}-\d{2}-\d{2})") && isValidDate(appaam.ADate.substr(0, 4), appaam.ADate.substr(5, 2), appaam.ADate.substr(8, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(0, appaam.ADate);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;

		case 2:
			cout << "Please enter the time you would like to schedule an appointment (HH:MM) (24 Hour Format): ";
			cin >> appaam.ATime;
			appaam.ATime += ":00";
			if (validateFormat(appaam.ATime, R"(\d{2}:\d{2}:\d{2})") && isValidTime(appaam.ATime.substr(0, 2), appaam.ATime.substr(3, 2), appaam.ATime.substr(6, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(1, appaam.ATime);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;
		
		case 3:
			doctorList();
			appaam.DoctorID = doctorSelected;
			AAM.setValue(2, to_string(appaam.DoctorID));
			break;

		case 4:
			if (appaam.ADate.empty() || appaam.ATime.empty() || to_string(appaam.DoctorID).empty() || appaam.DoctorID == -1) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				appaam.insertapp("Pending");
				cout << "Completed!";
				_getch();
				break;

			}
			
		case 5:
			return;
		default:
			break;
		}
	}
}
void updateAppointmentMenu(int AppointmentID, Accounts a, Patients p) {

	Appointments appuam;
	appuam.AppointmentID = AppointmentID;
	appuam.getappdataa(AppointmentID);
	Appointments temp  = appuam;
	

	//Update Appointment Menu 
	Menu UAM;
	UAM.header = "Update Appointment Details\n\n";
	UAM.addOption("->> Date (YYYY-MM-DD)");
	UAM.addOption("->> Time (08:00 - 18:00)");
	UAM.addOption("->> DoctorID");
	UAM.addOption("->> Confirm");
	UAM.addOption("->> Back");
	UAM.addOption("->> Reset");
	UAM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		UAM.setValue(0, (temp.ADate));
		UAM.setValue(1, temp.ATime);
		UAM.setValue(2, to_string(temp.DoctorID));
		switch (UAM.prompt())
		{
		case 1:
			cout << "Please enter the date you would like to schedule an appointment (YYYY-MM-DD): ";
			cin >> temp.ADate;
			if (validateFormat(temp.ADate, R"(\d{4}-\d{2}-\d{2})") && isValidDate(temp.ADate.substr(0, 4), temp.ADate.substr(5, 2), temp.ADate.substr(8, 2))) {
				cout << "Valid format." << endl;
				UAM.setValue(0, temp.ADate);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.ADate = appuam.ADate;
				_getch();
				break;

			}
			break;

		case 2:
			cout << "Please enter the time you would like to schedule an appointment (HH:MM) (24 Hour Format): ";
			cin >> temp.ATime;
			temp.ATime += ":00";
			if (validateFormat(temp.ATime, R"(\d{2}:\d{2}:\d{2})") && isValidTime(temp.ATime.substr(0, 2), temp.ATime.substr(3, 2), temp.ATime.substr(6, 2))) {
				cout << "Valid format." << endl;
				UAM.setValue(1, temp.ATime);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.ATime = appuam.ATime;
				_getch();
				break;

			}
			break;

		case 3:
			doctorList();
			temp.DoctorID = doctorSelected;
			UAM.setValue(2, to_string(temp.DoctorID));

			break;

		case 4:
			appuam = temp;
			appuam.updateapp("Pending", appuam.AppointmentID);
			cout << "Updated!";
			_getch();

		case 5:
			return;
			break;
		case 6:
			temp = appuam;
			break;
		default:
			break;
		}
	}
}
void feedbackMenu(Accounts a, Patients p) {
	Feedback ffm;
	vector<Feedback> Feedback;
	bool foundAppointment;
	string displayString = "", sortColumn = "FeedbackID";
	bool ascending = true;

	//Show the list at first encounter
	Feedback = Feedback::getflistp(p.PatientID, sortColumn, ascending);

	//Feedback Menu 
	Menu FM;
	FM.header = "Feedback\n\n";
	FM.addOption("->> Sort by");
	FM.addOption("->> Order by");
	FM.addOption("->> Search/Refresh");
	FM.addOption("->> Make Feedback");
	FM.addOption("->> Update Feedback");
	FM.addOption("->> Back");

	//FeedbackSortingSubMenu
	Menu FSSM;
	FSSM.header = "Choose the column that will be used to sort the list: \n";
	FSSM.addOption("->> FeedbackID");
	FSSM.addOption("->> Ratings");
	FSSM.addOption("->> Date");
	FSSM.addOption("->> DoctorID");

	while (1) {

		FM.setValue(0, sortColumn);
		if (ascending) {
			FM.setValue(1, "Ascending");
		}
		else {
			FM.setValue(1, "Descending");
		}
		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "FeedbackID" << "|" << setw(12) << " Ratings"
				<< "|" << setw(15) << "Date" << "|"<< setw(10) << "DoctorID" << "|" << setw(60) << "Comments" << "|" << endl;

			for (int i = 0; i < Feedback.size(); i++) {
				tmpString << setw(10) << Feedback[i].FeedbackID << "|" << setw(12) << Feedback[i].Ratings
					<< "|" << setw(15) << Feedback[i].FDate << "|" << setw(10) << Feedback[i].DoctorID << "|" << setw(60) << Feedback[i].Comments << "|" << endl;
			}
			displayString += tmpString.str();
		}
		FM.footer = displayString;

		switch (FM.prompt())
		{
		case 1:

			switch (FSSM.prompt())
			{
			case 1:
				sortColumn = "FeedbackID";
				break;
			case 2:
				sortColumn = "Ratings";
				break;
			case 3:
				sortColumn = "FDate";
				break;
			case 4:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			Feedback = Feedback::getflistp(p.PatientID, sortColumn, ascending);
			displayString = "";
			break;

		case 4:
			addFeedbackMenu(a, p);
			break;

		case 5:
			cout << "Enter the FeedbackID to choose the feedback that needs to be updated: ";
			cin >> ffm.FeedbackID;
			foundAppointment = false;
			for (int i = 0; i < Feedback.size(); i++) {
				if (ffm.FeedbackID == Feedback[i].FeedbackID) {
					foundAppointment = true;
					updateFeedbackMenu(ffm.FeedbackID, a, p);
					break;
				}
				
			}
			ffm.FeedbackID = -1;
			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;

		case 6:
			return;
			break;
		default:
			break;
		}
	}
}
void addFeedbackMenu(Accounts a, Patients p) {

	Feedback fafm;
	p.getpdatap(p.PatientID);
	fafm.PatientID = p.PatientID;
	fafm.DoctorID = -1;
	//Add Feedback Menu 
	Menu AFM;
	AFM.header = "Make Feedback\n\n";
	AFM.addOption("->> Ratings (1-5) [Required]");
	AFM.addOption("->> Comments [Optional]");
	AFM.addOption("->> Date (YYYY-MM-DD) [Required] Press to get current date:");
	AFM.addOption("->> DoctorID [Required]");
	AFM.addOption("->> Confirm");
	AFM.addOption("->> Back");
	AFM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		switch (AFM.prompt())
		{
		case 1:
			cout << "Please enter the ratings (1-5) : ";
			cin >> fafm.Ratings;
			if (isValidRatings(fafm.Ratings) == true) {
				std::cout << "Valid input: " << fafm.Ratings << std::endl;
				// Proceed with the valid input
				AFM.setValue(0, to_string(fafm.Ratings));
				break;
			}
			else {
				std::cout << "Invalid input. Please enter an integer between 1 and 5." << std::endl;
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
				_getch();
				break;
				// Handle the case of invalid input
			}
			break;

		case 2:
			cout << "Please enter the comments: : ";
			cin >> fafm.Comments;
			AFM.setValue(1, fafm.Comments);
			break;

		case 3: //retrive localtime
		{
			auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
			struct tm parts;

			localtime_s(&parts, &now);

			char buffer[11];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d", &parts);

			fafm.FDate = buffer;
		}
					AFM.setValue(2, fafm.FDate);
			break;

		case 4:
			doctorList();
			fafm.DoctorID = doctorSelected;
			AFM.setValue(3, to_string(fafm.DoctorID));
			break;

		case 5:
			if (to_string(fafm.Ratings).empty() || fafm.FDate.empty() || to_string(fafm.DoctorID).empty() || fafm.DoctorID == -1) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				fafm.insertf();
				cout << "Completed!";
				_getch();
				break;

			}
			
		case 6:
			return;
		default:
			break;
		}
	}
}
void updateFeedbackMenu(int FeedbackID, Accounts a, Patients p) {

	Feedback fufm;
	fufm.FeedbackID = FeedbackID;
	fufm.getfdataf(FeedbackID);
	Feedback temp = fufm;
	fufm.PatientID = p.PatientID;
	//Update Feedback Menu 
	Menu UFM;
	UFM.header = "Update Feedback Details\n\n";
	UFM.addOption("->> Ratings (1-5)");
	UFM.addOption("->> Comments [Optional]");
	UFM.addOption("->> Date [YYYY-MM-DD] Press to get current date: ");
	UFM.addOption("->> DoctorID");
	UFM.addOption("->> Confirm");
	UFM.addOption("->> Back");
	UFM.addOption("->> Reset");
	UFM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		UFM.setValue(0, to_string(temp.Ratings));
		UFM.setValue(1, temp.Comments);
		UFM.setValue(2, temp.FDate);
		UFM.setValue(3, to_string(temp.DoctorID));
		switch (UFM.prompt())
		{
		case 1:
			cout << "Please enter the ratings (1-5) : ";
			cin >> temp.Ratings;
			if (isValidRatings(temp.Ratings) == true) {
				std::cout << "Valid input: " << temp.Ratings << std::endl;
				// Proceed with the valid input
				UFM.setValue(0, to_string(temp.Ratings));
				break;
			}
			else {
				std::cout << "Invalid input. Please enter an integer between 1 and 5." << std::endl;
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
				temp.Ratings = fufm.Ratings;
				_getch();
				break;
				// Handle the case of invalid input
			}
			break;


		case 2:
			cout << "Please enter the comments: : ";
			cin >> temp.Comments;
			UFM.setValue(1, temp.Comments);
			break;

		case 3:
		{
			auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
			struct tm parts;

			localtime_s(&parts, &now);

			char buffer[11];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d", &parts);

			temp.FDate = buffer;
		}
		UFM.setValue(2, temp.FDate);
		break;

		case 4:
			doctorList();
			temp.DoctorID = doctorSelected;
			UFM.setValue(3, to_string(temp.DoctorID));
			break;

		case 5:
			fufm = temp;
			fufm.updatef(fufm.FeedbackID);
			cout << "Updated!";
			_getch();

		case 6:
			return;
			break;
		case 7:
			temp = fufm;
			break;
		default:
			break;
		}
	}
}
void viewDoctorList(Accounts a, Patients p) {
	vector<Doctors> Doctors;
	string displayString = "",  keyWord = "", sortColumn = "DoctorID";
	int DepartmentID = -1;

	bool ascending = true;

	//Show the list at first encounter
	Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);

	//ViewDoctorList
	Menu VDL;
	VDL.header = "Search Option";
	VDL.addOption("->> Keyword");
	VDL.addOption("->> Sort By");
	VDL.addOption("->> Ordering");
	VDL.addOption("->> Search/Refresh");
	VDL.addOption("->> Back");

	//ViewDoctorSortingSubMenu
	Menu VDSSM;
	VDSSM.header = "Choose the column that will be used to sort the list: \n";
	VDSSM.addOption("->> Name");
	VDSSM.addOption("->> Specialization");
	VDSSM.addOption("->> DepartmentID");
	VDSSM.addOption("->> DoctorID");

	while (1)
	{
		VDL.setValue(1, sortColumn);
		if (ascending) {
			VDL.setValue(2, "Ascending");
		}
		else {
			VDL.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DoctorID" << "|" << setw(11) << "Name"
				<< "|" << setw(15) << "TelephoneNo" << "|" << setw(20) << "Specialization" << "|" << setw(20) << "DepartmentID" << "|" << endl;

			for (int i = 0; i < Doctors.size(); i++) {
				tmpString << setw(8) << Doctors[i].DoctorID << "|" << setw(11) << Doctors[i].DName
					<< "|" << setw(15) << Doctors[i].DTelephoneNo << "|" << setw(20) << Doctors[i].Specialization << "|" << setw(20)
					<< Doctors[i].DepartmentID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		VDL.footer = displayString;

		switch (VDL.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			VDL.setValue(0, keyWord);
			break;

		case 2:
			switch (VDSSM.prompt())
			{
			case 1:
				sortColumn = "DName";
				break;
			case 2:
				sortColumn = "Specialization";
				break;
			case 3:
				sortColumn = "DepartmentID";
				break;
			case 4:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			return;
			break;
		default:
			break;
		}
	};
}
void viewDepartmentList(Accounts a, Patients p) {

	vector<Departments> Departments;
	string displayString = "", keyWord = "", sortColumn = "DepartmentID";
	int DepartmentID = -1;
	bool ascending = true;

	//Show the list at first encounter
	Departments = Departments::getdptlist(keyWord, sortColumn, ascending);

	//ViewDoctorList
	Menu VDPTL;
	VDPTL.header = "Search Option";
	VDPTL.addOption("->> Keyword");
	VDPTL.addOption("->> Sort By");
	VDPTL.addOption("->> Ordering");
	VDPTL.addOption("->> Search/Refresh");
	VDPTL.addOption("->> Back");

	//ViewDoctorSortingSubMenu
	Menu VDSSM;
	VDSSM.header = "Choose the column that will be used to sort the list: \n";
	VDSSM.addOption("->> DepartmentID");
	VDSSM.addOption("->> Name");

	while (1)
	{
		VDPTL.setValue(1, sortColumn);
		if (ascending) {
			VDPTL.setValue(2, "Ascending");
		}
		else {
			VDPTL.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DepartmentID" << "|" << setw(34) << "Name"
				<< "|" << setw(75) << "Description" << "|" << endl;

			for (int i = 0; i < Departments.size(); i++) {
				tmpString << setw(12) << Departments[i].DepartmentID << "|" << setw(34) << Departments[i].DptName
					<< "|" << setw(75) << Departments[i].Description << "|" << endl;
			}
			displayString += tmpString.str();
		}
		VDPTL.footer = displayString;

		switch (VDPTL.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			VDPTL.setValue(0, keyWord);
			break;

		case 2:
			switch (VDSSM.prompt())
			{
			case 1:
				sortColumn = "DepartmentID";
				break;
			case 2:
				sortColumn = "DptName";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Departments = Departments::getdptlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			return;
			break;
		default:
			break;
		}
	};
}

//HomeMenuDoctors
void homeDoctor(Accounts a, Doctors d) {
	auto notificationCallback = [a, d]() { doctorNotificationChecker(a, d); };
	Timer::getInstance().startTimer(60, notificationCallback);
	//HomeMenuDoctors
	Menu HMD;
	HMD.addOption("->> Profile Details");
	HMD.addOption("->> Account Details");
	HMD.addOption("->> Appointment");
	HMD.addOption("->> Feedback");
	HMD.addOption("->> Logout");
	HMD.addOption("->> Exit");
	HMD.footer = "\nEnter the number that represents your selection.";
	while (1) {

		HMD.header = "Welcome Dr. " + d.DName + "\n\n";

		switch (HMD.prompt())
		{
		case 1:
			d = doctorProfileMenu(a, d);
			break;

		case 2:
			a = accountEditMenu(a, d);
			break;

		case 3:
			doctorAppointmentMenu(a, d);
			break;

		case 4:
			feedbackMenu(a, d);
			break;

		case 5:
			return;
			break;
		case 6:
			exit(0);
			break;
		default:
			break;
		}
	}
}
void doctorNotificationChecker(Accounts a, Doctors d) {
	//Current Time
	string currentDate;
	string currentTime;
	auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
	struct tm parts;
	localtime_s(&parts, &now);
	char dateBuffer[11];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &parts);
	char timeBuffer[9];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &parts);
	currentDate = dateBuffer;
	currentTime = timeBuffer;

	//Class getter
	Appointments appdnc;
	Patients pdnc;
	Notifications ndnc;
	vector<Appointments> Appointments;

	d.getddataa(a.AccountID);


	//Get data for appointment using DoctorID, we dont have patientID in this case
	Appointments = Appointments::getappdatapd(d.DoctorID, -1);

	for (int i = 0; i < Appointments.size(); i++) {
		//To get minutesDifference
		string scheduledDateTimeStr = Appointments[i].ADate + " " + Appointments[i].ATime;
		tm scheduledDateTimeStruct = {};
		istringstream scheduledSS(scheduledDateTimeStr);
		scheduledSS >> get_time(&scheduledDateTimeStruct, "%Y-%m-%d %H:%M:%S");
		auto scheduledTime = chrono::system_clock::from_time_t(mktime(&scheduledDateTimeStruct));

		auto timeDifference = scheduledTime - chrono::system_clock::from_time_t(now);
		auto minutesDifference = chrono::duration_cast<chrono::minutes>(timeDifference).count();

		//To get extra data for notification
		pdnc.getpdatap(Appointments[i].PatientID);
		ndnc.getndataapp(Appointments[i].AppointmentID);

		//After having every parameter call for condition checker, if fulfill send notification
		Notifications::conditioncheckerd(minutesDifference, Appointments[i].AStatus, ndnc.NStatus, ndnc.Reason, Appointments[i].ADate, Appointments[i].ATime, Appointments[i].PatientID, pdnc.PName, ndnc.NotificationID, Appointments[i].AppointmentID);
	}
}
Doctors doctorProfileMenu(Accounts a, Doctors d) {

	Doctors temp = d;

	//DoctorProfileMenu
	Menu DPM;
	DPM.header = "Edit Your Profile Details\n\n";
	DPM.addOption("->> Name");
	DPM.addOption("->> Mobile Number (010-0000000)");
	DPM.addOption("->> Specialization");
	DPM.addOption("->> Confirm");
	DPM.addOption("->> Back");
	DPM.addOption("->> Reset");
	DPM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		DPM.setValue(0, temp.DName);
		DPM.setValue(1, temp.DTelephoneNo);
		DPM.setValue(2, temp.Specialization);

		switch (DPM.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> temp.DName;
			DPM.setValue(0, temp.DName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> temp.DTelephoneNo;
			if (validateFormat(temp.DTelephoneNo, R"(\d{3}-\d{7})") && temp.DTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				DPM.setValue(1, temp.DTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.DTelephoneNo = d.DTelephoneNo;
				_getch();
				break;
			}
			break;

		case 3:
			cout << "Please enter your specialization: ";
			cin >> temp.Specialization;
			DPM.setValue(2, temp.Specialization);
			break;


		case 4: //Confirm
			d = temp;
			d.updated();
			cout << "Updated!";
			_getch();
			break;

		case 5: //Back
			return d;
			break;

		case 6: //Reset
			temp = d;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
Accounts accountEditMenu(Accounts a, Doctors d) {

	Accounts temp = a;

	//AccountEditMenu
	Menu AEM;
	AEM.header = "Edit Your Account Details\n\n";
	AEM.addOption("->> Username");
	AEM.addOption("->> Password");
	AEM.addOption("->> Confirm");
	AEM.addOption("->> Back");
	AEM.addOption("->> Reset");
	AEM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		AEM.setValue(0, (temp.Username));
		AEM.setValue(1, temp.Password);
		switch (AEM.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> temp.Username;
			if (temp.Username.length() < 5) {
				cout << "Username must be at least 5 characters.";
				temp.Username = a.Username;
				_getch();
				break;
			}

			if (temp.usernameExisted(temp.Username) == true) {
				temp.Username = a.Username;
				cout << "This username is already in use. Please try a different one.";
				_getch();
				break;
			}
			AEM.setValue(0, temp.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> temp.Password;

			if (temp.Password.length() < 8) {
				cout << "Password must be at least 8 characters.";
				temp.Password = a.Password;
				_getch();
				break;
			}

			if (hasAlphabet(temp.Password) == true && hasDigit(temp.Password) == true) {
				AEM.setValue(1, temp.Password);
				break;
			}
			else {
				cout << "The password needs to be made up of both numbers and letters.";
				temp.Password = a.Password;
				_getch();
				break;
			}
			break;

		case 3:
			a = temp;
			a.updateacc();
			cout << "Updated!";
			_getch();

		case 4:
			return a;
			break;
		case 5:
			temp = a;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
void doctorAppointmentMenu(Accounts a, Doctors d) {
	Appointments appam;
	vector<Appointments> Appointments;
	string displayString = "", sortColumn = "AppointmentID", updateStatus = "";
	bool ascending = true;
		bool isAll = true;

	//Show the list at first encounter
	Appointments = Appointments::getapplistd(d.DoctorID, sortColumn, ascending, isAll);
	bool foundAppointment;
	//AppointmentMenu
	Menu AM;
	AM.header = "Appointment\n\n";
	AM.addOption("->> Sort by");
	AM.addOption("->> Order by");
	AM.addOption("->> Toogle");
	AM.addOption("->> Search/Refresh");
	AM.addOption("->> Make Appointment");
	AM.addOption("->> Update Appointment");
	AM.addOption("->> Update Patient Profile");
	AM.addOption("->> Back");

	//AppointmentSortingSubMenu
	Menu ASSM;
	ASSM.header = "Choose the column that will be used to sort the list: \n";
	ASSM.addOption("->> AppointmentID");
	ASSM.addOption("->> Date");
	ASSM.addOption("->> Time");
	ASSM.addOption("->> Status");
	ASSM.addOption("->> PatientID");

	Menu DASM;
	DASM.header = "Choose the status that you want to update to the appoinment: \n";
	DASM.addOption("->> Succeed");
	DASM.addOption("->> Fail");
	DASM.addOption("->> Back");
	DASM.footer = "\nYou are no longer able to change it after you select it! \nPlease be caution on what you want to select!";


	while (1) {

		AM.setValue(0, sortColumn);
		if (ascending) {
			AM.setValue(1, "Ascending");
		}
		else {
			AM.setValue(1, "Descending");
		}

		if (isAll) {
			AM.setValue(2, "Show All Appointments");
		}
		else {
			AM.setValue(2, "Show Future Appointments");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "AppointmentID" << "|" << setw(12) << "Date"
				<< "|" << setw(15) << "Time" << "|" << setw(20) << "Status" << "|" << setw(20) << "PatientID" << "|" << endl;

			for (int i = 0; i < Appointments.size(); i++) {
				tmpString << setw(13) << Appointments[i].AppointmentID << "|" << setw(12) << Appointments[i].ADate
					<< "|" << setw(15) << Appointments[i].ATime << "|" << setw(20) << Appointments[i].AStatus << "|" << setw(20)
					<< Appointments[i].PatientID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		AM.footer = displayString;

		switch (AM.prompt())
		{
		case 1:
			switch (ASSM.prompt())
			{
			case 1:
				sortColumn = "AppointmentID";
				break;
			case 2:
				sortColumn = "ADate";
				break;
			case 3:
				sortColumn = "ATime";
				break;
			case 4:
				sortColumn = "AStatus";
				break;
			case 5:
				sortColumn = "PatientID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			isAll = !isAll;
			break;

		case 4:
			Appointments = Appointments::getapplistd(d.DoctorID, sortColumn, ascending, isAll);
			displayString = "";
			break;

		case 5:
			cout << "Enter the PatientID to make an follow up appointment: ";
			cin >> appam.PatientID;
			

			foundAppointment = false;
			for (int i = 0; i < Appointments.size(); i++) {
				if (appam.PatientID == Appointments[i].PatientID) {
					foundAppointment = true;
					appam.getappdataa(appam.PatientID);
					addAppointmentMenu(a, d, appam.PatientID);
				}

			}
			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;

		case 6:
			cout << "Enter the AppointmentID to choose the appointment that needs to be updated: ";
			cin >> appam.AppointmentID;
			foundAppointment = false;
			for (int i = 0; i < Appointments.size(); i++) {
			if (appam.AppointmentID == Appointments[i].AppointmentID) {
				foundAppointment = true;
				appam.getappdataa(appam.AppointmentID);
				if (appam.AStatus == "Succeeded") {
					cout << "This appointment has been confirmed, thus you are no longer able to update.\n";
					_getch();
					break;

				}
				else if (appam.AStatus == "Canceled") {
					cout << "This appointment has been canceled, thus you are no longer able to update.\n";
					_getch();
					break;
				}
				else if (appam.AStatus == "Failed") {
					cout << "This appointment has been failed, thus you are no longer able to update.\n";
					_getch();
					break;
				}
				else {
					switch (DASM.prompt())
					{
					case 1:
						updateStatus = "Succeeded";
						appam.updateapps(updateStatus, appam.AppointmentID);
						cout << "Updated!";
						updateStatus = "";
						_getch();
						break;
					case 2:
						updateStatus = "Failed";
						appam.updateapps(updateStatus, appam.AppointmentID);
						cout << "Updated!";
						updateStatus = "";
						_getch();
						break;
					case 3:
						break;
					default:
						break;
					}

					break;
				}
			}
			
		}
			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;

		case 7:
			cout << "Enter the PatientID to Update Their Profile Details: ";
			cin >> appam.PatientID;
			
			foundAppointment = false;
			for (int i = 0; i < Appointments.size(); i++) {
				if (appam.PatientID == Appointments[i].PatientID) {
					foundAppointment = true;
					appam.getappdataa(appam.PatientID);
					updatePatientProfileMenu(a, d, appam.PatientID);
				}

			}
			if (!foundAppointment) {
				cout << "This appointment does not belong to you!";
				_getch();
			}
			break;

		case 8:
			return;
			break;
		default:
			break;
		}
	}
}
void addAppointmentMenu(Accounts a, Doctors d, int PatientID) {

	Appointments appaam;
	appaam.PatientID = PatientID;
	appaam.DoctorID = d.DoctorID;
	//Add Appointment Menu
	Menu AAM;
	AAM.header = "Make Follow Up Appointment \n\n";
	AAM.addOption("->> Date (YYYY-MM-DD)");
	AAM.addOption("->> Time (08:00 - 18:00)");
	AAM.addOption("->> Confirm");
	AAM.addOption("->> Back");
	AAM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		switch (AAM.prompt())
		{
		case 1:
			cout << "Please enter the date you would like to schedule an appointment (YYYY-MM-DD): ";
			cin >> appaam.ADate;
			if (validateFormat(appaam.ADate, R"(\d{4}-\d{2}-\d{2})") && isValidDate(appaam.ADate.substr(0, 4), appaam.ADate.substr(5, 2), appaam.ADate.substr(8, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(0, appaam.ADate);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;

		case 2:
			cout << "Please enter the time you would like to schedule an appointment (HH:MM) (24 Hour Format): ";
			cin >> appaam.ATime;
			appaam.ATime += ":00";
			if (validateFormat(appaam.ATime, R"(\d{2}:\d{2}:\d{2})") && isValidTime(appaam.ATime.substr(0, 2), appaam.ATime.substr(3, 2), appaam.ATime.substr(6, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(1, appaam.ATime);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;
		case 3:
			if (appaam.ADate.empty() || appaam.ATime.empty()) {
				cout << "Date and Time cannot be empty!";
				_getch();
				break;
			}
			else {
				appaam.insertapp("Succeeded");
				cout << "Completed!";
				_getch();
				break;
			}
			
		case 4:
			return;
		default:
			break;
		}
	}
}
Patients updatePatientProfileMenu(Accounts a, Doctors d, int PatientID) {
	Patients p;
	p.getpdatap(PatientID);
	Patients temp = p;

	ostringstream weightStream, heightStream;

	heightStream << fixed << setprecision(2) << temp.Height;
	weightStream << fixed << setprecision(2) << temp.Weight;

	//PatientProfileMenu
	Menu PPM;
	PPM.header = "Edit Patients Profile Details\n\n";
	PPM.addOption("->> Name");
	PPM.addOption("->> Mobile Number (010-0000000)");
	PPM.addOption("->> Gender");
	PPM.addOption("->> Date of Birth (YYYY-MM-DD)");
	PPM.addOption("->> Height (cm)");
	PPM.addOption("->> Weight (kg)");
	PPM.addOption("->> Confirm");
	PPM.addOption("->> Back");
	PPM.addOption("->> Reset");
	PPM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		PPM.setValue(0, p.PName);
		PPM.setValue(1, p.PTelephoneNo);
		PPM.setValue(2, p.Gender);
		PPM.setValue(3, p.DateOfBirth);
		PPM.setValue(4, heightStream.str());
		PPM.setValue(5, weightStream.str());


		switch (PPM.prompt())
		{
		case 1:
			cout << "You are not allowed to edit this! ";
			_getch();
			break;

		case 2:
			cout << "You are not allowed to edit this! ";
			_getch();
			break;
		case 3:
			cout << "You are not allowed to edit this! ";
			_getch();
			break;

		case 4:
			cout << "You are not allowed to edit this! ";
			_getch();
			break;

		case 5:
			cout << "Please enter your patient updated height (cm): ";
			heightStream.str("");
			cin >> temp.Height;
			heightStream << fixed << setprecision(2) << temp.Height;
			PPM.setValue(4, heightStream.str());

			break;

		case 6:
			cout << "Please enter your patient updated weight (kg): ";
			weightStream.str("");
			cin >> temp.Weight;
			weightStream << fixed << setprecision(2) << temp.Weight;
			PPM.setValue(5, weightStream.str());

			break;

		case 7: //Confirm
			p = temp;
			p.updatep();
			cout << "Updated!";
			_getch();
			break;

		case 8: //Back
			return p;
			break;

		case 9: //Reset
			temp = p;
			heightStream.str(""); // Clear the contents of the height stream
			weightStream.str(""); // Clear the contents of the weight stream
			heightStream << fixed << setprecision(2) << temp.Height;
			weightStream << fixed << setprecision(2) << temp.Weight;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
void feedbackMenu(Accounts a, Doctors d) {

	Feedback ffm;
	vector<Feedback> Feedback;
	string displayString = "", sortColumn = "FeedbackID";
	bool ascending = true;

	//Show the list at first encounter
	Feedback = Feedback::getflistd(d.DoctorID, sortColumn, ascending);

	//Feedback Menu 
	Menu FM;
	FM.header = "Feedback\n\n";
	FM.addOption("->> Sort by");
	FM.addOption("->> Order by");
	FM.addOption("->> Search/Refresh");
	FM.addOption("->> Back");

	//FeedbackSortingSubMenu
	Menu FSSM;
	FSSM.header = "Choose the column that will be used to sort the list: \n";
	FSSM.addOption("->> FeedbackID");
	FSSM.addOption("->> Ratings");
	FSSM.addOption("->> Date");
	FSSM.addOption("->> PatientID");

	while (1) {
		FM.setValue(0, sortColumn);
		if (ascending) {
			FM.setValue(1, "Ascending");
		}
		else {
			FM.setValue(1, "Descending");
		}
		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "FeedbackID" << "|" << setw(12) << " Ratings"
				<< "|" << setw(15) << "Date" << "|" << setw(10) << "PatientID" << "|" << setw(60) << "Comments" << "|" << endl;

			for (int i = 0; i < Feedback.size(); i++) {
				tmpString << setw(10) << Feedback[i].FeedbackID << "|" << setw(12) << Feedback[i].Ratings
					<< "|" << setw(15) << Feedback[i].FDate << "|" << setw(10) << Feedback[i].PatientID << "|" << setw(60) << Feedback[i].Comments << "|" << endl;
			}
			displayString += tmpString.str();
		}
		FM.footer = displayString;

		switch (FM.prompt())
		{
		case 1:

			switch (FSSM.prompt())
			{
			case 1:
				sortColumn = "FeedbackID";
				break;
			case 2:
				sortColumn = "Ratings";
				break;
			case 3:
				sortColumn = "FDate";
				break;
			case 4:
				sortColumn = "PatientID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			Feedback = Feedback::getflistd(d.DoctorID, sortColumn, ascending);
			displayString = "";
			break;

		case 4:
			return;
			break;
		default:
			break;
		}
	}
}


//HomeMenuAdmin 
void homeAdmin(Accounts aa) {
	//HomeMenuAdmin
	Menu HMA;
	HMA.addOption("->> Accounts Management");
	HMA.addOption("->> Doctors Management");
	HMA.addOption("->> Patients Management");
	HMA.addOption("->> Departments Management");
	HMA.addOption("->> Appointments Management");
	HMA.addOption("->> Feedback Management");
	HMA.addOption("->> Statistics");
	HMA.addOption("->> Logout");
	HMA.addOption("->> Exit");
	HMA.footer = "\nEnter the number that represents your selection.";
	while (1) {

		HMA.header = "Welcome Admin\n\n";

		switch (HMA.prompt())
		{
		case 1:
			accountsManager();
			break;

		case 2:
			doctorsManager();
			break;

		case 3:
			patientsManager();
			break;

		case 4:
			departmentsManager();
			break;

		case 5:
			appointmentsManager();
			break;

		case 6:
			feedbackManager();
			break;

		case 7:
			statistics();
			break;

		case 8:
			return;
			break;
		case 9:
			exit(0);
			break;
		default:
			break;
		}
	}
}
void accountsManager() {

	Accounts a;
	vector<Accounts> Accounts;
	string displayString = "", keyWord = "", sortColumn = "AccountID";

	bool ascending = true;

	//Show the list at first encounter
	Accounts = Accounts::getalist(keyWord, sortColumn, ascending);

	Menu ALM;
	ALM.header = "Search Option";
	ALM.addOption("->> Keyword");
	ALM.addOption("->> Sort By");
	ALM.addOption("->> Ordering");
	ALM.addOption("->> Search/Refresh");
	ALM.addOption("->> Add Account");
	ALM.addOption("->> Update Account");
	ALM.addOption("->> Remove Account");
	ALM.addOption("->> Back");

	Menu ASSM;
	ASSM.header = "Choose the column that will be used to sort the list: \n";
	ASSM.addOption("->> AccountID");
	ASSM.addOption("->> Username");
	ASSM.addOption("->> Password");
	ASSM.addOption("->> Role");

	while (1)
	{
		ALM.setValue(1, sortColumn);
		if (ascending) {
			ALM.setValue(2, "Ascending");
		}
		else {
			ALM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "AccountID" << "|" << setw(23) << "Username"
				<< "|" << setw(15) << "Password" << "|" << setw(10) << "Role" << "|" << endl;

			for (int i = 0; i < Accounts.size(); i++) {
				tmpString << setw(9) << Accounts[i].AccountID << "|" << setw(23) << Accounts[i].Username
					<< "|" << setw(15) << Accounts[i].Password << "|" << setw(10) << Accounts[i].ARole << "|" << endl;
			}
			displayString += tmpString.str();
		}
		ALM.footer = displayString;

		switch (ALM.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			ALM.setValue(0, keyWord);
			break;

		case 2:
			switch (ASSM.prompt())
			{
			case 1:
				sortColumn = "AccountID";
				break;
			case 2:
				sortColumn = "Username";
				break;
			case 3:
				sortColumn = "Password";
				break;
			case 4:
				sortColumn = "ARole";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Accounts = Accounts::getalist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			addAccounts();
			break;

		case 6:
			cout << "Enter the AccountID to choose the account that needs to be updated: ";
			cin >> a.AccountID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				accountEditMenu(a.AccountID);
			}
			
			break;

		case 7:
			cout << "Enter the AccountID to choose the account that needs to be remove: ";
			cin >> a.AccountID; 
			
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the account? (y/n)";
				char confirm;
				confirm = _getch();
				if (confirm == 'Y' || confirm == 'y') {
					Accounts::removeAccount(a.AccountID);
					cout << "Done!";
					_getch();
					break;
				}
				break;
			}
			

		case 8:
			return;
			break;
		default:
			break;
		}
	};
}
void addAccounts() {

	Accounts accreg;
	accreg.ARole = -1;
	Menu RM1;
	RM1.header = "Add Account\n\n";
	RM1.addOption("->> Username");
	RM1.addOption("->> Password");
	RM1.addOption("->> Role");
	RM1.addOption("->> Make Account");
	RM1.addOption("->> Back");
	RM1.footer = "\nEnter the data required accordingly.";

	while (1) {
		switch (RM1.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> accreg.Username;

			if (accreg.Username.length() < 5) {
				cout << "Username must be at least 5 characters.";
				_getch();
				break;
			}

			if (accreg.usernameExisted(accreg.Username) == true) {
				cout << "This username is already in use. Please try a different one.";
				_getch();
				break;
			}
			RM1.setValue(0, accreg.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> accreg.Password;
			if (accreg.Password.length() < 8) {
				cout << "Password must be at least 8 characters.";
				_getch();
				break;
			}

			if (hasAlphabet(accreg.Password) == true && hasDigit(accreg.Password) == true) {
				RM1.setValue(1, accreg.Password);
				break;
			}
			else {
				cout << "The password needs to be made up of both numbers and letters.";
				_getch();
				break;
			}

			break;

		case 3:
			cout << "Please enter the role: " << endl;
			cout << "0 = Admin " << endl;
			cout << "1 = Doctor " << endl;
			cout << "2 = Patient " << endl;
			cout << ": ";
			cin >> accreg.ARole;
			RM1.setValue(2, to_string(accreg.ARole));
			break;

		case 4:
			if (accreg.Username.empty() || accreg.Password.empty() || to_string(accreg.ARole).empty() || accreg.ARole == -1) {
			cout << "Some required details should not be NULl or Space!!";
			_getch();
			break;
		}
			  else {
			accreg.AccountID = accreg.insertacc();
			cout << "Done!";
			_getch();
			break;

		}
			

		case 5:
			return;
			break;

		default:
			break;
		}
	}
}
void accountEditMenu(int AccountID) {
	Accounts a;
	a.AccountID = AccountID;
	a.getaccdata(a.AccountID);

	Accounts temp = a;

	//AccountEditMenu
	Menu AUM;
	AUM.header = "Account Update: \n";
	AUM.addOption("->> Username");
	AUM.addOption("->> Password");
	AUM.addOption("->> Role");
	AUM.addOption("->> Confirm");
	AUM.addOption("->> Back");
	AUM.addOption("->> Reset");
	AUM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		AUM.setValue(0, (temp.Username));
		AUM.setValue(1, temp.Password);
		AUM.setValue(2, to_string(temp.ARole));
		switch (AUM.prompt())
		{
		case 1:
			cout << "Please enter your username: ";
			cin >> temp.Username;
			if (temp.Username.length() < 5) {
				cout << "Username must be at least 5 characters.";
				temp.Username = a.Username;
				_getch();
				break;
			}

			if (temp.usernameExisted(temp.Username) == true) {
				cout << "This username is already in use. Please try a different one.";
				temp.Username = a.Username;
				_getch();
				break;
			}
			AUM.setValue(0, temp.Username);
			break;

		case 2:
			cout << "Please enter your password: ";
			cin >> temp.Password;

			if (temp.Password.length() < 8) {
				cout << "Password must be at least 8 characters.";
				temp.Password = a.Password;
				_getch();
				break;
			}

			if (hasAlphabet(temp.Password) == true && hasDigit(temp.Password) == true) {
				AUM.setValue(1, temp.Password);
				break;
			}
			else {
				cout << "The password needs to be made up of both numbers and letters.";
				temp.Password = a.Password;
				_getch();
				break;
			}

			break;

		case 3:
			cout << "Please enter the role: " << endl;
			cout << "0 = Admin " << endl;
			cout << "1 = Doctor " << endl;
			cout << "2 = Patient " << endl;
			cout << ": ";
			cin >> temp.ARole;
			AUM.setValue(2, to_string(temp.ARole));
			break;
		case 4:
			a = temp;
			a.updateacca();
			cout << "Updated!";
			_getch();
			break;
		case 5:
			return;
			break;
		case 6:
			temp = a;
			cout << "Reset!";
			_getch();
			break;
		default:
			break;
		}
	}

}
void doctorsManager() {
	Doctors d;
	vector<Doctors> Doctors;
	string displayString = "", keyWord = "", sortColumn = "DoctorID";
	int DepartmentID = -1;

	bool ascending = true;

	//Show the list at first encounter
	Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);

	//Doctor List Menu
	Menu DLM;
	DLM.header = "Search Option";
	DLM.addOption("->> Keyword");
	DLM.addOption("->> Sort By");
	DLM.addOption("->> Ordering");
	DLM.addOption("->> Search/Refresh");
	DLM.addOption("->> Add Doctor");
	DLM.addOption("->> Update Doctor");
	DLM.addOption("->> Remove Doctor");
	DLM.addOption("->> Back");

	//DoctorSortingSubMenu
	Menu DSSM;
	DSSM.header = "Choose the column that will be used to sort the list: \n";
	DSSM.addOption("->> Name");
	DSSM.addOption("->> Specialization");
	DSSM.addOption("->> DepartmentID");
	DSSM.addOption("->> DoctorID");

	while (1)
	{
		DLM.setValue(1, sortColumn);
		if (ascending) {
			DLM.setValue(2, "Ascending");
		}
		else {
			DLM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DoctorID" << "|" << setw(11) << "Name"
				<< "|" << setw(15) << "Mobile Number" << "|" << setw(20) << "Specialization" << "|" << setw(20) << "DepartmentID" << "|" << endl;

			for (int i = 0; i < Doctors.size(); i++) {
				tmpString << setw(8) << Doctors[i].DoctorID << "|" << setw(11) << Doctors[i].DName
					<< "|" << setw(15) << Doctors[i].DTelephoneNo << "|" << setw(20) << Doctors[i].Specialization << "|" << setw(20)
					<< Doctors[i].DepartmentID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DLM.footer = displayString;

		switch (DLM.prompt()) {
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DLM.setValue(0, keyWord);
			break;

		case 2:
			switch (DSSM.prompt())
			{
			case 1:
				sortColumn = "DName";
				break;
			case 2:
				sortColumn = "Specialization";
				break;
			case 3:
				sortColumn = "DepartmentID";
				break;
			case 4:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			addDoctors();
			break;
		case 6:
			cout << "Please enter the DoctorID for the profile you would like to update: ";
			cin >> d.DoctorID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				doctorProfileMenu(d.DoctorID);
			}

			break;

		case 7:
			cout << "Enter the DoctorID to choose the account that needs to be remove: ";
			cin >> d.DoctorID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the Doctor? (y/n)";
				char confirm1;
				confirm1 = _getch();

				if (confirm1 == 'Y' || confirm1 == 'y') {
					Doctors::removeDoctor(d.DoctorID);
					cout << "Done!";
					_getch();

					cout << "Delete the Account? (y/n)";
					char confirm2;
					confirm2 = _getch();
					if (confirm2 == 'Y' || confirm2 == 'y') {
						Accounts::removeAccount(d.AccountID);
						cout << "Done!";
						_getch();
						break;
					}
				}
			}
			break;

		case 8:
			return;
			break;
		default:
			break;
		}
	};
}
void addDoctors() {

	Doctors profiled;
	Departments dpt;
	profiled.DepartmentID = -1;
	profiled.AccountID = -1;
	Menu RM3;
	RM3.header = "Add Doctor\n\n";
	RM3.addOption("->> Name [Required]");
	RM3.addOption("->> Mobile Number (010-0000000) [Required]");
	RM3.addOption("->> Specialization [Required]");
	RM3.addOption("->> DepartmentID [Required]");
	RM3.addOption("->> AccountID [Required]");
	RM3.addOption("->> Confirm");
	RM3.addOption("->> Back");
	RM3.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		switch (RM3.prompt())
		{
		case 1:
			cout << "Please enter the name: ";
			cin >> profiled.DName;
			RM3.setValue(0, profiled.DName);
			break;

		case 2:
			cout << "Please enter the mobile number (010-0000000): ";
			cin >> profiled.DTelephoneNo;

			if (validateFormat(profiled.DTelephoneNo, R"(\d{3}-\d{7})") && profiled.DTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				RM3.setValue(1, profiled.DTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;
			}
			break;

		case 3:
			cout << "Please enter your specialization: ";
			cin >> profiled.Specialization;
			RM3.setValue(2, profiled.Specialization);
			break;

		case 4:
			listDepartments();
			profiled.DepartmentID = tempDepartmentID;
			RM3.setValue(3, to_string(profiled.DepartmentID));
			break;
		case 5:
			accountList();
			profiled.AccountID = accountSelected;
			RM3.setValue(4, to_string(profiled.AccountID));
			break;

		case 6:
			if (profiled.DName.empty() || profiled.DTelephoneNo.empty() || profiled.Specialization.empty() || to_string(profiled.DepartmentID).empty() || to_string(profiled.AccountID).empty() || profiled.DepartmentID == -1 || profiled.AccountID == -1)  {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				profiled.insertd();
				cout << "Done!";
				_getch();
				break;

			}
			
		case 7:
			return;
			break;

		default:
			break;
		}
	}
}
void doctorProfileMenu(int DoctorID) {
	Doctors d;
	d.DoctorID = DoctorID;
	d.getddatad(d.DoctorID);

	Doctors temp = d;

	//DoctorProfileMenu
	Menu DPM;
	DPM.header = "Doctor Profile Update\n\n";
	DPM.addOption("->> Name");
	DPM.addOption("->> Mobile Number (010-0000000)");
	DPM.addOption("->> Specialization");
	DPM.addOption("->> DepartmentID");
	DPM.addOption("->> AccountID");
	DPM.addOption("->> Confirm");
	DPM.addOption("->> Back");
	DPM.addOption("->> Reset");
	DPM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		DPM.setValue(0, temp.DName);
		DPM.setValue(1, temp.DTelephoneNo);
		DPM.setValue(2, temp.Specialization);
		DPM.setValue(3, to_string(temp.DepartmentID));
		DPM.setValue(4, to_string(temp.AccountID));

		switch (DPM.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> temp.DName;
			DPM.setValue(0, temp.DName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> temp.DTelephoneNo;
			if (validateFormat(temp.DTelephoneNo, R"(\d{3}-\d{7})") && temp.DTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				DPM.setValue(1, temp.DTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.DTelephoneNo = d.DTelephoneNo;
				_getch();
				break;
			}
			break;

		case 3:
			cout << "Please enter your specialization: ";
			cin >> temp.Specialization;
			DPM.setValue(2, temp.Specialization);
			break;

		case 4:
			listDepartments();
			temp.DepartmentID = tempDepartmentID;
			DPM.setValue(3, to_string(temp.DepartmentID));
			break;

		case 5:
			accountList();
			temp.AccountID = accountSelected;
			DPM.setValue(4, to_string(temp.AccountID));
			break;


		case 6: //Confirm
			d = temp;
			d.updated();
			cout << "Updated!";
			_getch();
			break;

		case 7: //Back
			return;
			break;

		case 8: //Reset
			temp = d;
			cout << "Reset!";
			_getch();
		default:
			break;
		}
	}
}
void patientsManager() {
	Patients p;
	vector<Patients> Patients;
	string displayString = "", keyWord = "", sortColumn = "PatientID";
	bool ascending = true;

	//Show the list at first encounter
	Patients = Patients::getplist(keyWord, sortColumn, ascending);

	//Doctor List Menu
	Menu DLM;
	DLM.header = "Search Option";
	DLM.addOption("->> Keyword");
	DLM.addOption("->> Sort By");
	DLM.addOption("->> Ordering");
	DLM.addOption("->> Search/Refresh");
	DLM.addOption("->> Add Patient");
	DLM.addOption("->> Update Patient");
	DLM.addOption("->> Remove Patient");
	DLM.addOption("->> Back");

	//DoctorSortingSubMenu
	Menu DSSM;
	DSSM.header = "Choose the column that will be used to sort the list: \n";
	DSSM.addOption("->> PatientID");
	DSSM.addOption("->> AccountID");
	DSSM.addOption("->> Name");
	DSSM.addOption("->> Mobile Number");
	DSSM.addOption("->> Gender");
	DSSM.addOption("->> DateOfBirth");
	DSSM.addOption("->> Height");
	DSSM.addOption("->> Weight");

	while (1)
	{
		DLM.setValue(1, sortColumn);
		if (ascending) {
			DLM.setValue(2, "Ascending");
		}
		else {
			DLM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2)
				<< setw(5) << "PatientID" << "|"
				<< setw(10) << "AccountID" << "|"
				<< setw(13) << "Name" << "|"
				<< setw(17) << "Mobile Number" << "|"
				<< setw(17) << "Gender" << "|"
				<< setw(17) << "DateOfBirth" << "|"
				<< setw(17) << "Height" << "|"
				<< setw(17) << "Weight" << "|" << endl;

			for (int i = 0; i < Patients.size(); i++) {
				tmpString << setw(9) << Patients[i].PatientID << "|"
					<< setw(10) << Patients[i].AccountID << "|"
					<< setw(13) << Patients[i].PName << "|"
					<< setw(17) << Patients[i].PTelephoneNo << "|"
					<< setw(17) << Patients[i].Gender << "|"
					<< setw(17) << Patients[i].DateOfBirth << "|"
					<< setw(17) << Patients[i].Height << "|"
					<< setw(17) << Patients[i].Weight << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DLM.footer = displayString;

		switch (DLM.prompt()) {
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DLM.setValue(0, keyWord);
			break;

		case 2:
			switch (DSSM.prompt())
			{
			case 1:
				sortColumn = "PatientID";
				break;
			case 2:
				sortColumn = "AccountID";
				break;
			case 3:
				sortColumn = "PName";
				break;
			case 4:
				sortColumn = "PTelephoneNo";
				break;
			case 5:
				sortColumn = "Gender";
				break;
			case 6:
				sortColumn = "DateOfBirth";
				break;
			case 7:
				sortColumn = "Height";
				break;
			case 8:
				sortColumn = "Weight";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Patients = Patients::getplist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			addPatients();
			break;
		case 6:
			cout << "Please enter the PatientID for the profile you would like to update: ";
			cin >> p.PatientID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				patientProfileMenu(p.PatientID);
			}

			break;

		case 7:
			cout << "Enter the PatientID to choose the patient that needs to be remove: ";
			cin >> p.PatientID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the patient? (y/n)";
				char confirm1;
				confirm1 = _getch();

				if (confirm1 == 'Y' || confirm1 == 'y') {
					Patients::removePatient(p.PatientID);
					cout << "Done!";
					_getch();

					cout << "Delete the Account? (y/n)";
					char confirm2;
					confirm2 = _getch();
					if (confirm2 == 'Y' || confirm2 == 'y') {
						Accounts::removeAccount(p.AccountID);
						cout << "Done!";
						_getch();
						break;
					}
				}
			}
			break;

		case 8:
			return;
			break;
		default:
			break;
		}
	};
}
void addPatients() {
	Patients profilep;
	profilep.AccountID = -1;
	ostringstream weightStream, heightStream;
	bool isMale = true;
	//Register Menu 2 (Into Patient)
	Menu RM2;
	RM2.header = "Add Patient\n\n";
	RM2.addOption("->> Name [Required]");
	RM2.addOption("->> Mobile Number (010-0000000) [Required]");
	RM2.addOption("->> Toggle to Change Your Gender");
	RM2.setValue(2, "Male");
	RM2.addOption("->> Date of Birth (YYYY-MM-DD) [Required]");
	RM2.addOption("->> Height (cm) [Optional]");
	RM2.addOption("->> Weight (kg) [Optional]");
	RM2.addOption("->> AccountID [Required]");
	RM2.addOption("->> Confirm");
	RM2.addOption("->> Back");
	RM2.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		//Toogle function
		if (isMale) {
			RM2.setValue(2, "Male");
			profilep.Gender = "Male";
		}
		else {
			RM2.setValue(2, "Female");
			profilep.Gender = "Female";
		}

		switch (RM2.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> profilep.PName;
			RM2.setValue(0, profilep.PName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> profilep.PTelephoneNo;
			if (validateFormat(profilep.PTelephoneNo, R"(\d{3}-\d{7})") && profilep.PTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				RM2.setValue(1, profilep.PTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;
			}
			break;

		case 3:
			isMale = !isMale;
			break;

		case 4:
			cout << "Please enter your date of birth (YYYY-MM-DD): ";
			cin >> profilep.DateOfBirth;

			if (validateFormat(profilep.DateOfBirth, 
				R"(\d{4}-\d{2}-\d{2})") && 
				isValidDOB(profilep.DateOfBirth.substr(0, 4), 
					profilep.DateOfBirth.substr(6, 2), 
					profilep.DateOfBirth.substr(9, 2))) {
				cout << "Valid format." << endl;
				RM2.setValue(3, profilep.DateOfBirth);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;
			}
			break;


		case 5:
			cout << "Please enter your height (cm): ";
			heightStream.str("");
			cin >> profilep.Height;
			heightStream << fixed << setprecision(2) << profilep.Height;
			RM2.setValue(4, heightStream.str());
			break;

		case 6:
			cout << "Please enter your weight (kg): ";
			weightStream.str("");
			cin >> profilep.Weight;
			weightStream << fixed << setprecision(2) << profilep.Weight;
			RM2.setValue(5, weightStream.str());
			break;

		case 7:
			accountList();
			profilep.AccountID = accountSelected;
			RM2.setValue(6, to_string(profilep.AccountID));
			break;

		case 8:
			if (profilep.PName.empty() || profilep.PTelephoneNo.empty() || profilep.DateOfBirth.empty() || to_string(profilep.AccountID).empty() || profilep.AccountID == -1) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				profilep.insertp();
				cout << "Done!";
				_getch();
				break;

			}
			

		case 9:
			return;
			break;

		default:
			break;

		}
	}
}
void patientProfileMenu(int PatientID) {

	Patients p;
	p.PatientID = PatientID;
	p.getpdatap(p.PatientID);

	Patients temp = p;

	ostringstream weightStream, heightStream;
	bool isMale;

	if (p.Gender == "Male") {
		temp.Gender = "Male";
		isMale = true;
	}
	else {
		temp.Gender = "Female";
		isMale = false;
	}

	heightStream << fixed << setprecision(2) << temp.Height;
	weightStream << fixed << setprecision(2) << temp.Weight;

	//PatientProfileMenu
	Menu PPM;
	PPM.header = "Patient Profile Update\n\n";
	PPM.addOption("->> Name");
	PPM.addOption("->> Mobile Number (010-0000000)");
	PPM.addOption("->> Toogle To Change Your Gender");
	PPM.addOption("->> Date of Birth (YYYY-MM-DD)");
	PPM.addOption("->> Height (cm)");
	PPM.addOption("->> Weight (kg)");
	PPM.addOption("->> Confirm");
	PPM.addOption("->> Back");
	PPM.addOption("->> Reset");
	PPM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		PPM.setValue(0, temp.PName);
		PPM.setValue(1, temp.PTelephoneNo);
		PPM.setValue(2, temp.Gender);
		PPM.setValue(3, temp.DateOfBirth);
		PPM.setValue(4, heightStream.str());
		PPM.setValue(5, weightStream.str());

		//Toogle function
		if (isMale) {
			PPM.setValue(2, "Male");
			temp.Gender = "Male";
		}
		else {
			PPM.setValue(2, "Female");
			temp.Gender = "Female";
		}

		switch (PPM.prompt())
		{
		case 1:
			cout << "Please enter your full name: ";
			cin >> temp.PName;
			PPM.setValue(0, temp.PName);
			break;

		case 2:
			cout << "Please enter your mobile number (010-0000000): ";
			cin >> temp.PTelephoneNo;
			if (validateFormat(temp.PTelephoneNo, R"(\d{3}-\d{7})") && temp.PTelephoneNo.substr(0, 2) == "01") {
				cout << "Valid format." << endl;
				PPM.setValue(1, temp.PTelephoneNo);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.PTelephoneNo = p.PTelephoneNo;
				_getch();
				break;
			}
			break;

		case 3:
			isMale = !isMale;
			break;

		case 4:
			cout << "Please enter your date of birth (YYYY-MM-DD): ";
			cin >> temp.DateOfBirth;
			if (validateFormat(temp.DateOfBirth, R"(\d{4}-\d{2}-\d{2})")) {
				cout << "Valid format." << endl;
				PPM.setValue(3, temp.DateOfBirth);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.DateOfBirth = p.DateOfBirth;
				_getch();
				break;

			}
			break;

		case 5:
			cout << "Please enter your height (cm): ";
			heightStream.str("");
			cin >> temp.Height;
			heightStream << fixed << setprecision(2) << temp.Height;
			PPM.setValue(4, heightStream.str());

			break;

		case 6:
			cout << "Please enter your weight (kg): ";
			weightStream.str("");
			cin >> temp.Weight;
			weightStream << fixed << setprecision(2) << temp.Weight;
			PPM.setValue(5, weightStream.str());

			break;

		case 7: //Confirm
			p = temp;
			p.updatep();
			cout << "Updated!";
			_getch();
			break;

		case 8: //Back
			return;
			break;

		case 9: //Reset
			temp = p;
			heightStream.str(""); // Clear the contents of the height stream
			weightStream.str(""); // Clear the contents of the weight stream
			heightStream << fixed << setprecision(2) << temp.Height;
			weightStream << fixed << setprecision(2) << temp.Weight;
			cout << "Reset!";
			_getch();

		default:
			break;
		}
	}
}
void departmentsManager() {
	Departments dpt;
	vector<Departments> Departments;
	string displayString = "", keyWord = "", sortColumn = "DepartmentID";
	int DepartmentID = -1;
	bool ascending = true;

	//Show the list at first encounter
	Departments = Departments::getdptlist(keyWord, sortColumn, ascending);

	//ViewDoctorList
	Menu VDPTL;
	VDPTL.header = "Search Option";
	VDPTL.addOption("->> Keyword");
	VDPTL.addOption("->> Sort By");
	VDPTL.addOption("->> Ordering");
	VDPTL.addOption("->> Search/Refresh");
	VDPTL.addOption("->> Add Department");
	VDPTL.addOption("->> Update Department");
	VDPTL.addOption("->> Remove Department");
	VDPTL.addOption("->> Back");

	//ViewDoctorSortingSubMenu
	Menu VDSSM;
	VDSSM.header = "Choose the column that will be used to sort the list: \n";
	VDSSM.addOption("->> DepartmentID");
	VDSSM.addOption("->> Name");

	while (1)
	{
		VDPTL.setValue(1, sortColumn);
		if (ascending) {
			VDPTL.setValue(2, "Ascending");
		}
		else {
			VDPTL.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DepartmentID" << "|" << setw(34) << "Name"
				<< "|" << setw(75) << "Description" << "|" << endl;

			for (int i = 0; i < Departments.size(); i++) {
				tmpString << setw(12) << Departments[i].DepartmentID << "|" << setw(34) << Departments[i].DptName
					<< "|" << setw(75) << Departments[i].Description << "|" << endl;
			}
			displayString += tmpString.str();
		}
		VDPTL.footer = displayString;

		switch (VDPTL.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			VDPTL.setValue(0, keyWord);
			break;

		case 2:
			switch (VDSSM.prompt())
			{
			case 1:
				sortColumn = "DepartmentID";
				break;
			case 2:
				sortColumn = "DptName";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Departments = Departments::getdptlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			addDepartments();
			break;
		case 6:
			cout << "Please enter the DepartmentID for the profile you would like to update: ";
			cin >> dpt.DepartmentID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				updateDepartmentMenu(dpt.DepartmentID);
			}

			break;
		case 7:
			cout << "Enter the DepartmentID to choose the department that needs to be remove: ";
			cin >> dpt.DepartmentID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the department? (y/n)";
				char confirm1;
				confirm1 = _getch();

				if (confirm1 == 'Y' || confirm1 == 'y') {
					Departments::removeDpt(dpt.DepartmentID);
					cout << "Done!";
					_getch();

				}
			}
			break;

		case 8:
			return;
			break;
		default:
			break;
		}
	};
}
void addDepartments() {
	Departments profiledpt;
	//Register Menu 2 (Into Patient)
	Menu RM2;
	RM2.header = "Add Department\n\n";
	RM2.addOption("->> Department Name [Required]");
	RM2.addOption("->> Description [Optional]");
	RM2.addOption("->> Confirm");
	RM2.addOption("->> Back");
	RM2.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		switch (RM2.prompt())
		{
		case 1:
			cout << "Please enter the department name: ";
			cin >> profiledpt.DptName;
			RM2.setValue(0, profiledpt.DptName);
			break;

		case 2:
			cout << "Please enter the department Description: ";
			cin >> profiledpt.Description;
			RM2.setValue(1, profiledpt.Description);
			break;

		case 3:
			if(profiledpt.DptName.empty()) {
				cout << "Department Name cannot be empty";
				_getch();
				break;
			}
			else {
				profiledpt.insertdpt();
				cout << "Done!";
				_getch();
				break;
			}
			
			break;

		case 4:
			return;
			break;

		default:
			break;

		}
	}
}
void updateDepartmentMenu(int DepartmentID) {
	Departments dpt;
	dpt.DepartmentID = DepartmentID;
	dpt.getdptdatadpt(dpt.DepartmentID);

	Departments temp = dpt;

	Menu RM2;
	RM2.header = "Department Update\n\n";
	RM2.addOption("->> Department Name [Required]");
	RM2.addOption("->> Description [Optional]");
	RM2.addOption("->> Confirm");
	RM2.addOption("->> Back");
	RM2.addOption("->> Reset");
	RM2.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		RM2.setValue(0, temp.DptName);
		RM2.setValue(1, temp.Description);

		switch (RM2.prompt())
		{
		case 1:
			cout << "Please enter the department name: ";
			cin >> temp.DptName;
			RM2.setValue(0, temp.DptName);
			break;

		case 2:
			cout << "Please enter the department Description: ";
			cin >> temp.Description;
			RM2.setValue(1, temp.Description);
			break;

		case 3: //Confirm
			dpt = temp;
			dpt.updatedpt();
			cout << "Updated!";
			_getch();
			break;

		case 4: //Back
			return;
			break;

		case 5: //Reset
			temp = dpt;
			cout << "Reset!";
			_getch();

		default:
			break;

		}
	}
}
void appointmentsManager(){
	Appointments appam;
	vector<Appointments> Appointments;
	string displayString = "", sortColumn = "AppointmentID";
	bool ascending = true;

	//Show the list at first encounter
	Appointments = Appointments::getapplistall(sortColumn, ascending);

	//AppointmentMenu
	Menu AM;
	AM.header = "Appointment\n\n";
	AM.addOption("->> Sort by");
	AM.addOption("->> Order by");
	AM.addOption("->> Search/Refresh");
	AM.addOption("->> Make Appointment");
	AM.addOption("->> Update Appointment");
	AM.addOption("->> Remove Appointment");
	AM.addOption("->> Back");

	//AppointmentSortingSubMenu
	Menu ASSM;
	ASSM.header = "Choose the column that will be used to sort the list: \n";
	ASSM.addOption("->> AppointmentID");
	ASSM.addOption("->> Date");
	ASSM.addOption("->> Time");
	ASSM.addOption("->> Status");
	ASSM.addOption("->> DoctorID");
	ASSM.addOption("->> PatientID");

	while (1) {

		AM.setValue(0, sortColumn);
		if (ascending) {
			AM.setValue(1, "Ascending");
		}
		else {
			AM.setValue(1, "Descending");
		}
		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "AppointmentID" << "|" 
				<< setw(12) << "Date" << "|" 
				<< setw(15) << "Time" << "|" 
				<< setw(20) << "Status" << "|" 
				<< setw(20) << "DoctorID" << "|" 
				<< setw(20) << "PatientID" << "|" << endl;

			for (int i = 0; i < Appointments.size(); i++) {
				tmpString << setw(13) << Appointments[i].AppointmentID << "|" 
					<< setw(12) << Appointments[i].ADate << "|" 
					<< setw(15) << Appointments[i].ATime << "|" 
					<< setw(20) << Appointments[i].AStatus << "|" 
					<< setw(20) << Appointments[i].DoctorID << "|" 
					<< setw(20) << Appointments[i].PatientID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		AM.footer = displayString;

		switch (AM.prompt())
		{
		case 1:
			switch (ASSM.prompt())
			{
			case 1:
				sortColumn = "AppointmentID";
				break;
			case 2:
				sortColumn = "ADate";
				break;
			case 3:
				sortColumn = "ATime";
				break;
			case 4:
				sortColumn = "AStatus";
				break;
			case 5:
				sortColumn = "DoctorID";
				break;
			case 6:
				sortColumn = "PatientID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			Appointments = Appointments::getapplistall(sortColumn, ascending);
			displayString = "";
			break;

		case 4:
			addAppointments();
			break;

		case 5:
			cout << "Please enter the AppointmentID for the appointment you would like to update: ";
			cin >> appam.AppointmentID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				updateAppointments(appam.AppointmentID);
			}

			break;

		case 6:
			cout << "Enter the AppointmentID to choose the appointment that needs to be remove: ";
			cin >> appam.AppointmentID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the appointment? (y/n)";
				char confirm1;
				confirm1 = _getch();

				if (confirm1 == 'Y' || confirm1 == 'y') {
					Appointments::removeApps(appam.AppointmentID);
					cout << "Done!";
					_getch();

				}
			}
			break;

		case 7:
			return;
			break;
		default:
			break;
		}
	}
}
void addAppointments(){
	Appointments appaam;
	string AStatus = "Pending";
	appaam.PatientID = -1;
	//Add Appointment Menu
	Menu AAM;
	AAM.header = "Make Appointment \n\n";
	AAM.addOption("->> Date (YYYY-MM-DD)");
	AAM.addOption("->> Time (08:00 - 18:00)");
	AAM.addOption("->> Status");
	AAM.addOption("->> DoctorID");
	AAM.addOption("->> PatientID");
	AAM.addOption("->> Confirm");
	AAM.addOption("->> Back");
	AAM.footer = "\nEnter the required data accordingly and confirm once done.";

	Menu ASAM;
	ASAM.header = "Select the status for this appointment \n\n";
	ASAM.addOption("->> Pending");
	ASAM.addOption("->> Canceled");
	ASAM.addOption("->> Succeeded");
	ASAM.addOption("->> Failed");

	while (1) {
		AAM.setValue(2, AStatus);
		switch (AAM.prompt())
		{
		case 1:
			cout << "Please enter the date you would like to schedule an appointment (YYYY-MM-DD): ";
			cin >> appaam.ADate;
			if (validateFormat(appaam.ADate, R"(\d{4}-\d{2}-\d{2})") && isValidDate(appaam.ADate.substr(0, 4), appaam.ADate.substr(5, 2), appaam.ADate.substr(8, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(0, appaam.ADate);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;

		case 2:
			cout << "Please enter the time you would like to schedule an appointment (HH:MM) (24 Hour Format): ";
			cin >> appaam.ATime;
			appaam.ATime += ":00";
			if (validateFormat(appaam.ATime, R"(\d{2}:\d{2}:\d{2})") && isValidTime(appaam.ATime.substr(0, 2), appaam.ATime.substr(3, 2), appaam.ATime.substr(6, 2))) {
				cout << "Valid format." << endl;
				AAM.setValue(1, appaam.ATime);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				_getch();
				break;

			}
			break;

		case 3:
			switch (ASAM.prompt())
			{
			case 1:
				AStatus = "Pending";
				break;
			case 2:
				AStatus = "Canceled";
				break;
			case 3:
				AStatus = "Succeeded";
				break;
			case 4:
				AStatus = "Failed";
				break;
			}
			break;
		case 4:
			doctorList();
			appaam.DoctorID = doctorSelected;
			AAM.setValue(3, to_string(appaam.DoctorID));
			break;

		case 5:
			patientList();
			appaam.PatientID = patientSelected;
			AAM.setValue(4, to_string(appaam.PatientID));
			break;
		case 6:
			if (appaam.ADate.empty() || appaam.ATime.empty() || to_string(appaam.DoctorID).empty() || to_string(appaam.PatientID).empty() || appaam.PatientID == -1) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				appaam.insertapp(AStatus);
				cout << "Completed!";
				_getch();
				break;

			}
			
		case 7:
			return;
		default:
			break;
		}
	}
}
void updateAppointments(int AppointmentID){

	
	Appointments app;
	app.AppointmentID = AppointmentID;
	app.getappdataa(app.AppointmentID);

	Appointments temp = app;

	Menu UAM;
	UAM.header = "Edit Appointment Details\n\n";
	UAM.addOption("->> Date (YYYY-MM-DD)");
	UAM.addOption("->> Time (08:00 - 18:00)");
	UAM.addOption("->> Status");
	UAM.addOption("->> DoctorID");
	UAM.addOption("->> PatientID");
	UAM.addOption("->> Confirm");
	UAM.addOption("->> Back");
	UAM.addOption("->> Reset");
	UAM.footer = "\nEnter the required data accordingly and confirm once done.";

	Menu ASAM;
	ASAM.header = "Select the status for this appointment \n\n";
	ASAM.addOption("->> Pending");
	ASAM.addOption("->> Canceled");
	ASAM.addOption("->> Succeeded");
	ASAM.addOption("->> Failed");

	while (1) {
		UAM.setValue(0, (temp.ADate));
		UAM.setValue(1, temp.ATime);
		UAM.setValue(2, temp.AStatus);
		UAM.setValue(3, to_string(temp.DoctorID));
		UAM.setValue(4, to_string(temp.PatientID));

		switch (UAM.prompt())
		{
		case 1:
			cout << "Please enter the date you would like to schedule an appointment (YYYY-MM-DD): ";
			cin >> temp.ADate;
			if (validateFormat(temp.ADate, R"(\d{4}-\d{2}-\d{2})") && isValidDate(temp.ADate.substr(0, 4), temp.ADate.substr(5, 2), temp.ADate.substr(8, 2))) {
				cout << "Valid format." << endl;
				UAM.setValue(0, temp.ADate);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.ADate = app.ADate;
				_getch();
				break;

			}
			break;

		case 2:
			cout << "Please enter the time you would like to schedule an appointment (HH:MM) (24 Hour Format): ";
			cin >> temp.ATime;
			temp.ATime += ":00";
			if (validateFormat(temp.ATime, R"(\d{2}:\d{2}:\d{2})") && isValidTime(temp.ATime.substr(0, 2), temp.ATime.substr(3, 2), temp.ATime.substr(6, 2))) {
				cout << "Valid format." << endl;
				UAM.setValue(1, temp.ATime);
				break;
			}
			else {
				cout << "Invalid format." << endl;
				temp.ATime = app.ATime;
				_getch();
				break;

			}
			break;

		case 3:
			switch (ASAM.prompt())
			{
			case 1:
				temp.AStatus = "Pending";
				break;
			case 2:
				temp.AStatus = "Canceled";
				break;
			case 3:
				temp.AStatus = "Succeeded";
				break;
			case 4:
				temp.AStatus = "Failed";
				break;
			}
			break;
		case 4:
			doctorList();
			temp.DoctorID = doctorSelected;
			UAM.setValue(3, to_string(temp.DoctorID));

			break;
			
		case 5:
			patientList();
			temp.PatientID = patientSelected;
			UAM.setValue(4, to_string(temp.PatientID));

			break;

		case 6:
			app = temp;
			app.updateapp(app.AStatus, app.AppointmentID);
			cout << "Updated!";
			_getch();

		case 7:
			return;
			break;
		case 8:
			temp = app;
			break;
		default:
			break;
		}
	}
}
void feedbackManager(){
	Feedback ffm;
	vector<Feedback> Feedback;
	string displayString = "", sortColumn = "FeedbackID";
	bool ascending = true;

	//Show the list at first encounter
	Feedback = Feedback::getflistall(sortColumn, ascending);

	//Feedback Menu 
	Menu FM;
	FM.header = "Feedback\n\n";
	FM.addOption("->> Sort by");
	FM.addOption("->> Order by");
	FM.addOption("->> Search/Refresh");
	FM.addOption("->> Make Feedback");
	FM.addOption("->> Update Feedback");
	FM.addOption("->> Remove Feedback");
	FM.addOption("->> Back");

	//FeedbackSortingSubMenu
	Menu FSSM;
	FSSM.header = "Choose the column that will be used to sort the list: \n";
	FSSM.addOption("->> FeedbackID");
	FSSM.addOption("->> Ratings");
	FSSM.addOption("->> Date");
	FSSM.addOption("->> DoctorID");

	while (1) {

		FM.setValue(0, sortColumn);
		if (ascending) {
			FM.setValue(1, "Ascending");
		}
		else {
			FM.setValue(1, "Descending");
		}
		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "FeedbackID" << "|"
				<< setw(12) << " Ratings" << "|"
				<< setw(15) << "Date" << "|"
				<< setw(10) << "DoctorID" << "|"
				<< setw(10) << "PatientID" << "|"
				<< setw(60) << "Comments" << "|" << endl;

			for (int i = 0; i < Feedback.size(); i++) {
				tmpString << setw(10) << Feedback[i].FeedbackID << "|"
					<< setw(12) << Feedback[i].Ratings << "|"
					<< setw(15) << Feedback[i].FDate << "|"
					<< setw(10) << Feedback[i].DoctorID << "|"
					<< setw(10) << Feedback[i].PatientID << "|"
					<< setw(60) << Feedback[i].Comments << "|" << endl;
			}
			displayString += tmpString.str();
		}
		FM.footer = displayString;

		switch (FM.prompt())
		{
		case 1:

			switch (FSSM.prompt())
			{
			case 1:
				sortColumn = "FeedbackID";
				break;
			case 2:
				sortColumn = "Ratings";
				break;
			case 3:
				sortColumn = "FDate";
				break;
			case 4:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 2:
			ascending = !ascending;
			break;

		case 3:
			Feedback = Feedback::getflistall(sortColumn, ascending);
			displayString = "";
			break;

		case 4:
			addFeedback();
			break;

		case 5:
			cout << "Enter the FeedbackID to choose the feedback that needs to be updated: ";
			cin >> ffm.FeedbackID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				updateFeedback(ffm.FeedbackID);

			}
			ffm.FeedbackID = -1;
			break;
		case 6:
			cout << "Enter the AppointmentID to choose the appointment that needs to be remove: ";
			cin >> ffm.FeedbackID;
			if (cin.fail()) {
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
			}
			else {
				cout << "Delete the feedback? (y/n)";
				char confirm1;
				confirm1 = _getch();

				if (confirm1 == 'Y' || confirm1 == 'y') {
					Feedback::removeFeedback(ffm.FeedbackID);
					cout << "Done!";
					_getch();

				}
			}
			break;
		case 7:
			return;
			break;
		default:
			break;
		}
	}
}
void addFeedback(){

	Feedback fafm;
	fafm.DoctorID = -1;
	fafm.PatientID = -1;
	//Add Feedback Menu 
	Menu AFM;
	AFM.header = "Make Feedback\n\n";
	AFM.addOption("->> Ratings (1-5) [Required]");
	AFM.addOption("->> Comments [Optional]");
	AFM.addOption("->> Date (YYYY-MM-DD) [Required] Press to get current date:");
	AFM.addOption("->> DoctorID [Required]");
	AFM.addOption("->> PatientID [Required]");
	AFM.addOption("->> Confirm");
	AFM.addOption("->> Back");
	AFM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {
		switch (AFM.prompt())
		{
		case 1:
			cout << "Please enter the ratings (1-5) : ";
			cin >> fafm.Ratings;
			if (isValidRatings(fafm.Ratings) == true) {
				std::cout << "Valid input: " << fafm.Ratings << std::endl;
				// Proceed with the valid input
				AFM.setValue(0, to_string(fafm.Ratings));
				break;
			}
			else {
				std::cout << "Invalid input. Please enter an integer between 1 and 5." << std::endl;
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
				_getch();
				break;
				// Handle the case of invalid input
			}
			break;

		case 2:
			cout << "Please enter the comments: : ";
			cin >> fafm.Comments;
			AFM.setValue(1, fafm.Comments);
			break;

		case 3: //retrive localtime
		{
			auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
			struct tm parts;

			localtime_s(&parts, &now);

			char buffer[11];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d", &parts);

			fafm.FDate = buffer;
		}
		AFM.setValue(2, fafm.FDate);
		break;

		case 4:
			doctorList();
			fafm.DoctorID = doctorSelected;
			AFM.setValue(3, to_string(fafm.DoctorID));
			break;
		case 5:
			patientList();
			fafm.PatientID = patientSelected;
			AFM.setValue(4, to_string(fafm.PatientID));
			break;

		case 6:
			if (to_string(fafm.Ratings).empty() || fafm.FDate.empty() || to_string(fafm.DoctorID).empty() || to_string(fafm.PatientID).empty() || fafm.DoctorID == -1 || fafm.PatientID == -1) {
				cout << "Some required details should not be NULl or Space!!";
				_getch();
				break;
			}
			else {
				fafm.insertf();
				cout << "Completed!";
				_getch();
				break;

			}
			
		case 7:
			return;
		default:
			break;
		}
	}
}
void updateFeedback(int FeedbackID) {

	Feedback fufm;
	fufm.FeedbackID = FeedbackID;
	fufm.getfdataf(FeedbackID);
	Feedback temp = fufm;

	//Update Feedback Menu 
	Menu UFM;
	UFM.header = "Edit Feedback Details\n\n";
	UFM.addOption("->> Ratings (1-5)");
	UFM.addOption("->> Comments [Optional]");
	UFM.addOption("->> Date [YYYY-MM-DD] Press to get current date: ");
	UFM.addOption("->> DoctorID");
	UFM.addOption("->> PatientID");
	UFM.addOption("->> Confirm");
	UFM.addOption("->> Back");
	UFM.addOption("->> Reset");
	UFM.footer = "\nEnter the required data accordingly and confirm once done.";

	while (1) {

		UFM.setValue(0, to_string(temp.Ratings));
		UFM.setValue(1, temp.Comments);
		UFM.setValue(2, temp.FDate);
		UFM.setValue(3, to_string(temp.DoctorID));
		UFM.setValue(4, to_string(temp.PatientID));
		switch (UFM.prompt())
		{
		case 1:
			cout << "Please enter the ratings (1-5) : ";
			cin >> temp.Ratings;
			if (isValidRatings(temp.Ratings) == true) {
				std::cout << "Valid input: " << temp.Ratings << std::endl;
				// Proceed with the valid input
				UFM.setValue(0, to_string(temp.Ratings));
				break;
			}
			else {
				std::cout << "Invalid input. Please enter an integer between 1 and 5." << std::endl;
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n'); // discard invalid input
				temp.Ratings = fufm.Ratings;
				_getch();
				break;
				// Handle the case of invalid input
			}
			break;


		case 2:
			cout << "Please enter the comments: : ";
			cin >> temp.Comments;
			UFM.setValue(1, temp.Comments);
			break;

		case 3:
		{
			auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
			struct tm parts;

			localtime_s(&parts, &now);

			char buffer[11];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d", &parts);

			temp.FDate = buffer;
		}
		UFM.setValue(2, temp.FDate);
		break;

		case 4:
			doctorList();
			temp.DoctorID = doctorSelected;
			UFM.setValue(3, to_string(temp.DoctorID));
			break;
		case 5:
			patientList();
			temp.PatientID = patientSelected;
			UFM.setValue(4, to_string(temp.PatientID));
			break;

		case 6:
			fufm = temp;
			fufm.updatef(fufm.FeedbackID);
			cout << "Updated!";
			_getch();

		case 7:
			return;
			break;
		case 8:
			temp = fufm;
			break;
		default:
			break;
		}
	}
}
void doctorList() {
	vector<Doctors> Doctors;
	string displayString = "", keyWord = "", sortColumn = "DoctorID";
	int DepartmentID = -1;

	bool ascending = true;

	//Show the list at first encounter
	Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);

	//Doctor List Menu
	Menu DLM;
	DLM.header = "Search Option";
	DLM.addOption("->> Keyword");
	DLM.addOption("->> Sort By");
	DLM.addOption("->> Ordering");
	DLM.addOption("->> Search/Refresh");
	DLM.addOption("->> Select");
	DLM.addOption("->> Back");

	//DoctorSortingSubMenu
	Menu DSSM;
	DSSM.header = "Choose the column that will be used to sort the list: \n";
	DSSM.addOption("->> Name");
	DSSM.addOption("->> Specialization");
	DSSM.addOption("->> DepartmentID");
	DSSM.addOption("->> DoctorID");

	while (1)
	{

		DLM.setValue(1, sortColumn);
		if (ascending) {
			DLM.setValue(2, "Ascending");
		}
		else {
			DLM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "DoctorID" << "|" << setw(11) << "Name"
				<< "|" << setw(15) << "TelephoneNo" << "|" << setw(20) << "Specialization" << "|" << setw(20) << "DepartmentID" << "|" << endl;

			for (int i = 0; i < Doctors.size(); i++) {
				tmpString << setw(8) << Doctors[i].DoctorID << "|" << setw(11) << Doctors[i].DName
					<< "|" << setw(15) << Doctors[i].DTelephoneNo << "|" << setw(20) << Doctors[i].Specialization << "|" << setw(20)
					<< Doctors[i].DepartmentID << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DLM.footer = displayString;

		switch (DLM.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DLM.setValue(0, keyWord);
			break;

		case 2:
			switch (DSSM.prompt())
			{
			case 1:
				sortColumn = "DName";
				break;
			case 2:
				sortColumn = "Specialization";
				break;
			case 3:
				sortColumn = "DepartmentID";
				break;
			case 4:
				sortColumn = "DoctorID";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Doctors = Doctors::getdlist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			cout << "Please enter the DoctorID you would like to schedule an appointment/ make a feedback with: ";
			cin >> doctorSelected;
			if (isNumeric(to_string(doctorSelected))) {
				return;
				break;
			}
			else
			{
				cout << "Error of your select";
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n');
				_getch();
				break;
			}
			

		case 6:
			return;
			break;
		default:
			break;
		}
	};
}
void accountList() {
	vector<Accounts> Accounts;
	string displayString = "", keyWord = "", sortColumn = "AccountID";
	bool ascending = true;

	//Show the list at first encounter
	Accounts = Accounts::getalist(keyWord, sortColumn, ascending);

	//List Menu
	Menu DLM;
	DLM.header = "Search Option";
	DLM.addOption("->> Keyword");
	DLM.addOption("->> Sort By");
	DLM.addOption("->> Ordering");
	DLM.addOption("->> Search/Refresh");
	DLM.addOption("->> Select");
	DLM.addOption("->> Back");

	//SortingSubMenu
	Menu ASSM;
	ASSM.header = "Choose the column that will be used to sort the list: \n";
	ASSM.addOption("->> AccountID");
	ASSM.addOption("->> Username");
	ASSM.addOption("->> Password");
	ASSM.addOption("->> Role");

	while (1)
	{

		DLM.setValue(1, sortColumn);
		if (ascending) {
			DLM.setValue(2, "Ascending");
		}
		else {
			DLM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2) << setw(5) << "AccountID" << "|" << setw(23) << "Username"
				<< "|" << setw(15) << "Password" << "|" << setw(10) << "Role" << "|" << endl;

			for (int i = 0; i < Accounts.size(); i++) {
				tmpString << setw(9) << Accounts[i].AccountID << "|" << setw(23) << Accounts[i].Username
					<< "|" << setw(15) << Accounts[i].Password << "|" << setw(10) << Accounts[i].ARole << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DLM.footer = displayString;

		switch (DLM.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DLM.setValue(0, keyWord);
			break;

		case 2:
			switch (ASSM.prompt())
			{
			case 1:
				sortColumn = "AccountID";
				break;
			case 2:
				sortColumn = "Username";
				break;
			case 3:
				sortColumn = "Password";
				break;
			case 4:
				sortColumn = "ARole";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Accounts = Accounts::getalist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			cout << "Please enter the AccountID you would like to select: ";
			cin >> accountSelected;
			if (isNumeric(to_string(accountSelected))) {
				return;
				break;
			}
			else
			{
				cout << "Error of your select";
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n');
				_getch();
				break;
			}
			return;
			break;

		case 6:
			return;
			break;
		default:
			break;
		}
	};
}
void patientList() {
	vector<Patients> Patients;
	string displayString = "", keyWord = "", sortColumn = "PatientID";
	bool ascending = true;

	//Show the list at first encounter
	Patients = Patients::getplist(keyWord, sortColumn, ascending);

	//List Menu
	Menu DLM;
	DLM.header = "Search Option";
	DLM.addOption("->> Keyword");
	DLM.addOption("->> Sort By");
	DLM.addOption("->> Ordering");
	DLM.addOption("->> Search/Refresh");
	DLM.addOption("->> Select");
	DLM.addOption("->> Back");

	//SortingSubMenu
	Menu DSSM;
	DSSM.header = "Choose the column that will be used to sort the list: \n";
	DSSM.addOption("->> PatientID");
	DSSM.addOption("->> AccountID");
	DSSM.addOption("->> Name");
	DSSM.addOption("->> Mobile Number");
	DSSM.addOption("->> Gender");
	DSSM.addOption("->> DateOfBirth");
	DSSM.addOption("->> Height");
	DSSM.addOption("->> Weight");

	while (1)
	{

		DLM.setValue(1, sortColumn);
		if (ascending) {
			DLM.setValue(2, "Ascending");
		}
		else {
			DLM.setValue(2, "Descending");
		}

		if (displayString == "") {
			displayString = "\nSearch Result: \n\n";
			stringstream tmpString;
			tmpString << fixed << setprecision(2)
				<< setw(5) << "PatientID" << "|"
				<< setw(10) << "AccountID" << "|"
				<< setw(15) << "Name" << "|"
				<< setw(18) << "Mobile Number" << "|"
				<< setw(15) << "Gender" << "|"
				<< setw(15) << "DateOfBirth" << "|"
				<< setw(15) << "Height" << "|"
				<< setw(15) << "Weight" << "|" << endl;

			for (int i = 0; i < Patients.size(); i++) {
				tmpString << setw(8) << Patients[i].PatientID << "|"
					<< setw(11) << Patients[i].AccountID << "|"
					<< setw(15) << Patients[i].PName << "|"
					<< setw(18) << Patients[i].PTelephoneNo << "|"
					<< setw(15) << Patients[i].Gender << "|"
					<< setw(15) << Patients[i].DateOfBirth << "|"
					<< setw(15) << Patients[i].Height << "|"
					<< setw(15) << Patients[i].Weight << "|" << endl;
			}
			displayString += tmpString.str();
		}
		DLM.footer = displayString;

		switch (DLM.prompt()) {

			/// the case will modify the variable used as parameter to call the search method
		case 1:
			cout << "Please enter the keyword: ";
			getline(cin, keyWord);
			DLM.setValue(0, keyWord);
			break;

		case 2:
			switch (DSSM.prompt())
			{
			case 1:
				sortColumn = "PatientID";
				break;
			case 2:
				sortColumn = "AccountID";
				break;
			case 3:
				sortColumn = "PName";
				break;
			case 4:
				sortColumn = "PTelephoneNo";
				break;
			case 5:
				sortColumn = "Gender";
				break;
			case 6:
				sortColumn = "DateOfBirth";
				break;
			case 7:
				sortColumn = "Height";
				break;
			case 8:
				sortColumn = "Weight";
				break;
			}
			break;

		case 3:
			ascending = !ascending;
			break;

		case 4:
			Patients = Patients::getplist(keyWord, sortColumn, ascending);
			displayString = "";
			break;

		case 5:
			cout << "Please enter the PatientID you would like to select: ";
			cin >> patientSelected;
			if (isNumeric(to_string(patientSelected)) ){
				return;
				break;
			}
			else
			{
				cout << "Error of your select";
				cin.clear(); // clear the error flag
				std::cin.ignore(100, '\n');
				_getch();
				break;
			}
			return;
			break;

		case 6:
			return;
			break;
		default:
			break;
		}
	};
}
void statistics() {
	Accounts a;
	int totalAcc = -1;
	totalAcc = Accounts::getTotalAccount();
	int totalaAdmin = -1;
	totalaAdmin = Accounts::getTotalRole(0);
	int totalaDoctor = -1;
	totalaDoctor = Accounts::getTotalRole(1);
	int totalaPatient= -1;
	totalaPatient = Accounts::getTotalRole(2);

	Doctors d;
	Doctors d1;
	int totalDoctor = 0;
	totalDoctor = Doctors::gettotal();
	string bestDoctor = "";
	d.getbestd();
	bestDoctor = d.DName;
	string worstDoctor = "";
	d1.getworstd();
	worstDoctor = d1.DName;


	Patients p;
	int totalPatient = 0;
	totalPatient = Patients::gettotal();

	int* ageGroup = Patients::getAgeGroup();
	int ageGroup1 = ageGroup[0];
	int ageGroup2 = ageGroup[1];
	int ageGroup3 = ageGroup[2];
	int ageGroup4 = ageGroup[3];

	int* BMIGroup = Patients::getBMIGroup();
	int BMIGroup1 = BMIGroup[0];
	int BMIGroup2 = BMIGroup[1];
	int BMIGroup3 = BMIGroup[2];
	int BMIGroup4 = BMIGroup[3];

	Departments dpt;
	int totalDepartment = -1;
	totalDepartment = Departments::gettotal();
	int* tdGroup = Departments::getDptDcount();
	int td1 = tdGroup[0];
	int td2 = tdGroup[1];
	int td3 = tdGroup[2];
	int td4 = tdGroup[3];
	int td5 = tdGroup[4];
	int td6 = tdGroup[5];
	int td7 = tdGroup[6];
	int td8 = tdGroup[7];
	int td9 = tdGroup[8];
	int td10 = tdGroup[9];

	Appointments app;
	int totalAppointment = -1;
	totalAppointment = Appointments::gettotal();
	int todaytotalAPP = -1;
	todaytotalAPP = Appointments::getTodayTotal();

	int tmrtotalAPP = -1;
	tmrtotalAPP = Appointments::getTmrTotal();

	int last2wkAPP = -1;
	last2wkAPP = Appointments::getLast2wkTotal();

	int next2wkAPP = -1;
	next2wkAPP = Appointments::getNext2wkTotal();

	Feedback f;
	int totalFeedback = -1;
	totalFeedback = Feedback::getTotal();
	int todaytotalF = -1;
	todaytotalF = Feedback::getTodayTotal();
	int last2wkF = -1;
	last2wkF = Feedback::getLast2wkTotal();
	ostringstream bestRatings, worstRatings;
	double bestRate = -1;
	double worstRate = -1;
	bestRate = d.getbestd();
	worstRate = d1.getworstd();
	bestRatings << fixed << setprecision(2) << bestRate;
	worstRatings << fixed << setprecision(2) << worstRate;
	

	Menu MM;
	MM.header = "Statistics\n";
	MM.addOption("->> Accounts");
	MM.addOption("->> Doctors");
	MM.addOption("->> Patients");
	MM.addOption("->> Departments");
	MM.addOption("->> Appointmemts");
	MM.addOption("->> Feedback");
	MM.addOption("->> Back");
	MM.footer = "\nEnter the number that represents your selection.";

	Menu AM;
	AM.header = "Accounts Statistics\n";
	AM.addOption("->> Total Accounts");
	AM.addOption("->> Total Accounts with Admin role");
	AM.addOption("->> Total Accounts with Doctor role");
	AM.addOption("->> Total Accounts with Patient role");
	AM.footer = "\nPress any key to go back";

	Menu DM;
	DM.header = "Doctors Statistics\n";
	DM.addOption("->> Total Doctor");
	DM.addOption("->> Best Doctor");
	DM.addOption("->> Worst Doctor");
	DM.footer = "\nPress any key to go back";

	Menu PM;
	PM.header = "Patients Statistics\n";
	PM.addOption("->> Total Patients");
	PM.addOption("->> Total Patients in age group before 21");
	PM.addOption("->> Total Patients in age group before 21 - 35");
	PM.addOption("->> Total Patients in age group before 36 - 65");
	PM.addOption("->> Total Patients in age group after 66");
	PM.addOption("->> Total Patients with Obese");
	PM.addOption("->> Total Patients with Overweight");
	PM.addOption("->> Total Patients with Normal");
	PM.addOption("->> Total Patients with Underweight");
	PM.footer = "\nPress any key to go back";

	Menu DPTM;
	DPTM.header = "Departments Statistics\n";
	DPTM.addOption("->>Total Departments");
	DPTM.addOption("->>Total Doctors on Cardiology");
	DPTM.addOption("->>Total Doctors on Orthopedics");
	DPTM.addOption("->>Total Doctors on Neurology");
	DPTM.addOption("->>Total Doctors on Pediatrics");
	DPTM.addOption("->>Total Doctors on Oncology");
	DPTM.addOption("->>Total Doctors on Gastroenterology");
	DPTM.addOption("->>Total Doctors on Dermatology");
	DPTM.addOption("->>Total Doctors on Radiology");
	DPTM.addOption("->>Total Doctors on Anesthesiology");
	DPTM.addOption("->>Total Doctors on Urology");
	DPTM.footer = "\nPress any key to go back";

	Menu APPM;
	APPM.header = "Appointmemts Statistics\n";
	APPM.addOption("->> Total Appointments");
	APPM.addOption("->> Total Appointments Today");
	APPM.addOption("->> Total Appointments Tomorrow");
	APPM.addOption("->> Total Appointments Last Two Weeks");
	APPM.addOption("->> Total Appointments Next Two Weeks");
	APPM.footer = "\nPress any key to go back";

	Menu FM;
	FM.header = "Feedback Statistics\n";
	FM.addOption("->> Total Feedback");
	FM.addOption("->> Total Feedback Today");
	FM.addOption("->> Total Feedback Last Two Weeks");
	FM.addOption("->> Best Ratings");
	FM.addOption("->> Best Ratings Doctor");
	FM.addOption("->> Worst Ratings");
	FM.addOption("->> Worst Ratings Doctor");
	FM.footer = "\nPress any key to go back";


	while (1) {
		
		switch (MM.prompt())
		{
		case 1:
			AM.setValue(0, to_string(totalAcc));
			AM.setValue(1, to_string(totalaAdmin));
			AM.setValue(2, to_string(totalaDoctor));
			AM.setValue(3, to_string(totalaPatient));
			switch (AM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;

		case 2:
			DM.setValue(0, to_string(totalDoctor));
			DM.setValue(1, bestDoctor);
			DM.setValue(2, worstDoctor);
			switch (DM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;
		case 3:
			PM.setValue(0, to_string(totalPatient));
			PM.setValue(1, to_string(ageGroup1));
			PM.setValue(2, to_string(ageGroup2));
			PM.setValue(3, to_string(ageGroup3));
			PM.setValue(4, to_string(ageGroup4));
			PM.setValue(5, to_string(BMIGroup1));
			PM.setValue(6, to_string(BMIGroup2));
			PM.setValue(7, to_string(BMIGroup3));
			PM.setValue(8, to_string(BMIGroup4));
			switch (PM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;
		case 4:
			DPTM.setValue(0, to_string(totalDepartment));
			DPTM.setValue(1, to_string(td1));
			DPTM.setValue(2, to_string(td2));
			DPTM.setValue(3, to_string(td3));
			DPTM.setValue(4, to_string(td4));
			DPTM.setValue(5, to_string(td5));
			DPTM.setValue(6, to_string(td6));
			DPTM.setValue(7, to_string(td7));
			DPTM.setValue(8, to_string(td8));
			DPTM.setValue(9, to_string(td9));
			DPTM.setValue(10, to_string(td10));
			switch (DPTM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;

		case 5:
			APPM.setValue(0, to_string(totalAppointment));
			APPM.setValue(1, to_string(todaytotalAPP));
			APPM.setValue(2, to_string(tmrtotalAPP));
			APPM.setValue(3, to_string(last2wkAPP));
			APPM.setValue(4, to_string(next2wkAPP));
			switch (APPM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;
		case 6:
			FM.setValue(0, to_string(totalFeedback));
			FM.setValue(1, to_string(todaytotalF));
			FM.setValue(2, to_string(last2wkF));
			FM.setValue(3, bestRatings.str());
			FM.setValue(4, bestDoctor);
			FM.setValue(5, worstRatings.str());
			FM.setValue(6, worstDoctor);
			switch (FM.prompt()) {
			case 1:
				break;
			default:
				break;
			}
			break;
		case 7:
			return;
			break;
		default:
			break;
		}
	}
}

//Main
int main() {
	//system("Color A");

	while (1) {
		mainMenu();
	}
}
void mainMenu() {
	//Main menu
	Menu MM;
	MM.header = "Welcome to Healthcare Appointments Scheduler\n\nAre you a new user or existing user?";
	MM.addOption("->> New user (Register)");
	MM.addOption("->> Existing user (Login for Patient)");
	MM.addOption("->> Existing user (Login for Doctor)");
	MM.addOption("->> Existing user (Login for Admin)");
	MM.addOption("->> Exit");
	//MM.addOption("->> Exit");
	MM.footer = "\nEnter the number that represents your selection.";

	while (1) {
		switch (MM.prompt())
		{
		case 1:
			registerAccount();
			break;

		case 2:
			loginAccount(2);
			break;
		case 3:
			loginAccount(1);
			break;
		case 4:
			loginAccount(0);
			break;

		case 5:
			exit(0);
		default:
			break;
		}
	}
}

//Miscellaneous Functions (Input validation)
bool hasAlphabet(const string& s) {
	for (char ch : s) {
		if (isalpha(ch)) {
			return true;
		}
	}
	return false;
}
bool hasDigit(const string& s) {
	for (char ch : s) {
		if (isdigit(ch)) {
			return true;
		}
	}
	return false;
}
bool validateFormat(const string& input, const string& formatRegex) {
	regex regexPattern(formatRegex);
	return regex_match(input, regexPattern);
}
bool isValidDate(const std::string& year, const std::string& month, const std::string& day) {
	int yearInt = std::stoi(year);
	int monthInt = std::stoi(month);
	int dayInt = std::stoi(day);

	// Array to store the number of days in each month
	int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	// Check for leap year and adjust February's days accordingly
	if (monthInt == 2) {
		daysInMonth[2] = isLeapYear(yearInt) ? 29 : 28;
	}

	// Basic checks for month and day
	if ((monthInt < 1 || monthInt > 12) || (dayInt < 1 || dayInt > daysInMonth[monthInt])) {
		return false;
	}

	// Check if the specified date is in the future (up to 1 year)
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	struct tm parts;
	localtime_s(&parts, &currentTime);

	return ((yearInt > parts.tm_year + 1900) && (yearInt <= parts.tm_year + 1901)) ||
		(yearInt == parts.tm_year + 1900 && monthInt > parts.tm_mon + 1) ||
		(yearInt == parts.tm_year + 1900 && monthInt == parts.tm_mon + 1 && dayInt > parts.tm_mday);
}
bool isValidTime(const string& hours, const string& minutes, const string& seconds) {
	int hoursInt = stoi(hours);
	int minutesInt = stoi(minutes);
	int secondsInt = stoi(seconds);

	// Basic checks for hours, minutes, and seconds
	return (hoursInt >= 8 && hoursInt <= 18) &&
		(minutesInt >= 0 && minutesInt <= 59) &&
		(secondsInt >= 0 && secondsInt <= 59);
}
bool isValidDOB(const std::string& year, const std::string& month, const std::string& day) {
	int yearInt = std::stoi(year);
	int monthInt = std::stoi(month);
	int dayInt = std::stoi(day);

	// Basic checks for month and day
	if ((monthInt < 1 || monthInt > 12) || (dayInt < 1 || dayInt > 31)) {
		return false;
	}

	// Array to store the number of days in each month
	int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	// Check for leap year and adjust February's days accordingly
	if (monthInt == 2) {
		daysInMonth[2] = isLeapYear(yearInt) ? 29 : 28;
	}

	// Check if the specified date is within the range of 1923 to today
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	struct tm parts;
	localtime_s(&parts, &currentTime);

	// Check if the date is in the future or not within the range
	return ((yearInt >= 1923) && (yearInt <= parts.tm_year + 1900) &&
		(yearInt < parts.tm_year + 1900 ||
			(yearInt == parts.tm_year + 1900 && monthInt < parts.tm_mon + 1) ||
			(yearInt == parts.tm_year + 1900 && monthInt == parts.tm_mon + 1 && dayInt <= parts.tm_mday)));
}
bool isValidRatings(int Ratings) {
	if (Ratings >= 1 && Ratings <= 5) {
		return true;
	}
	return false;
}
bool isNumeric(const std::string& str) {
	std::istringstream ss(str);
	double value;
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
