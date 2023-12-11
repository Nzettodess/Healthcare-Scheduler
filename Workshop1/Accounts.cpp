#include "Accounts.h"
#include "DatabaseConnector.h"
#include <iostream>
#include <mysql/jdbc.h>

//Constructor
Accounts::Accounts() {
	//Initialize variable
	AccountID = 0;
	Username = "";
	Password = "";
	ARole = 0;
	}
Accounts::Accounts(sql::ResultSet* data) {
	ARole = data->getInt("ARole");
	Username = data->getString("Username");
	Password = data->getString("Password");
	AccountID = data->getInt("AccountID");
}

//Insert for account (create account) (integrate with generatedID)
int Accounts::insertacc() {

	DatabaseConnector db;
	db.prepareStatement("Insert into Accounts (Username, Password, ARole) VALUES (?,?,?)");
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, ARole);
	db.QueryStatement();

	db.prepareStatement("SELECT LAST_INSERT_ID();");
	db.QueryResult();
	int lastInsertId = -1;
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			lastInsertId = db.res->getInt("LAST_INSERT_ID()"); //int64
		}
	}
	return lastInsertId;
	db.~DatabaseConnector();
}

//Update account Username & Password 
void Accounts::updateacc() {

	DatabaseConnector db;
	db.prepareStatement("Update Accounts set Username = ?, Password = ? WHERE AccountID = ? ");
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, AccountID);
	db.QueryStatement();
	db.~DatabaseConnector();

	//UPDATE ACCOUNTS	SET `Password` = '456' WHERE `AccountID` = 'admin';
	//
}
void Accounts::updateacca() {

	DatabaseConnector db;
	db.prepareStatement("Update Accounts set Username = ?, Password = ?, ARole = ? WHERE AccountID = ? ");
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, ARole);
	db.stmt->setInt(4, AccountID);
	db.QueryStatement();
	db.~DatabaseConnector();

	
}

//Login account 
bool Accounts::loginacc() {

	DatabaseConnector db;
	db.prepareStatement("SELECT * FROM Accounts WHERE Username = ? AND Password = ? ");
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.QueryResult();

	if (db.res->rowsCount() == 1)
	{
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			Username = db.res->getString("Username");
			Password = db.res->getString("Password");
			ARole = db.res->getInt("ARole");

		}
		//cout << "Hello!" << res->getString("AccountID") << endl;
		db.~DatabaseConnector();
		return true;
	}
	else {
		db.~DatabaseConnector();
		return false;
	}
		
	
};

//Return account data
void Accounts::getaccdata(int AccountID) {

	DatabaseConnector db;
	db.prepareStatement("SELECT * from Accounts WHERE AccountID = ? ");
	db.stmt->setInt(1, AccountID);

	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");//int64
			Username = db.res->getString("Username");
			Password = db.res->getString("Password");
			ARole = db.res->getInt("ARole");
		}
	}
	db.~DatabaseConnector();
}

bool Accounts::usernameExisted(string Username) {
	DatabaseConnector db;
	db.prepareStatement("SELECT * from Accounts WHERE Username = ? ");
	db.stmt->setString(1, Username);

	db.QueryResult();

	if (db.res->rowsCount() > 0) 
	{
		return true;
		//return (db.res->rowsCount() > 0);

	}
	else if (db.res->rowsCount() <= 0)
	{
		return false;
	}
	db.~DatabaseConnector();

}

vector<Accounts> Accounts::getalist(string KeyWord, string sortColumn, bool ascending) {

	string query = "SELECT * FROM `Accounts` WHERE "" (Username LIKE ?) "  " ORDER BY " + sortColumn;

	if (ascending) {
		query += " ASC";
	}
	else {
		query += " DESC";
	}

	DatabaseConnector db;
	db.prepareStatement(query);
	db.stmt->setString(1, "%" + KeyWord + "%");

		vector<Accounts> Accountsl;

	db.QueryResult();

	if (db.res->rowsCount() > 0) {

		while (db.res->next()) {
			Accounts tempAccounts(db.res);
			Accountsl.push_back(tempAccounts);
		}
	}

	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}

	db.~DatabaseConnector();
	return Accountsl;
}

void Accounts::removeAccount(int AccountID) {
	DatabaseConnector db;
	db.prepareStatement("DELETE FROM Accounts WHERE AccountID = ?");
	db.stmt->setInt(1, AccountID);
	db.QueryStatement();
	db.~DatabaseConnector();
}

int Accounts::getTotalAccount() {
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("Select COUNT(AccountID) AS totalacc FROM Accounts");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();

	/*Method to use:
	int test;
	test = Accounts::getTotalAccount();
	cout << test;*/
}

int Accounts::getTotalRole(int ARole) {
	DatabaseConnector db;
	int totalAcc = -1;
	db.prepareStatement("Select COUNT(AccountID) AS totalacc FROM Accounts WHERE ARole = ?");
	db.stmt->setInt(1, ARole);
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	return totalAcc;
	db.~DatabaseConnector();

	/*Method to use:
	int test;
	test = Accounts::getTotalAccount();
	cout << test;*/
}