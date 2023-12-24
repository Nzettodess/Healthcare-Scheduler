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
	// Get the account role from the database
	ARole = data->getInt("ARole");
	// Get the username from the database
	Username = data->getString("Username");
	// Get the password from the database
	Password = data->getString("Password");
	// Get the account ID from the database
	AccountID = data->getInt("AccountID");
}

//Insert for account (create account) (integrate with generatedID)
int Accounts::insertacc() {

	//Create a DatabaseConnector object
	DatabaseConnector db;
	//Prepare an insert statement
	db.prepareStatement("Insert into Accounts (Username, Password, ARole) VALUES (?,?,?)");
	//Set the values for the statement
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, ARole);
	//Execute the query
	db.QueryStatement();

	//Get the last inserted ID
	db.prepareStatement("SELECT LAST_INSERT_ID();");
	db.QueryResult();
	int lastInsertId = -1;
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			lastInsertId = db.res->getInt("LAST_INSERT_ID()"); //int64
		}
	}
	//Return the last inserted ID
	return lastInsertId;
	//Destroy the DatabaseConnector object
	db.~DatabaseConnector();
}

//Update account Username & Password 
void Accounts::updateacc() {

	//Create a DatabaseConnector object
	DatabaseConnector db;
	//Prepare a statement to update the Accounts table
	db.prepareStatement("Update Accounts set Username = ?, Password = ? WHERE AccountID = ? ");
	//Set the parameters for the statement
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, AccountID);
	//Execute the query
	db.QueryStatement();
	//Destroy the DatabaseConnector object
	db.~DatabaseConnector();

	//UPDATE ACCOUNTS	SET `Password` = '456' WHERE `AccountID` = 'admin';
	//
}
//Update account Username, Password & Account Role 
void Accounts::updateacca() {

	//Create a DatabaseConnector object
	DatabaseConnector db;
	//Prepare a statement to update the Accounts table
	db.prepareStatement("Update Accounts set Username = ?, Password = ?, ARole = ? WHERE AccountID = ? ");
	//Set the parameters for the statement
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	db.stmt->setInt(3, ARole);
	db.stmt->setInt(4, AccountID);
	//Execute the query
	db.QueryStatement();
	//Destroy the DatabaseConnector object
	db.~DatabaseConnector();

	
}
//Login account 
bool Accounts::loginacc() {

	//Create a database connector object
	DatabaseConnector db;
	//Prepare a statement to select all data from the Accounts table where the username and password match the given username and password
	db.prepareStatement("SELECT * FROM Accounts WHERE Username = ? AND Password = ? ");
	//Set the username and password to the values given by the user
	db.stmt->setString(1, Username);
	db.stmt->setString(2, Password);
	//Execute the query
	db.QueryResult();

	//Check if the query returned a result
	if (db.res->rowsCount() == 1)
	{
		//Loop through the result set and store the data in the AccountID, Username, Password and ARole variables
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");
			Username = db.res->getString("Username");
			Password = db.res->getString("Password");
			ARole = db.res->getInt("ARole");

		}
		//cout << "Hello!" << res->getString("AccountID") << endl;
		//Delete the database connector object
		db.~DatabaseConnector();
		//Return true if the query returned a result
		return true;
	}
	//Delete the database connector object
	else {
		db.~DatabaseConnector();
		//Return false if the query did not return a result
		return false;
	}
		
	
};

//Return account data
void Accounts::getaccdata(int AccountID) {

	//Create a database connector object
	DatabaseConnector db;
	//Prepare a statement to select all data from the Accounts table where the AccountID matches the given AccountID
	db.prepareStatement("SELECT * from Accounts WHERE AccountID = ? ");
	//Set the AccountID to the value given by the user
	db.stmt->setInt(1, AccountID);

	//Execute the query
	db.QueryResult();

	//Check if the query returned a result
	if (db.res->rowsCount() > 0) {
		//Loop through the result set and store the data in the AccountID, Username, Password and ARole variables
		while (db.res->next()) {
			AccountID = db.res->getInt("AccountID");//int64
			Username = db.res->getString("Username");
			Password = db.res->getString("Password");
			ARole = db.res->getInt("ARole");
		}
	}
	//Delete the database connector object
	db.~DatabaseConnector();
}

//Check if the username already exists in the database
bool Accounts::usernameExisted(string Username) {
	//Create a database connector object
	DatabaseConnector db;
	//Prepare a statement to select all data from the Accounts table where the username matches the given username
	db.prepareStatement("SELECT * from Accounts WHERE Username = ? ");
	//Set the username to the value given by the user
	db.stmt->setString(1, Username);

	//Execute the query
	db.QueryResult();

	//Check if the query returned a result
	if (db.res->rowsCount() > 0) 
	{
		//Return true if the query returned a result
		return true;
		//return (db.res->rowsCount() > 0);

	}
	//Return false if the query did not return a result
	else if (db.res->rowsCount() <= 0)
	{
		return false;
	}
	//Delete the database connector object
	db.~DatabaseConnector();

}

//Return a list of accounts based on the given parameters
vector<Accounts> Accounts::getalist(string KeyWord, string sortColumn, bool ascending) {

	//Create a query string to select all data from the Accounts table where the username matches the given KeyWord and sort the data by the given sortColumn
	string query = "SELECT * FROM `Accounts` WHERE "" (Username LIKE ?) "  " ORDER BY " + sortColumn;

	//Check if the ascending parameter is true
	if (ascending) {
		//Add the ASC keyword to the query string
		query += " ASC";
	}
	else {
		//Add the DESC keyword to the query string
		query += " DESC";
	}

	//Create a database connector object
	DatabaseConnector db;
	//Prepare a statement to execute the query
	db.prepareStatement(query);
	//Set the username to the value given by the user
	db.stmt->setString(1, "%" + KeyWord + "%");

		//Create a vector to store the results
	vector<Accounts> Accountsl;

	//Execute the query
	db.QueryResult();

	//Check if the query returned a result
	if (db.res->rowsCount() > 0) {

		//Loop through the result set and store the data in the Accountsl vector
		while (db.res->next()) {
			Accounts tempAccounts(db.res);
			Accountsl.push_back(tempAccounts);
		}
	}

	//Check if the query did not return a result
	if (db.res->rowsCount() <= 0) {
		cout << "Empty!";
		_getch();
	}

	//Delete the database connector object
	db.~DatabaseConnector();
	//Return the vector containing the results
	return Accountsl;
}
void Accounts::removeAccount(int AccountID) {
	//create an instance of DatabaseConnector
	DatabaseConnector db;
	//prepare a statement to delete an account from the Accounts table
	db.prepareStatement("DELETE FROM Accounts WHERE AccountID = ?");
	//set the AccountID to the parameter
	db.stmt->setInt(1, AccountID);
	//execute the query
	db.QueryStatement();
	//delete the instance of DatabaseConnector
	db.~DatabaseConnector();
}

//method to get the total number of accounts
int Accounts::getTotalAccount() {
	//create an instance of DatabaseConnector
	DatabaseConnector db;
	//initialize the total account to -1
	int totalAcc = -1;
	//prepare a statement to select the total number of accounts
	db.prepareStatement("Select COUNT(AccountID) AS totalacc FROM Accounts");
	//execute the query
	db.QueryResult();

	//check if the result is valid
	if (db.res->rowsCount() > 0) {
		//loop through the result set
		while (db.res->next()) {
			//set the total account to the value of the result set
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	//return the total account
	return totalAcc;
	//delete the instance of DatabaseConnector
	db.~DatabaseConnector();

	/*Method to use:
	int test;
	test = Accounts::getTotalAccount();
	cout << test;*/
}

//method to get the total number of roles
int Accounts::getTotalRole(int ARole) {
	DatabaseConnector db;
	int totalAcc = -1;
	//prepare statement to select total number of accounts
	db.prepareStatement("Select COUNT(AccountID) AS totalacc FROM Accounts WHERE ARole = ?");
	//set the parameter to the ARole value
	db.stmt->setInt(1, ARole);
	//execute the query
	db.QueryResult();

	//check if the query was successful
	if (db.res->rowsCount() > 0) {
		//if the query was successful, loop through the results
		while (db.res->next()) {
			//set the totalAcc variable to the value of the totalacc column
			totalAcc = db.res->getInt("totalacc");//int64
		}
	}
	//return the totalAcc variable
	return totalAcc;
	//destroy the DatabaseConnector object
	db.~DatabaseConnector();

	/*Method to use:
	int test;
	test = Accounts::getTotalAccount();
	cout << test;*/
}

std::vector<int> Accounts::getTotalRoleCounts() {
	DatabaseConnector db;

	// Assuming you have a list of roles (modify this based on your actual roles)
	std::vector<int> roles = { 1, 2, 3, 4 };  // Replace with your role values

	std::vector<int> roleCounts;

	for (int role : roles) {
		int totalAcc = -1;

		// Prepare statement to select total number of accounts for each role
		db.prepareStatement("SELECT COUNT(AccountID) AS totalacc FROM Accounts WHERE ARole = ?");

		// Set the parameter to the current role value
		db.stmt->setInt(1, role);

		// Execute the query
		db.QueryResult();

		// Check if the query was successful
		if (db.res->rowsCount() > 0) {
			// If the query was successful, loop through the results
			while (db.res->next()) {
				// Set the totalAcc variable to the value of the totalacc column
				totalAcc = db.res->getInt("totalacc");
			}
		}

		// Add the total count for the current role to the vector
		roleCounts.push_back(totalAcc);
	}

	// Destroy the DatabaseConnector object
	db.~DatabaseConnector();

	// Return the vector containing total counts for each role
	return roleCounts;
}
