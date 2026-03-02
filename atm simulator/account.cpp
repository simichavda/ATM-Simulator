#include "account.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::filesystem;


account::account(string firstName_value, string lastName_value, string DOB_value, string adline1_value, string adline2_value, string city_value, string county_value, string postcode_value, float balance_value, string username_value, string pin_value, int id_value){
	first_name = firstName_value;
	last_name = lastName_value;
	DOB = DOB_value;
	adline1 = adline1_value;
	adline2 = adline2_value;
	city = city_value;
	county = county_value;
	postcode = postcode_value;
	balance = balance_value;
	username = username_value;
	pin = pin_value;
	id = id_value;


	// cout << "I've just been made yay! :)";

}

string account::getName() {
	return first_name + " " + last_name;

}

float account::deposit(float amount, path dir_to_folder) {
	float old_balance = balance;
	balance = balance + amount;
	if (!save(dir_to_folder)){
		cout << "Error when updating account balance" << endl;
		balance = old_balance;
	}
	return balance;
}

float account::withdraw(float amount, path dir_to_folder) {
	float old_balance = balance;
	balance = balance - amount;
	if (!save(dir_to_folder)) {
		cout << "Error when updating account balance" << endl;
		balance = old_balance;
	}
	return balance;
}

float account::getBalance(){
	return balance;
}

bool account::changePin(string new_pin, string postcode_value, path path_to_accounts) {
	string postcode_value_lowercase = postcode_value;
	transform(postcode_value_lowercase.begin(), postcode_value_lowercase.end(), postcode_value_lowercase.begin(), ::tolower);

	string postcode_lowercase = postcode;
	transform(postcode_lowercase.begin(), postcode_lowercase.end(), postcode_lowercase.begin(), ::tolower);

	string old_pin = pin;

	if (postcode_lowercase != postcode_value_lowercase) {
		return false;
	}
	if (new_pin.length() != 4) {
		return false;
	}
	for (char c : new_pin) {
		if (isdigit(c) == false) {
			return false;
		}
	}
	pin = new_pin;

	bool didSave = save(path_to_accounts);
	if (didSave == false) {
		pin = old_pin;
		return false;
	}

	return true;	
}

bool account::save(path dir_to_folder){
	ifstream file_in(dir_to_folder / "accounts.csv");
	if (!file_in.is_open()) {
		cout << "Account file not found! Please restart" << endl;
		return false;
	}

	vector<string> newLines; // the lines to save afterwards
	
	string line;
	int line_number = 0;
	while (getline(file_in, line)) {
		line_number++;
		if(line_number != id + 1){
			newLines.push_back(line);
			continue;
		}

		newLines.push_back(first_name + "," + last_name + "," + DOB + "," + adline1 + "," + adline2 + "," + city + "," + county + "," + postcode + "," + to_string(balance) + "," + username + "," + pin);
	}
	file_in.close();

	ofstream file_out(dir_to_folder / "accounts.csv");
	if(!file_out.is_open()){
		return false;
	}
	
	for(string newLine : newLines){
		file_out << newLine << endl;
	}
	file_out.close();

	return true;

}