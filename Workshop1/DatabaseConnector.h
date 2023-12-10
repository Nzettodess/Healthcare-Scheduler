#pragma once

#ifndef DatabaseConnector_H
#define DatabaseConnector_H

#include <mysql/jdbc.h>
#include <string>

class DatabaseConnector
{

public:
	// Constructor
	DatabaseConnector(); 

	// Destructor
	~DatabaseConnector(); 

	//PrepareStatement
	void prepareStatement(std::string query);

	//A pointer to store statement
	sql::PreparedStatement* stmt; 

	//A pointer to store the result set
	sql::ResultSet* res; 

	// For Insert, Update, Delete query = NO RESULTS
	void QueryStatement(); 

	// For Select query = RESULTS
	void QueryResult(); 

	int getGeneratedId();

private:
	sql::Connection* connection;
};

#endif 