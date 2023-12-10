#include "AppointmentManager.h"
#include "Patients.h"
#include <mysql/jdbc.h>
#include <iostream>
using namespace sql;
using namespace std;


int AppointmentManager::addAppointments(Appointments* appointment, Patients* patient) //part2 49.00
{
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	Connection* connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
	connection->setSchema("Healthcare Appointment Scheduler");


	PreparedStatement* ps = connection->prepareStatement("INSERT INTO APPOINTMENTS (ADate, ATime, AStatus, PatientID, DoctorID) VALUES  ?, ?, ?, ?, ?) ");
	ps->setString(1, appointment->ADate);
	ps->setString(2, appointment->ATime);
	ps->setString(3, appointment->AStatus);// aware
	ps->setInt(4, patient->PatientID);
	ps->setInt(5, appointment->DoctorID);

	int numRowsAffected = ps->executeUpdate();

	PreparedStatement* ps1 = connection->prepareStatement("SELECT * FROM APPOINTMENTS WHERE PatientID = ? ");
	ps->setInt(1, patient->PatientID);
	ResultSet* rs = ps->executeQuery();

	if (rs->next())
	{
		cout << "Hello! " << rs->getString("ADate, ATime, AStatus, PatientID, DoctorID") << endl; //aware
	}

	ps->close();
	delete ps;
	return numRowsAffected;
}