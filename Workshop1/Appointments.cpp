#include "Appointments.h"
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
#include <chrono>
#include <iostream>

//Constructor
Appointments::Appointments() {
	AppointmentID = 0;
	PatientID = 0;
	DoctorID = 0;
	ADate = "";
	ATime = "";
	AStatus = "";
}

Appointments::Appointments(sql::ResultSet* data) {

	AppointmentID = data->getInt("AppointmentID");
	DoctorID = data->getInt("DoctorID");
	PatientID = data->getInt("PatientID");
	ADate = data->getString("ADate");
	ATime = data->getString("ATime");
	AStatus = data->getString("AStatus");
}

//Insert for appointment (Make Appointment)
void Appointments::insertapp(string AStatus){
	DatabaseConnector db;
	db.prepareStatement("Insert into Appointments (ADate, ATime, AStatus, PatientID, DoctorID) VALUES (?,?,?,?,?)");
	db.stmt->setString(1, ADate);
	db.stmt->setString(2, ATime);
	db.stmt->setString(3, AStatus);
	db.stmt->setInt(4, PatientID);
	db.stmt->setInt(5, DoctorID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Genaral update
void Appointments::updateapp(string AStatus, int AppointmentID) {

	DatabaseConnector db;
	db.prepareStatement("Update Appointments set AStatus = ?, ADate = ?,  ATime = ?, DoctorID = ? WHERE AppointmentID = ? ");
	db.stmt->setString(1, AStatus);
	db.stmt->setString(2, ADate);
	db.stmt->setString(3, ATime);
	db.stmt->setInt(4, DoctorID);
	db.stmt->setInt(5, AppointmentID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Update appointment (status)
void Appointments::updateapps(string AStatus, int AppointmentID) {

	DatabaseConnector db;
	db.prepareStatement("Update Appointments set AStatus = ? WHERE AppointmentID = ? ");
	db.stmt->setString(1, AStatus);
	db.stmt->setInt(2, AppointmentID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Return appointment data(AppointmentID) //For machine read data
void Appointments::getappdataa(int AppointmentID) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Appointments WHERE AppointmentID = ? ");
	db.stmt->setInt(1, AppointmentID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AppointmentID = db.res->getInt("AppointmentID");
			DoctorID = db.res->getInt("DoctorID");
			PatientID = db.res->getInt("PatientID");
			ADate = db.res->getString("ADate");
			ATime = db.res->getString("ATime");
			AStatus = db.res->getString("AStatus");
		}
	}
	db.~DatabaseConnector();
}

//Return appointment data(PatientID) //For machine read data
void Appointments::getappdatap(int PatientID) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Appointments WHERE PatientID = ? ");
	db.stmt->setInt(1, PatientID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AppointmentID = db.res->getInt("AppointmentID");
			DoctorID = db.res->getInt("DoctorID");
			PatientID = db.res->getInt("PatientID");
			ADate = db.res->getString("ADate");
			ATime = db.res->getString("ATime");
			AStatus = db.res->getString("AStatus");
		}
	}
	db.~DatabaseConnector();
}

//Return appointment data //For user dynamic query
vector<Appointments> Appointments::getapplistp(int PatientID, string sortColumn, bool ascending, bool isAll) {

	string query;

	if (isAll) {
		
		query = "SELECT * FROM `Appointments` WHERE PatientID  =  ? " " ORDER BY " + sortColumn;
	}
	else {
		query = "SELECT * FROM Appointments WHERE PatientID = ? "
			" AND (ADate > CURDATE() OR (ADate = CURDATE() AND ATime >= CURTIME())) "
			" ORDER BY " + sortColumn;
	}
	

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}
	
	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, PatientID);

	vector<Appointments> Appointmentsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Appointments tempAppointments(db.res);
			Appointmentsl.push_back(tempAppointments);
		}

	}

	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}
	db.~DatabaseConnector();
	return Appointmentsl;
}

//Return appointment data //For user dynamic query
vector<Appointments> Appointments::getapplistd(int DoctorID, string sortColumn, bool ascending, bool isAll) {

	string query;

	if (isAll) {
		query = "SELECT * FROM `Appointments` WHERE DoctorID  =  ? " " ORDER BY " + sortColumn;
	}
	else {
		query = "SELECT * FROM Appointments WHERE DoctorID = ? "
			" AND (ADate > CURDATE() OR (ADate = CURDATE() AND ATime >= CURTIME())) "
			" ORDER BY " + sortColumn;
	}	


	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, DoctorID);

	vector<Appointments> Appointmentsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Appointments tempAppointments(db.res);
			Appointmentsl.push_back(tempAppointments);
		}

	}
	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}
	db.~DatabaseConnector();
	return Appointmentsl;
}

//Return appointment data //For user dynamic query
vector<Appointments> Appointments::getappdatapd(int PatientID, int DoctorID) {

	string query = "SELECT * FROM `Appointments` WHERE PatientID  =  ? OR DoctorID = ?";

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, PatientID);
	db.stmt->setInt(2, DoctorID);

	vector<Appointments> Appointmentsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Appointments tempAppointments(db.res);
			Appointmentsl.push_back(tempAppointments);
		}
	}
	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}
	db.~DatabaseConnector();
	return Appointmentsl;
}

vector<Appointments> Appointments::getapplistall(string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Appointments` " " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);

	vector<Appointments> Appointmentsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Appointments tempAppointments(db.res);
			Appointmentsl.push_back(tempAppointments);
		}
	}
	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}
	db.~DatabaseConnector();
	return Appointmentsl;
}


//Remove departments
void Appointments::removeApps(int AppointmentID) {

	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Appointments WHERE AppointmentID=?");
	db.stmt->setInt(1, AppointmentID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

int Appointments::gettotal() {
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("Select COUNT(AppointmentID) AS totalacc FROM Appointments");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();
}

int Appointments::getTodayTotal(){
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("SELECT COUNT(*) AS FeedbackCount FROM Appointments	WHERE ADate = CURDATE();");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("FeedbackCount");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();
}
int Appointments::getTmrTotal(){
	DatabaseConnector db;
	int totalAppointments = -1;

	// Adjust the query to select appointments for tomorrow
	db.prepareStatement("SELECT COUNT(*) AS AppointmentCount FROM Appointments WHERE ADate = DATE_ADD(CURDATE(), INTERVAL 1 DAY)");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAppointments = db.res->getInt("AppointmentCount");
		}
	}
	return totalAppointments;
	db.~DatabaseConnector();
}
int Appointments::getLast2wkTotal(){
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("SELECT COUNT(*) AS FeedbackCount FROM Appointments WHERE ADate >= DATE_SUB(CURDATE(), INTERVAL 14 DAY)");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("FeedbackCount");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();
}
int Appointments::getNext2wkTotal(){
	DatabaseConnector db;
	int totalAppointments = -1;

	// Adjust the query to select appointments for tomorrow
	db.prepareStatement("SELECT COUNT(*) AS AppointmentCount FROM Appointments WHERE ADate BETWEEN CURDATE() AND DATE_ADD(CURDATE(), INTERVAL 14 DAY)");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAppointments = db.res->getInt("AppointmentCount");
		}
	}
	return totalAppointments;
	db.~DatabaseConnector();
}