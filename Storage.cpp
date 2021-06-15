#include "Storage.h"
#include "Util.h"
#include "Calculation.h"
#include <iostream>
#include <limits>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

/*
	Performs variable storage, and manipulation
	Rundown:
	Line - Name			: Description
	20	 - doesVarExist : Checks if the is already assigned
	25	 - BEG			: Performs BEG command (get variable name and store a value)
	36	 - PRINT		: Performs PRINT command (get variable name or literal, then display the value)
	43	 - ASSIGN		: Performs assignment (assign value to variable, check if variable names exist in the hash table, and solve if expression)
*/

bool Storage::doesVarExist(string command) {
	if (variables.find(command) == variables.end()) return 0;	// Variable not assigned yet
	else return 1;
}

void Storage::BEG(string command) {
	string temp = command.erase(0, 4);	// Remove "BEG "
	string store;
	cout << "SNOL> Please enter value for [" << temp << "]: ";
	cin >> store;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (isDigit(store)) variables[temp] = store;
	else cout << "SNOL> Error! Value of [" << temp << "] can only store integer or float values." << endl;
}

void Storage::PRINT(string command) {
	string temp = command.erase(0, 6);	// Remove "PRINT  "
	if (doesVarExist(temp)) cout << "SNOL> [" << temp << "] = " << variables.at(temp) << endl;
	else if(isDigit(temp)) cout << "SNOL> [" << temp << "] = " << temp << endl;
	else cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
}

void Storage::ASSIGN(string command) {
	string temp, expr, var;
	// Perform expression evaluation similar to in syntax checking, break down expression part by part
	for (int i = 0; i < command.length(); i++) {
		if (isOperator(command[i])) {	// Current operator is an operator
			if (command[i] == '-' && temp.length() == 0) {
				temp += command[i];
				continue;
			}
			if (isVar(temp)) {	// Is part in variable syntax
				if(doesVarExist(temp)) expr += " " + variables.at(temp) + " " + command[i];	// Add variable value and operator to the string
				else {
					cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
					return;
				}
			}
			else expr += " " + temp + " " + command[i];
			temp.erase();
		}
		//new changes
		else if (command[i] == ' ') {// Ignore spaces
			if ((command[i + 1] == '-') && (isdigit(command[i + 2]))) {
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
				return;
			}
			else if ((isdigit(command[i - 1])) && (isdigit(command[i + 1]))) {
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
				return;
			}
			else continue;
		}	
		//----------
		else if (command[i] == '=') {
			var += temp;	// Get variable name
			temp.erase();
		}
		else temp += command[i];	// Add character to part to be assessed
	}

	// Same as above, but for the last part before end of string
	if (isVar(temp)) {
		if (doesVarExist(temp)) expr += " " + variables.at(temp);	// Add variable value and operator to the string
		else {
			cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
			return;
		}
	}
	else expr += " " + temp;
	temp.erase();
	command.erase();
	cout << "EXPR =" << expr << endl; // For checking only removable

	//conditions if the expression is only a digit with no operations
	if (isDigit(expr)) {
		variables[var] = expr;
	}
	else {
		// for the infix to postfix conversion
		stack <char> s;
		string postfix = infixToPostfix(s, expr);
		string ans = "";

		//checks if all numbers have the same data type
		if (!checkError(postfix)) {
			cout << "Error!" << endl;
			return;
		}
		else {
			//cout << "Postfix Expression: " << postfix << endl;

			if (checkType(postfix)) {
				stack <float> i;
				ans = evaluateIntPostfix(i, postfix);
				if (ans == "ERROR") {
					cout << "SNOL> Error! Operands must be in int type to perform Modulo!";
					return;
				}
			}
			else {
				stack <float> f;
				ans = evaluateFloatPostfix(f, postfix);
				if (ans == "ERROR") {
					cout << "SNOL> Error! Operands must be in int type to perform Modulo!";
					return;
				}
				//cout << "Answer: " << ans << endl;
			}
			variables[var] = ans;
		}
	}
}

bool Storage::VAR_CHECK(string command) {
	string var;
	for (int i = 0; i < command.length(); i++) {
		if (isOperator(command[i])) {
			if (isVar(var) && !doesVarExist(var)) {
				cout << "SNOL> Error! [" << var << "] is not defined!" << endl;
				return false;
			}
			else var.erase();
		}
		else if (command[i] == ' ') {// Ignore spaces
			if ((command[i + 1] == '-') && (isdigit(command[i + 2]))) {
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
				return false;
			}
			else if ((isdigit(command[i - 1])) && (isdigit(command[i + 1]))) {
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
				return false;
			}
			else continue;
		}
		var += command[i];
	}
	if (isVar(var) && !doesVarExist(var)) {
		cout << "SNOL> Error! [" << var << "] is not defined!" << endl;
		return false;
	}
	return true;
}

string Storage::GET_VAL(string command) {
	string temp, result;
	for (int i = 0; i < command.length(); i++) {
		if (isOperator(command[i])) {
			if (isVar(temp)) result += " " + variables.at(temp) + " " + command[i];
			else result += " " + temp + " " + command[i];
			temp.erase();
			continue;
		}
		else if (command[i] == ' ') continue;
		temp += command[i];
	}
	if (isVar(temp)) result += " " + variables.at(temp) ;
	else result += " " + temp;
	temp.erase();
	return result;
}


