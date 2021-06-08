#pragma once
#include <iostream>
#include <string>
#include <stack>

using namespace std;

int precedence(char value);
string infixToPostfix(stack <char> stack, string infix);
bool checkError(string postfix);
bool checkType(string postfix);
