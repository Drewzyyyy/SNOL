#include <iostream>
#include <string>
#include <stack>
#include <ctype.h>
#include "Calculation.h"
#include "Util.h"

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
    return (postfix.find('.') >= postfix.length());
    // Returns true if Integer, then false if float
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
            if (stack.top() == '(') {										//check if it encounters an open parentheses then pop
                stack.pop();
            }
        }
        else { 																//if the current infix[i] is operator
            if (infix[i] == '-' && isdigit(infix[i + 1])) {
                postfix += infix[i];
                continue;
            }
            if (stack.empty()) {											//checks if stack is empty then push the current infix[i]
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

bool ifInt(float num){
	int x=num;
	float y=num-x;
	
	if(y>0){
		return false;
	}
	return true;
}

string evaluateIntPostfix(stack <float> mystack, string postfix){
	string num;
	
	 for (int i = 0; i < postfix.length(); i++){
	 	if(isdigit(postfix[i])) {		
            num += postfix[i];														
        	if (!isdigit(postfix[i + 1])) num += " ";			
        }
        else if (postfix[i]==' ' && postfix[i+1] != NULL) {										
			int b=atoi(num.c_str()); 
        	mystack.push(b);
            num.erase();
            continue;
        }
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) {
            num += postfix[i];
            continue;
        }
        else {
        	float val1 = mystack.top();
        	mystack.pop();
        	float val2 = mystack.top();	
        	mystack.pop();
        	
        	if(postfix[i]=='%'){
        		if(ifInt(val1)==true && ifInt(val2)==true){																	//checks if both val1 and val2 is int
	        		int v1=val1; 
	        		int v2=val2;
					mystack.push(v2%v1);
				}
				else{
					return "ERROR";
				}
			}
			        	
        	switch(postfix[i]) { 
	            case '+': mystack.push(val2+val1); break;
	            case '-': mystack.push(val2-val1); break;
	            case '*': mystack.push(val2*val1); break; 
	            case '/': mystack.push(val2/val1); break; 
            }
            i++;
		}
	 }
	 
	float ans=mystack.top();
	 
	if(ifInt(ans)==true){
		int ans2=mystack.top();
		mystack.pop();
		string ret= to_string(ans2);
		return ret;
	}
	else{
		mystack.pop();
		string ret= to_string(ans);
		return ret;
	}
	
}

string evaluateFloatPostfix(stack <float> mystack, string postfix){
	string num;
	
	 for (int i = 0; i < postfix.length(); i++){
	 	if ((isdigit(postfix[i])) || (postfix[i] == '.')) {		
            num += postfix[i];								
            if (postfix[i + 1] == '.') continue;						
            else if (!isdigit(postfix[i + 1])) num += " ";			
        }
        else if (postfix[i]==' ' && postfix[i+1]!=NULL) {										
			float a=atof(num.c_str()); 
    		mystack.push(a);
            num.erase();
            continue;
        }
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) {
            num += postfix[i];
            continue;
        }
        else if (postfix[i] == '%') {
            return "Error";
        }
        else {
        	float val1 = mystack.top();
        	mystack.pop();
        	float val2 = mystack.top();	
        	mystack.pop();
			        	
        	switch(postfix[i]) { 
	            case '+': mystack.push(val2+val1); break;
	            case '-': mystack.push(val2-val1); break;
	            case '*': mystack.push(val2*val1); break; 
	            case '/': mystack.push(val2/val1); break; 
            }
            i++;
		}
	 }
	
	float ans=mystack.top();
	if(ifInt(ans)==true){
		int ans2=mystack.top();
		mystack.pop();
		string ret= to_string(ans2);
		return ret;
	}
	else{
		mystack.pop();
		string ret= to_string(ans);
		return ret;
	}	
}
