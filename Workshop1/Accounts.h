#include "DatabaseConnector.h"
#include <chrono>
#include <conio.h>
#include <ctime>
#include <dos.h>
#include <iomanip>
#include <iostream>
#include <mysql/jdbc.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <thread>
#include <time.h>
#include <vector> 
#include <windows.h>

using namespace std;

#ifndef Accounts_H
#define Accounts_H

class Accounts
{
public:
	int ARole, AccountID;
	string Username, Password;

	//Constructor
	Accounts();
	Accounts(sql::ResultSet* data);

	//Insert for account patients
    int insertacc();

	//Update account Username & Password
	void updateacc();
	void updateacca();

	//Login account
	bool loginacc();

	//Return account data
	void getaccdata(int AccountID);

	//Check Username Existence
	bool usernameExisted(string Username); 

	static vector<Accounts> getalist(string keyWord, string sortColumn, bool ascending);

	static void removeAccount(int AccountID);

	static int getTotalAccount();
	static int getTotalRole(int ARole);
};
#endif
