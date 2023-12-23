#include "Notifications.h"

//Constructor
Notifications::Notifications() {
	AppointmentID = 0;
	NotificationID = 0;
	Reason = "";
	NStatus = "";

}
Notifications::Notifications(sql::ResultSet* data) {

	AppointmentID = data->getInt("AppointmentID");
	NotificationID = data->getInt("NotificationID");
	Reason = data->getString("Reason");
	NStatus = data->getString("NStatus");

}

//Insert for avoid repeating about (Reason = Time) (NStatus = Done)
void Notifications::updatens(string NStatus, int NotificationID) {

	DatabaseConnector db;
	db.prepareStatement("Update Notifications set NStatus = ? WHERE NotificationID = ? ");
	db.stmt->setString(1, NStatus);
	db.stmt->setInt(2, NotificationID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Return notification data(appointmentID) //For machine read data
void Notifications::getndataapp(int AppointmentID) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Notifications WHERE AppointmentID = ? ");
	db.stmt->setInt(1, AppointmentID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AppointmentID = db.res->getInt("AppointmentID");
			NotificationID = db.res->getInt("NotificationID");
			Reason = db.res->getString("Reason");
			NStatus = db.res->getString("NStatus");
		}
	}
	db.~DatabaseConnector();
}

//For Patients Notifications
//check the condition of send notification is fulfilled or not
void Notifications::conditioncheckerp(chrono::minutes::rep minutesDifference, string AStatus, string NStatus, string Reason, string ADate, string ATime, int DoctorID, string DName, int NotificationID, int AppointmentIDa, int AppointmentIDn)
{
	if ((minutesDifference > 0 && minutesDifference <= 60) && AStatus == "Succeeded" && NStatus == "Yet" && Reason == "TimeP") {

		string messaget1 = "Your appointment is in less than an hour!";
		string messaget2 =	"The Appointment details: \n"
							"Date: " + ADate + "\n" +
							"Time: " + ATime + "\n" +
							"Doctor's Name: " + DName + "\n"
							"DoctorID: " + to_string(DoctorID) + "\n";
							
		wstring wMessaget1 = Toast::stringToWstring(messaget1);
		wstring wMessaget2 = Toast::stringToWstring(messaget2);
		Toast::toastingmsg(wMessaget1,wMessaget2);

		Notifications::updatens("Done", NotificationID);	
	}
	if ((AStatus == "Succeeded" || AStatus == "Failed") && NStatus == "Yet" && Reason == "ToPatient") {
		string messages1 = "There has been an update regarding your appointment!";
		string messages2 = 	"Date: " + ADate + "\n" +
							"Time: " + ATime + "\n" +
							"Status: " + AStatus + "\n" +
							"Doctor's Name: " + DName + "\n"
							"DoctorID: " + to_string(DoctorID) + "\n";

		wstring wMessages1 = Toast::stringToWstring(messages1);
		wstring wMessages2 = Toast::stringToWstring(messages2);
		Toast::toastingmsg(wMessages1, wMessages2);

		Notifications::updatens("Done", NotificationID);
	}
}

//For Doctors Notifications
//check the condition of send notification is fulfilled or not
void Notifications::conditioncheckerd(chrono::minutes::rep minutesDifference, string AStatus, string NStatus, string Reason, string ADate, string ATime, int PatientID, string PName, int NotificationID, int AppointmentID)
{
	if ((minutesDifference > 0 && minutesDifference <= 60) && AStatus == "Succeeded" && NStatus == "Yet" && Reason == "TimeD") {

		string messaget1 = "Your appointment is in less than an hour!";
				string messaget2 = "The Appointment details: \n"
			"Date: " + ADate + "\n" +
			"Time: " + ATime + "\n" +
			"Patient's Name: " + PName + "\n"
			"PatientID: " + to_string(PatientID) + "\n";

		wstring wMessaget1 = Toast::stringToWstring(messaget1);
		wstring wMessaget2 = Toast::stringToWstring(messaget2);
		Toast::toastingmsg(wMessaget1, wMessaget2);

		Notifications::updatens("Done", NotificationID);
		}
	if ((AStatus == "Pending" || AStatus == "Canceled") && NStatus == "Yet" && Reason == "ToDoctor") {
		string messages1 = "There has been an update regarding your appointment!";
				string messages2 = "Date: " + ADate + "\n" +
							"Time: " + ATime + "\n" +
							"Status: " + AStatus + "\n" +
							"Patient's Name: " + PName + "\n"
							"PatientID: " + to_string(PatientID) + "\n";

		wstring wMessages1 = Toast::stringToWstring(messages1);
		wstring wMessages2 = Toast::stringToWstring(messages2);
		Toast::toastingmsg(wMessages1, wMessages2);

		Notifications::updatens("Done", NotificationID);
	}
}

vector<Notifications> Notifications::getNdataApp(int AppointmentID) {

	string query = "SELECT * FROM `Notifications` WHERE AppointmentID  =  ?";

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, AppointmentID);

	vector<Notifications> Notificationsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Notifications tempNotifications(db.res);
			Notificationsl.push_back(tempNotifications);
		}
	}
	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}
	db.~DatabaseConnector();
	return Notificationsl;
}
//send notication because time is close //Dumped
//void Notifications::sendtimemsgp(string ADate, string ATime, int DoctorID, string DName) {
//
//	string message = "Your appointment is in less than an hour!\n\nThe Appointment details: \n" 
//		"Date: " + ADate + "\n" + 
//		"Time: " +ATime + "\n" + 
//		"DoctorID: " + to_string(DoctorID) + "\n" +
//		"Doctor's Name: " + DName + "\n";
//
//	AllocConsole();
//	HWND consoleWindow = GetConsoleWindow();
//	ShowWindow(consoleWindow, SW_SHOWNORMAL);
//
//	std::wstringstream wss;
//	wss << message.c_str();
//	MessageBox(NULL, wss.str().c_str(), L"Notification", MB_OK);
//	if (IDOK) {
//		FreeConsole();
//	}
//	
//}

//send notication because status is changing // Dumped
//void Notifications::sendstatusmsgp(int AppointmentID, string ADate, string ATime, string AStatus, int DoctorID, string DName) {
//
//	string message = "There has been an update regarding your appointment!\nPlease have a look at it! \n\n"
//		"AppointmentID: " + to_string(AppointmentID) + "\n" +
//		"Date: " + ADate + "\n" +
//		"Time: " + ATime + "\n" +
//		"Status: " + AStatus + "\n" +
//		"DoctorID: " + to_string(DoctorID) + "\n" +
//		"Doctor's Name: " + DName + "\n";
//
//	cout << message;
//	_getch();
//
//	AllocConsole();
//	HWND consoleWindow = GetConsoleWindow();
//	ShowWindow(consoleWindow, SW_SHOWNORMAL); // SW_SHOWNORMAL, SW_SHOWDEFAULT, SW_HIDE
//
//	std::wstringstream wss;
//	wss << message.c_str();
//	MessageBox(NULL, wss.str().c_str(), L"Notification", MB_OK);
//
//	FreeConsole();
//
//
//}
