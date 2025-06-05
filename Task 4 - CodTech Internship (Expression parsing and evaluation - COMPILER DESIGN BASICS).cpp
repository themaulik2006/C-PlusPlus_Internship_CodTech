#include <iostream>
#include <stack>     // STL package for DSA implementation of stack
#include <string>
#include <cctype>    // for isdigit
#include <cmath>     // for pow

using namespace std;

// NOTE: For pop() - first display the top element (maybe store it somewhere) then pop it

// size_t <variable> refers to an unsigned integer which is specifically 
// meant to represent sizes and counts, like the length of a string, size of an array, etc

// Check if character is operator
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^';
}

// Simple check: unbalanced parentheses and invalid starting operator (except unary minus not handled)
void simpleValidate(const string& infix) {
    int parenCount = 0;
    if (infix.empty()) {
        throw runtime_error("Empty expression.");
    }
    // Check first char
    if (isOperator(infix[0]) && infix[0] != '-') {
        throw runtime_error("Expression starts with invalid operator.");
    }

    for (char c : infix) {
        if (c == '(') parenCount++;
        else if (c == ')') {
            parenCount--;
            if (parenCount < 0) {
                throw runtime_error("Unbalanced parentheses: too many closing parentheses.");
            }
        }
    }
    if (parenCount != 0) {
        throw runtime_error("Unbalanced parentheses: mismatched count.");
    }
}

// Function to return precedence of operators
int precedence(char ch) {
    if (ch == '+' || ch == '-')
        return 1;
    else if (ch == '%' || ch == '/' || ch == '*')
        return 2;
    else if (ch == '^')
        return 3;
    else // any other character which is not an operator has the least precedence (0)
        return 0;
}

// Function to convert infix expression to postfix
void infixToPostfix(const string& infix, string& postfix) {
    stack<char> stk;  // operator stack

    for (size_t i = 0; i < infix.length(); ) {
        if (isdigit(infix[i])) {
            // Read full number
            string number;
            while (i < infix.length() && isdigit(infix[i])) {
                number += infix[i];
                i++;
            }
            postfix += number + ' ';  // add number + space (so tokens are separated)
            continue;  // skip rest, don't increment i here because we already moved i inside while
        } else {
            // process operators, parentheses as before
            char c = infix[i];

            if (c == '(') {
                stk.push(c);
            } 
            else if (c == ')') {
                while (!stk.empty() && stk.top() != '(') {
                    postfix += stk.top();
                    postfix += ' ';
                    stk.pop();
                }
                if (!stk.empty())
                    stk.pop();  // pop '('
            } 
            else {
                // operator encountered
                while (!stk.empty() && precedence(c) <= precedence(stk.top())) {
                    postfix += stk.top();
                    postfix += ' ';
                    stk.pop();
                }
                stk.push(c);
            }
            i++;  // increment i only here for non-digit chars
        }
    }

    // Pop all remaining operators
    while (!stk.empty()) {
        if (stk.top() == '(' || stk.top() == ')') {
            throw runtime_error("Unbalanced parentheses detected at the end.");
        }
        postfix += stk.top();
        postfix += ' ';
        stk.pop();
        }
}

// Function to evaluate postfix expression without using stringstream or tokenization
int postfixEvaluation(const string& postfix) {
    stack<int> stk;  // stack for operands
    size_t i = 0;

    while (i < postfix.length()) {
        if (isdigit(postfix[i])) {
            // Parse full number (multi-digit)
            int num = 0;
            while (i < postfix.length() && isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            stk.push(num);
        } 
        else if (postfix[i] == ' ') {
            // Skip spaces
            i++;
        }
        else {
            if (stk.size() < 2) {
                throw runtime_error("Invalid postfix expression: insufficient operands.");
            }
            // operator
            int a = stk.top(); stk.pop();
            int b = stk.top(); stk.pop();

            switch (postfix[i]) {
                case '+': stk.push(b + a); break;
                case '-': stk.push(b - a); break;
                case '*': stk.push(b * a); break;
                case '/':
                    if (a == 0)
                        throw runtime_error("Division by zero error!");
                    stk.push(b / a);
                    break;
                case '%':
                    if (a == 0)
                        throw runtime_error("Modulo by zero error!");
                    stk.push(b % a);
                    break;
                case '^': stk.push(pow(b, a)); break;
            }
            i++;
        }
    }

    return stk.top();
}

int main() {
    string infix, postfix;

    cout << "Enter an infix expression: ";
    cin >> infix;

    try {
        simpleValidate(infix);

        infixToPostfix(infix, postfix);
        cout << "\nPostfix expression: " << postfix << endl;

        int result = postfixEvaluation(postfix);
        cout << "Evaluated Result: " << result << endl;
    }
    catch (runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
