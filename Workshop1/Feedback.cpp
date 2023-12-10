#include "Feedback.h"
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
#include <iostream>


//Constructor
Feedback::Feedback() {
	FeedbackID = 0;
	Ratings = 0;
	PatientID = 0;
	DoctorID = 0;
	Comments = "";
	FDate = "";
}

Feedback::Feedback(sql::ResultSet* data) {

	FeedbackID = data->getInt("FeedbackID");
	Ratings = data->getInt("Ratings");
	PatientID = data->getInt("PatientID");
	DoctorID = data->getInt("DoctorID");
	Comments = data->getString("Comments");
	FDate = data->getString("FDate");
}

//Insert feedback
void Feedback::insertf() {

	DatabaseConnector db;
	db.prepareStatement("Insert into Feedback (Ratings, Comments, FDate, PatientID, DoctorID) VALUES (?,?,?,?,?)");
	db.stmt->setInt(1, Ratings);
	db.stmt->setString(2, Comments);
	db.stmt->setString(3, FDate);
	db.stmt->setInt(4, PatientID);
	db.stmt->setInt(5, DoctorID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Update feedback
void Feedback::updatef(int FeedbackID) {

	DatabaseConnector db;
	db.prepareStatement("Update Feedback set Ratings = ?,  Comments = ?, FDate = ?, DoctorID = ?, PatientID = ? WHERE FeedbackID = ? ");
	db.stmt->setInt(1, Ratings);
	db.stmt->setString(2, Comments);
	db.stmt->setString(3, FDate);
	db.stmt->setInt(4, DoctorID);
	db.stmt->setInt(5, PatientID);
	db.stmt->setInt(6, FeedbackID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Return feedback data(FeedbackID) //For machine read data
void Feedback::getfdataf(int FeedbacktID) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Feedback WHERE FeedbackID = ? ");
	db.stmt->setInt(1, FeedbackID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			FeedbackID = db.res->getInt("FeedbackID");//int64
			DoctorID = db.res->getInt("DoctorID");
			PatientID = db.res->getInt("PatientID");
			Ratings = db.res->getInt("Ratings");
			Comments = db.res->getString("Comments");
			FDate = db.res->getString("FDate");
		}
	}
	db.~DatabaseConnector();
}

//Return feedback history for patient
vector<Feedback> Feedback::getflistp(int PatientID, string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Feedback` WHERE PatientID  =  ? " " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, PatientID);

	vector<Feedback> Feedbackl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Feedback tempFeedback(db.res);
			Feedbackl.push_back(tempFeedback);
		}
	}
	else {
		cout << "You dont have any feedback in history!";
		_getch();
	}
	db.~DatabaseConnector();
	return Feedbackl;
}

//Return feedback history for patient
vector<Feedback> Feedback::getflistd(int DoctorID, string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Feedback` WHERE DoctorID  =  ? " " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setInt(1, DoctorID);

	vector<Feedback> Feedbackl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Feedback tempFeedback(db.res);
			Feedbackl.push_back(tempFeedback);
		}
	}
	else {
		cout << "You dont have any feedback yet!";
		_getch();
	}
	db.~DatabaseConnector();
	return Feedbackl;
}

void Feedback::removeFeedback(int FeedbackID) {
	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Feedback WHERE FeedbackID = ?");
	db.stmt->setInt(1, FeedbackID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

vector<Feedback> Feedback::getflistall(string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Feedback` " " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);


	vector<Feedback> Feedbackl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Feedback tempFeedback(db.res);
			Feedbackl.push_back(tempFeedback);
		}
	}
	else {
		cout << "You dont have any feedback yet!";
		_getch();
	}
	db.~DatabaseConnector();
	return Feedbackl;
}