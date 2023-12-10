#include "Departments.h"
#include "DatabaseConnector.h"
#include <mysql/jdbc.h>
#include <iostream>

//Constructor
Departments::Departments() {
	DepartmentID = 0;
	DptName = "";
	Description = "";
}

Departments::Departments(sql::ResultSet* data) {
	DepartmentID = data->getInt("DepartmentID");
	DptName = data->getString("DptName");
	Description = data->getString("Description");
}

//Insert for departments detail 
void Departments::insertdpt() {

	DatabaseConnector db;
	db.prepareStatement("Insert into Departments (DptName, Description) VALUES (?,?)");
	db.stmt->setString(1, DptName);
	db.stmt->setString(2, Description);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Update for departments detail 
void Departments::updatedpt() {

	DatabaseConnector db;
	db.prepareStatement("Update Departments set DptName = ?,  Description = ? WHERE DepartmentID = ? ");
	db.stmt->setString(1, DptName);
	db.stmt->setString(2, Description);
	db.stmt->setString(3, to_string(DepartmentID));
	db.QueryStatement();
	db.~DatabaseConnector();

}

//Remove departments
void Departments::removeDpt() {

	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Departments WHERE DepartmentID=?");
	db.stmt->setInt(1, DepartmentID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

//Query department
int Departments::dptidquery(string DptName) {
	try {
		int DepartmentID = -1;
		DatabaseConnector db;
		db.prepareStatement("SELECT DepartmentID FROM Departments WHERE DptName = ? ");
		//db.stmt->setString(1,  keyword);
		db.stmt->setString(1, DptName);

		db.QueryResult();
		if (db.res->rowsCount() > 0) {
			while (db.res->next()) {
				DepartmentID = db.res->getInt("DepartmentID");
			}
		}
		//db.~DatabaseConnector();
		return DepartmentID;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	}

vector<Departments>Departments::getdptlist(string keyword, string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Departments` WHERE "	" (DptName LIKE ? OR Description LIKE ?) "  " ORDER BY " + sortColumn;

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

	vector<Departments> Departmentsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Departments tempDepartments(db.res);
			Departmentsl.push_back(tempDepartments);

		}
	}
	else {
		cout << "Empty!";
		_getch();
	}


	db.~DatabaseConnector();
	return Departmentsl;
}

void Departments::removeDpt(int DepartmentID) {
	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Departments WHERE DepartmentID = ?");
	db.stmt->setInt(1, DepartmentID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

void Departments::getdptdatadpt(int DepartmentID) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Departments WHERE DepartmentID = ? ");
	db.stmt->setInt(1, DepartmentID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			DepartmentID = db.res->getInt("DepartmentID");
			DptName = db.res->getString("DptName");
			Description = db.res->getString("Description");

		}
	}
	db.~DatabaseConnector();
}