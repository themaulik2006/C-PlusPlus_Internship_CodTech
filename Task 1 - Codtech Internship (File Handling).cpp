#include <iostream>
#include <fstream>//universal header file for file handling purposes
#include <string>
using namespace std;

int main() {
	fstream my_file;//to access a file in all modes
	string line;
	int choice;

	do {
		cout << "\n===== FILE HANDLING MENU =====\n";
		cout << "1. Write to File (overwrite)\n";
		cout << "2. Append to File\n";
		cout << "3. Read from File\n";
		cout << "4. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1) {
			// WRITE MODE
			my_file.open("intern.txt", ios::out);//opens the file in write mode
			if (!my_file)//if file can't be accessed
			{
				cout << "Unable to open the file for writing!" << endl;
				continue;//we simply continue to take the next choice as input
			}

			cout << "\n--- WRITE MODE ---\n";
			do
			{
				cout << "Enter a line to write (type 'exit' or 'stop' to finish): ";
				getline(cin, line);
				if (line != "exit" && line != "stop")//reads inputs until 'exit' or 'stop' is sent by user
				{
					my_file << line << endl;
				}
			} while (line != "exit" && line != "stop");

			my_file.close();//closes file after writing in it
		}

		else if (choice == 2)
		{
			// APPEND MODE
			my_file.open("intern.txt", ios::app);//opens the file in append mode
			if (!my_file)//if file can't be accessed
			{
				cout << "Unable to open the file for appending!" << endl;
				continue;//we simply continue to take the next choice as input
			}

			cout << "\n--- APPEND MODE ---\n";
			do
			{
				cout << "Enter a line to append (type 'exit' or 'stop' to finish): ";
				getline(cin, line);
				if (line != "exit" && line != "stop")//reads inputs until 'exit' or 'stop' is sent by user
				{
					my_file << line << endl;
				}
			} while (line != "exit" && line != "stop");

			my_file.close();//closes file after appending is done
		}

		else if (choice == 3)
		{
			// READ MODE
			my_file.open("intern.txt", ios::in);//opens the file in read mode
			if (!my_file)//if file can't be accessed
			{
				cout << "Unable to open the file for reading!" << endl;
				continue;//we simply continue to take the next choice as input
			}

			cout << "\n--- FILE CONTENTS ---\n";

			while (!my_file.eof())//accesses till the END OF THE FILE
			{
				getline(my_file, line);//copies each line from the file into the string 'line'
				cout << line << endl;//prints each copied line
			}

			my_file.close();//closes file after reading it
		}

		else if (choice == 4)
		{
			cout << "Exiting program. Goodbye!" << endl;
		}

		else//when the choice is not between 1-4
		{
			cout << "Invalid choice! Please select from the menu.\n";
		}

	} while (choice != 4);

	return 0;
}
