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

string infixToPostfix(stack <char> stack, string infix) {
    string postfix;

    for (int i = 0; i < infix.length(); i++) { 								//scan the string per character
        if ((isdigit(infix[i])) || (infix[i] == '.')) {						// check if the current character is a digit or a period
            postfix += infix[i];											//append the current character to the postfix string
            if (infix[i + 1] == '.') continue;								//check if the next character of the current character is a period (if float), ignore
            else if (!isdigit(infix[i + 1])) postfix += " ";				//check if the next character of the current character is not a digit (it may be two digits or more)
        }
        else if (infix[i] == ' ') {											//check if the current character is a space then ignore
            continue;
        }
        else if (infix[i] == '(') {											//check if it is an open parentheses
            stack.push(infix[i]);											//push the open parentheses
        }
        else if (infix[i] == ')') {											//check if it is a closed parentheses 
            while ((stack.top() != '(') && (!stack.empty())) {				//loop until it meets the open parentheses or the stack is empty
                char temp = stack.top();
                postfix += temp;											//append the top of the stack to the postfix string
                postfix += " ";												// this will add spaces after each append in the postfix string
                stack.pop();
            }
            if (stack.top() == '(') {											//check if it encounters an open parentheses then pop
                stack.pop();
            }
        }
        else { 																//if the current infix[i] is operator
            if (stack.empty()) {												//checks if stack is empty then push the current infix[i]
                stack.push(infix[i]);
            }
            else {															//checks the precedence and of the top of the stack and the current infix[i]
                if (precedence(infix[i]) > precedence(stack.top())) {		//push to the stack the current infix[i] if the precedence of the top of the stack is less than the current character
                    stack.push(infix[i]);
                }
                else if (precedence(infix[i]) == precedence(stack.top())) { 	//check if the precedence of the current character is equal to the stack top 
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
    while (!stack.empty()) {													//if there are remaining operators in the stack pop it
        postfix += stack.top();
        postfix += " ";
        stack.pop();
    }
    return postfix;
}

// need to add the evaluation for the postfix expression 
// postfix expression is returned from the function with the var name == postfix
// it is called in the storage.cpp. function calls are done in the storage.cpp