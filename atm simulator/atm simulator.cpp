#include <iostream>
#include <filesystem>
#include "menu_handler.h"
using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
    path executable_path(argv[0]); // Get the path of the executable
	path current_dir = executable_path.parent_path(); // Get the parent directory of the executable

    new menu_handler(current_dir);
}

/*
        ATM SIMULATOR       

    Please select an option

    (1) - Create an account
    (2) - Log in
    (3) - Exit
*/




/*
Plan:

Account:
- first name
- last name

- address line 1
- address line 2
- city
- county
- postcode

- dob
- username
- pin
- 





*/


/*TO DO:
    
    finish create account
    finish balance
    deposit
    withdraw
    signin

*/