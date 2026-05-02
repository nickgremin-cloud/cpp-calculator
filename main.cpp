#include <iostream>
#include "calculator.h"
using namespace std;

int main()
{
    if (!RunCalculatorCycle()) {
        return 1;
    }
    return 0;
}