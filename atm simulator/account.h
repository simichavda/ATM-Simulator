#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

class account
{
public:
	// constructor called for each account object when created
	account(string firstName_value, string lastName_value, string DOB_value, string adline1_value, string adline2_value, string city_value, string county_value, string postcode_value, float balance_value, string username_value, string pin_value, int id_value);

	float getBalance();
	float withdraw(float amount, path dir_to_folder);
	float deposit(float amount, path dir_to_folder);

	string getName();
	bool changePin(string new_pin, string postcode_value, path dir_to_folder);


private:
	bool save(path path_to_accounts);

	float balance = 0;
	string first_name;
	string last_name;
	string DOB;
	string adline1;
	string adline2;
	string city;
	string county;
	string postcode;
	string username;
	string pin;

	int id;


};

