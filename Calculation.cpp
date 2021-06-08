#include <iostream>
#include <string>
#include <stack>
#include <ctype.h>
#include "Calculation.h"

using namespace std;

int precedence(char value) {
    if (value == '*' || value == '/' || value == '%') {
        return 2;
    }
    else if (value == '+' || value == '-') {
        return 1;
    }
    else return -1;
}

bool checkError(string postfix){
    int prev, curr;
    string tmp, tmp1;
    int j;
    
    // 1 if int; 0 if float
    
    for (int i = 0; i < postfix.length(); i++) {          // Loop checks each character to see if all the numbers have the same data type
        if (isdigit(postfix[i]) || postfix[i]=='.') {     // Stores character into a temporary string if num or period
            tmp+= postfix[i];
            continue;
        } else {
            if (postfix[i] == ' ' && !tmp.empty()) {      // If char is space and tmp is not empty, it will go in the if statement
                if (tmp.find(".") >= tmp.length()) { //num is int
                    prev= 1;
                } else {
                    prev = 0; // num is float
                }
                
                if ((i+1) < postfix.length()) {
                    for (j = i+1; j < postfix.length(); j++){   // Loop to find the next number to compare with tmp
                        if (isdigit(postfix[j]) || postfix[j]=='.') {   // Stores character into a temporary string if num or period
                            tmp1+= postfix[j];
                            continue;
                        } else {
                            if (postfix[j] == ' ') {    // If char is space, it will compare the previous number and the next number
                                if (tmp1.find(".") >= tmp1.length()) { // num is int
                                    curr = 1;
                                } else {                                // num is float
                                    curr = 0;
                                }
                                
                                if (prev != curr) {     // If prev != curr, then the two data types do not match
                                    return false;       // Returns false
                                }
                                
                                tmp = tmp1;             // Gets the current number and stores it into the temporary string storage for the previous number
                                tmp1.clear();           // Clears tmp1
                                break;
                            }else continue;
                        }
                    }
                    i = j;
                }
            }else continue;
        }
    }
    
    return true;
}

bool checkType(string postfix){
    int prev;
    string tmp, tmp1;
    
    // 1 if int; 0 if float
    
    for (int i = 0; i < postfix.length(); i++) {          // Loop checks each character to see if all the numbers have the same data type
        if (isdigit(postfix[i]) || postfix[i]=='.') {     // Stores character into a temporary string if num or period
            tmp+= postfix[i];
            continue;
        } else {
            if (postfix[i] == ' ' && !tmp.empty()) {      // If char is space and tmp is not empty, it will go in the if statement
                if (tmp.find(".") >= tmp.length()) {
                    prev= 1;  //num is int
                } else {
                    prev = 0; // num is float
                }
                break;
            }else continue;
        }
    }
    
    
    // Returns true if formula only has int/float
    if (prev == 1) {
        return true;  // Returns true if int
    } else {
        return false; // Returns false if float
    }
}


string infixToPostfix(stack <char> stack, string infix) {
    string postfix;

    for (int i = 0; i < infix.length(); i++) { 								//scan the string per character
        if ((isdigit(infix[i])) || (infix[i] == '.')) {						// check if the current character is a digit or a period
            postfix += infix[i];											//append the current character to the postfix string
            if (infix[i + 1] == '.') continue;								//check if the next character of the current character is a period (if float), ignore
            else if (!isdigit(infix[i + 1])) postfix += " ";	            //check if the next character of the current character is not a digit (it may be two digits or more)
        }
        else if (infix[i] == ' ') {								            //check if the current character is a space then ignore
            continue;
        }
        else if (infix[i] == '(') {								            //check if it is an open parentheses
            stack.push(infix[i]);							                //push the open parentheses
        }
        else if (infix[i] == ')') {								            //check if it is a closed parentheses
            while ((stack.top() != '(') && (!stack.empty())) {	            //loop until it meets the open parentheses or the stack is empty
                char temp = stack.top();
                postfix += temp;							                //append the top of the stack to the postfix string
                postfix += " ";									            // this will add spaces after each append in the postfix string
                stack.pop();
            }
            if (stack.top() == '(') {							            //check if it encounters an open parentheses then pop
                stack.pop();
            }
        }
        else { 																//if the current infix[i] is operator
            if (stack.empty()) {									    	//checks if stack is empty then push the current infix[i]
                stack.push(infix[i]);
            }
            else {															//checks the precedence and of the top of the stack and the current infix[i]
                if (precedence(infix[i]) > precedence(stack.top())) {		//push to the stack the current infix[i] if the precedence of the top of the stack is less than the current character
                    stack.push(infix[i]);
                }
                else if (precedence(infix[i]) == precedence(stack.top())) { //check if the precedence of the current character is equal to the stack top
                    postfix += stack.top();									//append the top of the stack to the postfix string
                    postfix += " ";
                    stack.pop();
                    stack.push(infix[i]);									//push the current infix [i] after popping the top of the stack
                }
                else {														//check if the precedence of the current infix[i] is less than the top of the stack
                    while ((!stack.empty()) && (precedence(infix[i]) < precedence(stack.top()))) {
                        postfix += stack.top();
                        postfix += " ";
                        stack.pop();
                    }
                    stack.push(infix[i]);
                }
            }
        }
    }
    while (!stack.empty()) {											    //if there are remaining operators in the stack pop it
        postfix += stack.top();
        postfix += " ";
        stack.pop();
    }
    
    if (checkError(postfix)) {
        return postfix;
    }else {
        return "Error.";
    }
}

// need to add the evaluation for the postfix expression 
// postfix expression is returned from the function with the var name == postfix
// it is called in the storage.cpp. function calls are done in the storage.cpp
