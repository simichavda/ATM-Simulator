#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include "account.h"

using namespace std;
using namespace std::filesystem;

#pragma once
class menu_handler
{
public:
	menu_handler(path current_dir_value);


private:
	path current_dir;
	account* active_account;

	int start_menu();

	int create_account();
	int sign_in();
	int user_menu();

	void clear();
	bool is_date_val(string DOB);

};

