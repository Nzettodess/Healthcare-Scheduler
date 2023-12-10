#include "Patients.h"
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
#include <iostream>

//Constructor
Patients::Patients() {
	PatientID = 0;
	AccountID = 0;
	PName = ""; 
	PTelephoneNo = ""; 
	Gender = ""; 
	DateOfBirth = "";
	Height = 0; 
	Weight = 0;
}
Patients::Patients(sql::ResultSet* data) {
	PatientID = data->getInt("PatientID");
	PName = data->getString("PName");
	PTelephoneNo = data->getString("PTelephoneNo");
	Gender = data->getString("Gender");
	DateOfBirth = data->getString("DateOfBirth");
	AccountID = data->getInt("AccountID");
	Height = data->getDouble("Height");
	Weight = data->getDouble("Weight");
}

//Insert for profile patient
void Patients::insertp() {

	DatabaseConnector db;
	db.prepareStatement("Insert into Patients (PName, PTelephoneNo, Gender, DateOfBirth, Height, Weight, AccountID) VALUES (?,?,?,?,?,?,?)");
	db.stmt->setString(1, PName);
	db.stmt->setString(2, PTelephoneNo);
	db.stmt->setString(3, Gender);
	db.stmt->setString(4, DateOfBirth);
	db.stmt->setDouble(5, Height);
	db.stmt->setDouble(6, Weight);
	db.stmt->setInt(7, AccountID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Update profile patient
void Patients::updatep() {

	DatabaseConnector db;
	db.prepareStatement("Update Patients set PName = ?,  PTelephoneNo = ?, Gender= ?, DateOfBirth = ?, Height = ?, Weight = ? WHERE PatientID = ? ");
	db.stmt->setString(1, PName);
	db.stmt->setString(2, PTelephoneNo);
	db.stmt->setString(3, Gender);
	db.stmt->setString(4, DateOfBirth);
	db.stmt->setDouble(5, Height);
	db.stmt->setDouble(6, Weight);
	db.stmt->setInt(7, PatientID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Return PatientID for current user
/*int Patients::findpid(int AccountID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT PatientID from Patients WHERE AccountID = ? ");
	db.stmt->setInt(1, AccountID);

	db.QueryResult();
	int PatientID = -1;
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			PatientID = db.res->getInt64("PatientID");
		}
	}
	return PatientID;
	db.~DatabaseConnector();
}*/

//Return patient profile data (using accountID)
void Patients::getpdataa(int AccountID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT * from Patients WHERE AccountID = ? ");
	db.stmt->setInt(1, AccountID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			PatientID = db.res->getInt("PatientID");
			PName = db.res->getString("PName");
			PTelephoneNo = db.res->getString("PTelephoneNo");
			Gender = db.res->getString("Gender");
			DateOfBirth = db.res->getString("DateOfBirth");
			Height = db.res->getDouble("Height");
			Weight = db.res->getDouble("Weight");

		}
	}
	db.~DatabaseConnector();
}

//Return doctor profile data (using PatientID)
void Patients::getpdatap(int PatientID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT * from Patients WHERE PatientID = ? ");
	db.stmt->setInt(1, PatientID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			PatientID = db.res->getInt("PatientID");
			PName = db.res->getString("PName");
			PTelephoneNo = db.res->getString("PTelephoneNo");
			Gender = db.res->getString("Gender");
			DateOfBirth = db.res->getString("DateOfBirth");
			Height = db.res->getDouble("Height");
			Weight = db.res->getDouble("Weight");

		}
	}
	db.~DatabaseConnector();
}

vector<Patients> Patients::getplist(string keyword, string sortColumn, bool ascending) {
	string query = "SELECT * FROM `Patients` WHERE "	" (PName LIKE ?) "  " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}
	// 
	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setString(1, "%" + keyword + "%");

	vector<Patients> Patientsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Patients tempPatients(db.res);
			Patientsl.push_back(tempPatients);
		}
	}
	else {
		cout << "Empty!";
		_getch();
	}

	db.~DatabaseConnector();
	return Patientsl;
}

void Patients::removePatient(int PatientID) {
	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Patients WHERE PatientID = ?");
	db.stmt->setInt(1, PatientID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

int* Patients::getAgeGroup() {
	DatabaseConnector db;
	db.prepareStatement("SELECT COUNT(CASE WHEN TIMESTAMPDIFF(YEAR, DateOfBirth, CURDATE()) BETWEEN 21 AND 35 THEN 1 END) AS Age21To35, COUNT(CASE WHEN TIMESTAMPDIFF(YEAR, DateOfBirth, CURDATE()) > 65 THEN 1 END) AS AgeAbove65, COUNT(CASE WHEN TIMESTAMPDIFF(YEAR, DateOfBirth, CURDATE()) BETWEEN 36 AND 65 THEN 1 END) AS Age36To65, COUNT(CASE WHEN TIMESTAMPDIFF(YEAR, DateOfBirth, CURDATE()) < 21 THEN 1 END) AS AgeBelow21 FROM Patients");

	int* ageGroup = new int[4] { 0,0,0,0 };

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			ageGroup[0] = db.res->getInt("AgeBelow21");
			ageGroup[1] = db.res->getInt("Age21To35");
			ageGroup[2] = db.res->getInt("Age36To65");
			ageGroup[3] = db.res->getInt("AgeAbove65");
			}
	}
	return ageGroup;
	db.~DatabaseConnector();
}

int* Patients::getBMIGroup() {
	DatabaseConnector db;
	db.prepareStatement("SELECT CASE WHEN (Weight / (Height * Height)) >= 30 THEN 'Obese' WHEN (Weight / (Height * Height)) >= 25 THEN 'Overweight' WHEN (Weight / (Height * Height)) >= 18.5 THEN 'Normal' ELSE 'Underweight' END AS BMI_Category, COUNT(*) AS PatientCount FROM Patients GROUP BY BMI_Category;");

	int* ageGroup = new int[4] { 0, 0, 0, 0 };

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			ageGroup[0] = db.res->getInt("AgeBelow21");
			ageGroup[1] = db.res->getInt("Age21To35");
			ageGroup[2] = db.res->getInt("Age36To65");
			ageGroup[3] = db.res->getInt("AgeAbove65");
		}
	}
	return ageGroup;
	db.~DatabaseConnector();
}

int Patients::gettotal() {
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("Select COUNT(DoctorID) AS totalacc FROM Doctors");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();
}