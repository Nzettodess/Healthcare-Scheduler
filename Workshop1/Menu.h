#pragma once
#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <utility>

class Menu
{
public:
	
	std::string header, footer,  footer2, bullet;

	//Constructor
	Menu();

	//Destructor
	~Menu();

	//menu to prompt user input
	int prompt();

	//Add vector pairs //like stack, push into 
	void addOption(std::string option);

	//Check if the index is exist or not, if exist then proceed to add 
	//value for the pair
	void setValue(int index, std::string value);

private:

	// Vector [pair of strings]
	std::vector<std::pair<std::string, std::string>> options;
	//Example of options data [ {"1","2"}  ,{"1","2"}  ] 

};


#endif