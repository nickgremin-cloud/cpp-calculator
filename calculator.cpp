#include "calculator.h"
#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <cctype>

static bool isValidNumber(const std::string& str, Number& result);

bool ReadNumber(Number& result)
{
    std::string input;
    std::cin >> input;

    Number value;
    if (isValidNumber(input, value)) {
        result = value;
        return true;
    } else {
        std::cerr << "Error: Numeric operand expected" << std::endl;
        return false;
    }
}

bool RunCalculatorCycle()
{
    Number currentValue = 0;
    bool memoryHasValue = false;
    Number memoryStore = 0;

    std::string token;
    bool isFirst = true;

    while (std::cin >> token)
    {
        if (isFirst) {
            Number firstNum;
            if (isValidNumber(token, firstNum)) {
                currentValue = firstNum;
                isFirst = false;
                continue;
            } else {
                std::cerr << "Error: Numeric operand expected" << std::endl;
                return false;
            }
        }

        if (token == "q") {
            return true;
        }
        
        if (token == "=") {
            std::cout << currentValue << std::endl;
            continue;
        }
        
        if (token == "c") {
            currentValue = 0;
            continue;
        }
        
        if (token == "s") {
            memoryStore = currentValue;
            memoryHasValue = true;
            continue;
        }
        
        if (token == "l") {
            if (!memoryHasValue) {
                std::cerr << "Error: Memory is empty" << std::endl;
                return false;
            }
            currentValue = memoryStore;
            continue;
        }

        if (token == "+") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            currentValue = currentValue + operand;
            continue;
        }

        if (token == "-") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            currentValue = currentValue - operand;
            continue;
        }

        if (token == "*") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            currentValue = currentValue * operand;
            continue;
        }

        if (token == "/") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            
            if (operand == 0) {
                currentValue = std::numeric_limits<Number>::infinity();
            } else {
                currentValue = currentValue / operand;
            }
            continue;
        }

        if (token == "**") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            currentValue = std::pow(currentValue, operand);
            continue;
        }

        if (token == ":") {
            Number operand;
            if (!ReadNumber(operand)) {
                return false;
            }
            currentValue = operand;
            continue;
        }

        Number checkValue;
        if (isValidNumber(token, checkValue)) {
            std::cerr << "Error: Numeric operand expected" << std::endl;
            return false;
        } else {
            std::cerr << "Error: Unknown token " << token << std::endl;
            return false;
        }
    }

    return true;
}

static bool isValidNumber(const std::string& str, Number& result) {
    if (str.empty()) return false;
    
    size_t pos = 0;
    bool hasDigit = false;
    
    if (str[pos] == '+' || str[pos] == '-') {
        pos++;
    }
    
    while (pos < str.length() && std::isdigit(str[pos])) {
        hasDigit = true;
        pos++;
    }
    
    if (pos < str.length() && str[pos] == '.') {
        pos++;
    }
    
    while (pos < str.length() && std::isdigit(str[pos])) {
        hasDigit = true;
        pos++;
    }
    
    if (pos < str.length() && (str[pos] == 'e' || str[pos] == 'E')) {
        pos++;
        if (pos < str.length() && (str[pos] == '+' || str[pos] == '-')) {
            pos++;
        }
        while (pos < str.length() && std::isdigit(str[pos])) {
            hasDigit = true;
            pos++;
        }
    }
    
    if (pos == str.length() && hasDigit) {
        char* endptr;
        result = std::atof(str.c_str());
        return true;
    } else {
        return false;
    }
}