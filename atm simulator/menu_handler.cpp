#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <fstream>

#include "menu_handler.h"


using namespace std;
using namespace std::filesystem;

// cout << "\x1B[2K"; // Clear the current line
menu_handler::menu_handler(path current_dir_value) {

	current_dir = current_dir_value;


	while (start_menu() == 0) {
		cout << "Restarting ATM SIMULATOR..." << endl << endl << endl;
	}
}

void menu_handler::clear() {
	cout << "\x1B[2J\x1B[H";
}

int menu_handler::start_menu() {
	// TODO: Generate accounts.csv if it doesn't exist yet!

	cout << "		ATM SIMULATOR       " << endl;
	cout << endl << endl << "    Please select an option" << endl;
	cout << endl << "    (1) - Log in" << endl;
	cout << endl << "    (2) - Exit" << endl;

	string user_input;
	cin >> user_input;

	if (user_input != "1" && user_input != "2") {
		clear();
		cout << "	Invalid input, please select a menu option" << endl;
		return 0;
	}

	switch (stoi(user_input)) {
	case 1:
		// log in

		if(sign_in() == 0) return 0;

		clear();
		return user_menu();
		break;
	case 2:
		clear();
		cout << "	Have a nice day! :)" << endl;
		getchar();
		break;
	}

	return 1;
}

int menu_handler::user_menu(){
	cout << "Successfully logged in as " << active_account->getName() << endl << endl << endl;
	cout << endl << endl << "    Please select an option" << endl << endl;
	cout << endl << "    (1) - Deposit Cash" << endl;
	cout << endl << "    (2) - Withdrawal Cash" << endl;
	cout << endl << "    (3) - Check Balance" << endl; 
	cout << endl << "    (4) - Change Pin" << endl;
	cout << endl << "    (5) - Log Out" << endl;


	string user_input;
	cin >> user_input;
	if (user_input != "1" && user_input != "2" && user_input != "3" && user_input != "4" && user_input != "5") {
		clear();
		cout << "	Invalid input, please select a menu option" << endl;
		return user_menu();
	}
	
	

	if (stoi(user_input) == 1) {
		// deposit
		string dep_amount;
		float dep_amount_value;

		clear();
		cout << "	Enter the amount you would like to deposit: " << endl;
		cin >> dep_amount;
		dep_amount_value = stof(dep_amount);
		while ((dep_amount_value + active_account->getBalance()) > 85000) {
			cout << "	Invalid input. Please ensure your total balance does not exceed our limit of £85,000" << endl;
			cout << "	Enter the amount you would like to deposit: " << endl;
			cin >> dep_amount;
			dep_amount_value = stof(dep_amount);
		}

		active_account->deposit(dep_amount_value, current_dir);
		cout << "	Your new balance is: " << active_account->getBalance() << endl;
	}
	if (stoi(user_input) == 2) {
		// withdrawal
		string with_amount;
		float amount_value;

		clear();
		cout << "	Enter the amount you would like to withdraw: " << endl;
		cin >> with_amount;
		amount_value = stof(with_amount);
		while ((active_account->getBalance() - amount_value) < 0) {
			cout << "	Insufficient funds. Please enter an amount smaller/equal to the amount that you are holding in your bank account" << endl;
			cout << "	Enter the amount you would like to withdraw: " << endl;
			cin >> with_amount;
			amount_value = stof(with_amount);
		}

		active_account->withdraw(amount_value, current_dir);
		cout << "	Your new balance is: " << active_account->getBalance() << endl;
	}
	if (stoi(user_input) == 3){ 
		// check balance

		clear();
		cout << "	Your current balance is: " << active_account->getBalance() << endl;
	}
	if (stoi(user_input) == 4) {
		// change pin
		string postcode;
		string new_pin;
		string new_pin_check = "0";

		while (new_pin != new_pin_check) {
			clear();
			cout << "	To change your pin, please enter your post code (without spaces): " << endl;
			cin >> postcode;
			cout << "	Enter a new 4-digit pin: " << endl;
			cin >> new_pin;
			cout << "	Re-enter new pin: " << endl;
			cin >> new_pin_check;
		}

		while (new_pin != new_pin_check || (active_account->changePin(new_pin, postcode, current_dir) == false) ) {
			clear();
			cout << "	Postcode or new pin is invalid. Please ensure your postcode is written without spaces, and your new pin is numerical and 4 digits" << endl;
			cout << "	To change your pin, please enter your post code (without spaces): " << endl;
			cin >> postcode;
			cout << "	Enter a new 4-digit pin: " << endl;
			cin >> new_pin;
			cout << "	Re-enter new pin: " << endl;
			cin >> new_pin_check;
		}
		clear();
		cout << "	Pin has been updated!" << endl;
	}
	if (stoi(user_input) == 5) {
		// Log out
		clear();
		cout << "	Have a nice day! :)" << endl;
		active_account = nullptr;
		return 0;
	}

	return user_menu();
}

int menu_handler::sign_in() {

	ifstream accounts_file(current_dir / "accounts.csv");
	if (!accounts_file.is_open()) {
		clear();
		cout << "Account file not found! Please restart" << endl;
		return 0;
	}

	string username;
	string pin;

	clear();
	cout << "	Please log in to your account" << endl << endl;
	cout << "	Username: " << endl;
	cin >> username;

	vector<string> rows;
	string line;
	int line_number = 0;
	while (getline(accounts_file, line)) {
		line_number++;
		if (line_number == 1) continue;
		rows.push_back(line);
	}
	accounts_file.close();
	
	// anything from here is user details

	//cout << line_number << ": " << line << endl;

	int account_id = 0;
	for (string row : rows) {
		account_id++;
		stringstream line_stream(row);
		string segment;
		vector<string> account_info;

		while (getline(line_stream, segment, ',')) {
			account_info.push_back(segment);
		}
		string this_username = account_info[9];
		if (username == this_username) {

			cout << "	Pin: " << endl;
			cin >> pin;

			// check pin is the same for this_username					
			string this_pin = account_info[10];
			if (pin != this_pin) {
				cout << "\x1B[F";
				cout << "\x1B[F";
				cout << "\x1B[2K";
				cout << "\x1B[2K";
				cout << "	Please try again." << endl;
				cout << "	Pin: " << endl;
				cin >> pin;
			}

			if (pin == this_pin) {
				cout << "	Logging in..." << endl << endl;

				string first_name = account_info[0];
				string last_name = account_info[1];
				string DOB = account_info[2];
				string adline_1 = account_info[3];
				string adline_2 = account_info[4];
				string city = account_info[5];
				string county = account_info[6];
				string postcode = account_info[7];
				float balance = stof(account_info[8]);

				active_account = new account(first_name, last_name, DOB, adline_1, adline_2, city, county, postcode, balance, this_username, this_pin, account_id);

				return 1;
			}

			if (pin != this_pin) {
				clear();
				cout << "	You have exceeded the number of allowed login attempts." << endl << endl;
				return 0;
			}
		}
	}

	cout << "\x1B[F";
	cout << "\x1B[F";
	cout << "\x1B[2K";
	cout << "\x1B[2K";
	cout << "	This username does not exist, please try again." << endl;

	string response;
	while (response != "0" && response != "1") {
		cout << "\x1B[2K";
		cout << "	Press 0 to return to menu, or 1 to try again." << endl;
		cin >> response;
		cout << "\x1B[F";
		cout << "\x1B[F";
		cout << "\x1B[2K";
		cout << "\x1B[2K";

		if (response == "0") {
			clear();
			return 0;
		}
		if (response == "1") {
			clear();
			return sign_in();
		}
		else cout << "	Invalid response." << endl;
	}



	//


	return 1;
}

/*
	bool menu_handler::is_date_val(string DOB) {
	if (DOB.length() != 10 || DOB.empty()) return false;
	if (DOB[2] != '/') return false;
	if (DOB[5] != '/') return false;

	stringstream DOBStream(DOB);
	string segment;
	vector<string> DOBList;

	while (getline(DOBStream, segment, '/')) {
		DOBList.push_back(segment);
	}

	cout << DOBList.size() << endl;
	int day = stoi(DOBList[0]);
	int month = stoi(DOBList[1]);
	int year = stoi(DOBList[2]);

	time_t current_time = time(0);
	struct tm local_time;
	localtime_s(&local_time, &current_time);

	cout << "current year: " << local_time.tm_year;
	*/


	//today - 18 years = max birthday 

//



	// "01/01/1999"
	// int year = 1999
	// int month = 1
	// int day = 1

	// 1) Seperate them by the slashes
	//		["01","01","1999"]
	// array[0] = day
	// use stoi to turn to int



//	return false;
//}

/*int menu_handler::create_account() {
	string firstName;
	string lastName;
	string DOB;
	string adline1;
	string adline2;
	string city;
	string county;
	string postcode;

	cout << "	Please input the following credentials: " << endl;
	cout << "		Fields marked by (*) are required" << endl << endl;
	cout << "	First Name*: " << endl;
	cin >> firstName;
	cout << "	Last Name*: " << endl;
	cin >> lastName;

	while (is_date_val(DOB) == false) {
		cout << "	Date of Birth*: " << endl;
		cout << "		Please use the format dd/mm/yyyy" << endl;
		cin >> DOB;
	}

	cout << "	Address Line 1*: " << endl;
	cin >> adline1;
	cout << "	Address Line 2: " << endl;
	cin >> adline2;
	cout << "	City*: " << endl;
	cin >> city;
	cout << "	County: " << endl;
	cin >> county;
	cout << "	Postcode*: " << endl;
	cout << "		Do not include spaces, e.g. NG14FQ" << endl;
	cin >> postcode;



	return 1;
	*/

	/*
	Please input the following credentials:
		Fields marked by (*) are required

	First Name*:
	Last Name*:
	Date of Birth*:
	Address Line 1*:
	Address Line 2:
	City*:
	County:
	Postcode*:
	*/


/*
- address line 1
- address line 2
- city
- county
- postcode

- dob
- username
- pin*/