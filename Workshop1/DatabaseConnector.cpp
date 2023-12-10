#include "DatabaseConnector.h"
#include <conio.h>
using namespace std;
using namespace sql;

//Constuctor
DatabaseConnector::DatabaseConnector() {
	try {
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
		connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
		connection->setSchema("Healthcare Appointment Scheduler");
		stmt = nullptr;
		res = nullptr;
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 0) {
			system("cls");
			cout << "Unable to connect to database";
			_getch();
			exit(0);
			return;
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

//Destructor
DatabaseConnector::~DatabaseConnector() {
	try {
		if (connection) {
			connection->close();
		}
		if (stmt) {
			stmt->close();
		}
		if (res) {
			res->close();
		}
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() != 0) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			_getch();
		}
	}
}

void DatabaseConnector::prepareStatement(string query) {
	stmt = connection->prepareStatement(query);
}

void DatabaseConnector::QueryStatement() {

	try {
		stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

void DatabaseConnector::QueryResult() {

	try {
		res = stmt->executeQuery();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

int DatabaseConnector::getGeneratedId() {
	prepareStatement("SELECT LAST_INSERT_ID();");
	QueryResult();
	int lastInsertId = -1;
	if (res->rowsCount() > 0) {
		while (res->next()) {
			lastInsertId = res->getInt64("LAST_INSERT_ID()");
		}
	}
	return lastInsertId;
}
