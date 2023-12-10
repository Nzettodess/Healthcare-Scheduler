#include <string>
#include <ctime>
#include <conio.h>
#include <vector> 
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
using namespace std;

#ifndef Feedback_H
#define Feedback_H

class Feedback
{
public:
	int FeedbackID, Ratings, PatientID, DoctorID;
	string Comments, FDate;

	//Constructor
	Feedback();
	Feedback(sql::ResultSet* data);

	//Insert feedback
	void insertf();

	//Update feedback
	void updatef(int FeedbackID);

	void getfdataf(int FeedbacktID);

	static vector<Feedback> getflistp(int PatientID, string sortColumn, bool ascending);
	static vector<Feedback> getflistd(int DoctorID, string sortColumn, bool ascending);

	static void removeFeedback(int FeedbackID);

	static vector<Feedback> getflistall(string sortColumn, bool ascending);
};
#endif
