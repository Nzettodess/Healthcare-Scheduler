#include "Doctors.h"
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
#include <iostream>

//Constructor
Doctors::Doctors() {
	DoctorID = 0; 
	DepartmentID = 0;
	AccountID = 0;
	DName = "";  
	DTelephoneNo = ""; 
	Specialization = "";  
}

Doctors::Doctors(sql::ResultSet* data) {
	DoctorID = data->getInt("DoctorID");
	DepartmentID = data->getInt("DepartmentID");
	DName = data->getString("DName");
	DTelephoneNo = data->getString("DTelephoneNo");
	Specialization = data->getString("Specialization");
	AccountID = data->getInt("AccountID");
}

//Insert for profile doctor
void Doctors::insertd() {

	DatabaseConnector db;
	db.prepareStatement("Insert into Doctors (DName, DTelephoneNo, Specialization, DepartmentID, AccountID) VALUES (?,?,?,?,?)");
	db.stmt->setString(1, DName);
	db.stmt->setString(2, DTelephoneNo);
	db.stmt->setString(3, Specialization);
	db.stmt->setInt(4, DepartmentID);
	db.stmt->setInt(5, AccountID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Update profile doctor
void Doctors::updated() {

	DatabaseConnector db;
	db.prepareStatement("Update Doctors set DName = ?,  DTelephoneNo = ?, Specialization = ? WHERE DoctorID = ? ");
	db.stmt->setString(1, DName);
	db.stmt->setString(2, DTelephoneNo);
	db.stmt->setString(3, Specialization);
	db.stmt->setInt(4, DoctorID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Return doctor profile data (using accountID)
void Doctors::getddataa(int AccountID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT * from Doctors WHERE AccountID = ? ");
	db.stmt->setInt(1, AccountID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			DoctorID = db.res->getInt("DoctorID");
			DepartmentID = db.res->getInt("DepartmentID");
			DName = db.res->getString("DName");
			DTelephoneNo = db.res->getString("DTelephoneNo");
			Specialization = db.res->getString("Specialization");

		}
	}
	db.~DatabaseConnector();
}

//Return doctor profile data (using DoctorID)
void Doctors::getddatad(int DoctorID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT * from Doctors WHERE DoctorID = ? ");
	db.stmt->setInt(1, DoctorID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			DoctorID = db.res->getInt("DoctorID");
			DepartmentID = db.res->getInt("DepartmentID");
			DName = db.res->getString("DName");
			DTelephoneNo = db.res->getString("DTelephoneNo");
			Specialization = db.res->getString("Specialization");

		}
	}
	db.~DatabaseConnector();
}

vector<Doctors> Doctors::getdlist(string keyword, string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Doctors` WHERE "	" (DName LIKE ? OR Specialization LIKE ?) "  " ORDER BY " + sortColumn;

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
	db.stmt->setString(2, "%" + keyword + "%");

	vector<Doctors> Doctorsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Doctors tempDoctors(db.res);
			Doctorsl.push_back(tempDoctors);
		}
	}
	if (db.res->rowsCount() <= 0) {
		/*cout << "Empty!";
		_getch();*/
	}

	db.~DatabaseConnector();
	return Doctorsl;
}

void Doctors::removeDoctor(int DoctorID) {
	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Doctors WHERE DoctorID = ?");
	db.stmt->setInt(1, DoctorID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

double Doctors::getbestd() {

	DatabaseConnector db;
	db.prepareStatement(" SELECT Doctors.DoctorID, Doctors.DName, AVG(Feedback.Ratings) AS AverageRating FROM  Doctors JOIN Feedback USING(DoctorID) GROUP BY  Doctors.DoctorID, Doctors.DName ORDER BY AverageRating DESC LIMIT 1 ");
	
	double averageRatings = 0.0;
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			DoctorID = db.res->getInt("DoctorID");
			DName = db.res->getString("DName");
			averageRatings = db.res->getDouble("AverageRating");
			}
	}
	return averageRatings;
	db.~DatabaseConnector();
}

double Doctors::getworstd() {

	DatabaseConnector db;
	db.prepareStatement("  SELECT Doctors.DoctorID, Doctors.DName, AVG(Feedback.Ratings) AS AverageRating FROM  Doctors JOIN Feedback USING(DoctorID) GROUP BY  Doctors.DoctorID, Doctors.DName ORDER BY AverageRating ASC LIMIT 1 ");

	double averageRatings = 0.0;
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			DoctorID = db.res->getInt("DoctorID");
			DName = db.res->getString("DName");
			averageRatings = db.res->getDouble("AverageRating");
		}
	}
	return averageRatings;
	db.~DatabaseConnector();
}

int Doctors::gettotal() {
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


