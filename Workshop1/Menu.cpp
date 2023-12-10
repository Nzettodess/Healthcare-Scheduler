#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <Windows.h>
using namespace std;

//Constructor
Menu::Menu() {
	header = "";
	footer = "";
    footer2 =  "";
	bullet = "";

}

//Destructor
Menu::~Menu() {
	options.clear();
}

//menu to prompt user input
int Menu::prompt() {
	int selected = -1;
	do
	{
		system("cls"); // clear your console

        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		cout << header << endl; //cout header as a opening
		for (int i = 0; i < options.size(); i++) {//loop thru each option 
			//display index + bullet(Content)+1st part of vector pair(index)
			cout << i + 1 << bullet << options[i].first; 
			//access index of vector pair then get the values of 2nd part of vector pair
			if (options[i].second != "") {//display if it is not empty
				cout << " : " << options[i].second;
			}
			cout << endl;
		}
        //color
        //SetConsoleTextAttribute(h, 2);
		cout << footer << endl; //cout footer as a ending
        SetConsoleTextAttribute(h, 7);
		selected = _getch() - '0'; // wait for user to input 
		//_getch() is use ascii -'0' is use to convert ascii to normal text
	
		// check user input is valid or not, if not keep looping
	} while (selected < 1 || selected > options.size()); 

	return selected; //if valid proceed
}

// menu prompt but have to enter
/*int Menu::prompt() {
    std::string input = ""; // Create a string to collect input
    char ch;
    int selected = -1;

    do {
        system("cls"); // Clear your console
        cout << header << endl; // Output header as an opening

        for (int i = 0; i < options.size(); i++) {
            // Display the menu options
            cout << i + 1 << bullet << options[i].first;
            if (!options[i].second.empty()) {
                cout << " : " << options[i].second;
            }
            cout << endl;
        }
        cout << footer << endl; // Output footer as an ending

        cout << "Enter your choice: " << input; // Display user input as they type

        ch = _getch();

        if (ch == '\r') { // Enter key
            if (!input.empty()) {
                selected = std::stoi(input); // Convert the collected input to an integer
            }
            break; // Terminate the loop when Enter is pressed
        }
        else if (std::isdigit(ch)) {
            input += ch; // Append the character to the input string
        }
    } while (true);

    if (selected < 1 || selected > options.size()) {
        selected = -1; // Reset selected to -1 if the input is out of range
    }

    return selected; // Return the valid user input
}*/


//Add vector pairs //like stack, push into 
void Menu::addOption(string option) {
	options.push_back({ option,"" });
}

//Check if the index is exist or not, if exist then proceed to add value 
//for the pair
void Menu::setValue(int index, string value) {
	if (index >= 0 && index < options.size()) { 
		options[index].second = value;
	}
}
