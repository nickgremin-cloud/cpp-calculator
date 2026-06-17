#pragma once
// hello
#include <cmath>

using Number = double;

class Calculator {
public:
    Calculator() : number_(0) {}
    void Set(Number n) { number_ = n; }
    Number GetNumber() const { return number_; }
    void Add(Number n) { number_ += n; }
    void Sub(Number n) { number_ -= n; }
    void Mul(Number n) { number_ *= n; }
    void Div(Number n) { number_ /= n; }
    void Pow(Number n) { number_ = std::pow(number_, n); }
private:
    Number number_;
};
